#include <stdutils/utf8.hpp>

#include "OsmBuildingCreator.h"
#include "OsmParameter.h"
#include <LiveLogFeature.hpp>
#include <ProgressbarFeature.hpp>
#include <ifcdb/utils/CenterPointCreator.h>
#include <MainFrameObserverImpl.hpp>
#include "lodDlgImpl.h"


double m_epsilonAngle = 0;
OsmBuildingCreator::OsmBuildingCreator(IfcDB::Populationi* pDB, IfcDB::utils::PopulationStates* pStates, sdm::plugin::LiveLogInterface* pLogger, sdm::plugin::ProgressBarInterface* pProgressBar, sdm::plugin::MainFrameInterface* pInterface, IfcDB::MessageLogger* pMessageLogger)
  : m_pDB(pDB), m_pStates(pStates), m_pLogger(pLogger), m_pProgressBar(pProgressBar), m_pMainFrameInterface(pInterface), m_pMessageLogger(pMessageLogger)
{
    m_geometrySettings = m_pDB->getPopulationContext().m_GeometrySettings;
}

void OsmBuildingCreator::countStats(int typeNr, bool success)
{
    if (typeNr == 1)
    {
        m_OSMstats.Type1_total++;
        if (success)
        {
            m_OSMstats.Type1_success++;
        }
    }
    else if (typeNr == 2)
    {
        m_OSMstats.Type2_total++;
        if (success)
        {
            m_OSMstats.Type2_success++;
        }
    }
    else if (typeNr == 3)
    {
        m_OSMstats.Type3_total++;
        if (success)
        {
            m_OSMstats.Type3_success++;
        }
    }
    else
    {
        m_OSMstats.Type4_total++;
        if (success)
        {
            m_OSMstats.Type4_success++;
        }
    }
}

void OsmBuildingCreator::assignSatsType(BuildingParameter &buildingParameter, bool roofDefault, bool heightDefault)
{
    if (!roofDefault && !heightDefault)
    {
        buildingParameter.statisticType.typeNr = 1;
    }
    else if (!roofDefault && heightDefault)
    {
        buildingParameter.statisticType.typeNr = 2;
    }
    else if (roofDefault && !heightDefault)
    {
        buildingParameter.statisticType.typeNr = 3;
    }
    else
    {
        buildingParameter.statisticType.typeNr = 4;
    }
}

void OsmBuildingCreator::build()
{
    Lod1DlgImpl* dlg = new Lod1DlgImpl(nullptr, wxID_ANY, wxT("OSM Building Creator - LOD 1"), wxDefaultPosition, wxSize( 372,297 ), wxDEFAULT_DIALOG_STYLE);
    dlg->ShowModal();

    if (dlg->m_cancel)
    {
        return;
    }
    if (dlg->m_bttnChoice == 0)
    {
        m_BoundarySurface = true;
    }
    else
    {
        m_BoundarySurface = false;
    }

    IfcDB::IfcEntityList entities;

  IfcDB::tOidSet selectedEntities = m_pStates->getSelectedEntities();
  m_pMainFrameInterface->setSaveFileFlag(true);

  if (!selectedEntities.empty())
  {
    for (auto oid : selectedEntities)
    {
      IfcDB::IfcEntity* pEntity = m_pDB->get(oid);

      if (pEntity != nullptr)
      {
        entities.emplace_back(pEntity);
      }
    }
  }
  else
  {
    m_pDB->getAll(entities);
  }

  if (!IfcDB::Feature::hasGMLClasses())
  {
    IfcDB::Feature::createGMLClasses();
  }

  size_t nProcessedBuildings(0);
  IfcDB::ifcOid oidCityModel(false);
  if (!entities.empty())
  {
      oidCityModel = createCityModel();
  }

  m_pLogger->log("Comment", "Process OSM Buildings");

  std::wstring lod(L"lod1");

  m_pProgressBar->create();
  m_pProgressBar->setProgressData(entities.size(), "Processing OpenStreetMap elements:");

  for (auto pEntity : entities)
  {
    m_pProgressBar->increment();
    m_pProgressBar->process();

    if ((pEntity->getEntityType().find(L"OSM_building") ) != std::wstring::npos)
    {
        if ((pEntity->getEntityType().find(L"part") ) == std::wstring::npos || (pEntity->getEntityType().find(L"apartment") ) != std::wstring::npos )
        {
            ++nProcessedBuildings;

            IfcDB::ifcOid oidBuilding = createBuilding(oidCityModel, pEntity, lod);

            if (oidBuilding > 0)
            {
                createAddress(oidBuilding, pEntity);
            }
        }
    }
  }

  for (const auto& buildingF : buildingsMap)
  {
      m_pDB->createObjRelation(oidCityModel, buildingF->getOid(), L"core:cityObjectMember");
  }

  m_pStates->showRepresentationIdentifier(lod, true);

  m_pStates->compile();

  m_pDB->incrementSerialNo();

  m_pDB->getPopulationContext().m_GeometrySettings.update(m_geometrySettings);

  ///// Statistics

    wxString message = wxString::Format(
            "Type 1 (Roof shape and height known):    %d/%d\n"
            "Type 2 (Roof shape known and height default):    %d/%d\n"
            "Type 3 (Roof shape default and height known):    %d/%d\n"
            "Type 4 (Roof shape default and height default):    %d/%d\n\n",
            m_OSMstats.Type1_success, m_OSMstats.Type1_total,
            m_OSMstats.Type2_success, m_OSMstats.Type2_total,
            m_OSMstats.Type3_success, m_OSMstats.Type3_total,
            m_OSMstats.Type4_success, m_OSMstats.Type4_total
    );

    auto pDlg = new wxMessageDialog(nullptr, message, wxT("Successfully generated buildings"), wxOK, wxDefaultPosition);
    pDlg->ShowModal();
}

IfcDB::ifcOid OsmBuildingCreator::createCityModel()
{
  IfcDB::ModelInfo* pModelInfo = m_pDB->addModelInfo(L"", IfcDB::ModelInfo::MT_GML);

  IfcDB::Feature* pCityModel = new IfcDB::Feature(L"", L"core:CityModel", 0, true);
  pCityModel->setType(IfcDB::CITYGML_CITY_MODEL);

  m_pDB->add(pCityModel, pModelInfo);
  m_pDB->addGmlType(pCityModel->getOid(), IfcDB::CITYGML_2_0);

  return pCityModel->getOid();
}

IfcDB::ifcOid OsmBuildingCreator::createBuilding(IfcDB::ifcOid oidCityModel, IfcDB::IfcEntity* pEntity, const std::wstring& lod)
{
    double buildingHeight(m_OsmParameter.DefaultBuildingHeight);
    auto it = m_OsmParameter.MapDefaultHeights.find(pEntity->getEntityType());

    if (it != m_OsmParameter.MapDefaultHeights.end())
    {
        buildingHeight = it->second;
    }
    IfcDB::Geometry* pGeometry = pEntity->getGeometry();

    if (pGeometry != nullptr)
    {
        IfcDB::Feature* pBuilding = new IfcDB::Feature(L"", L"bldg:Building", oidCityModel, true);
        pBuilding->setType(IfcDB::CITYGML_BUILDING);
        pBuilding->setState(IfcDB::STATE_INITIAL_GEOMETRY_TRANSFORMATION);

        m_pDB->add(pBuilding, m_pDB->getModelInfoPending());

        IfcDB::Feature* pFeature = dynamic_cast<IfcDB::Feature*>(pEntity);
        BuildingParameter buildingParameter;

        if (pFeature != nullptr)
        {
            int64_t osmId;
            if (pFeature->getIntegerAttributWert(L"OSM_ID", osmId))
            {
                pBuilding->setIntegerAttribut(_T("OSM_ID"), osmId);
            }
            ////////
            std::wstring buildingType;
            if (pFeature->getStringAttributWert(L"building", buildingType))
            {
                auto it = m_OsmParameter.MapDefaultHeights.find(buildingType);
                if (it != m_OsmParameter.MapDefaultHeights.end())
                {
                    buildingHeight = it->second;
                }
            }

            bool roofDeafault = true;
            bool heightDeafault = true;
            std::vector<std::wstring > attributeNames;
            pFeature->getAttributeNames(IfcDB::AT_STRING, attributeNames);

            for (const auto& att : attributeNames)
            {
                std::wstring currentAtt;
                if (pFeature->getStringAttributWert(att, currentAtt))
                {
                    std::wstring attribute;
                    if (att != _T("OSM_ID"))
                    {
                        attribute = _T("OSM_") + att;
                    }
                    else
                    {
                        attribute = att;
                    }
                    pBuilding->setStringAttribut(attribute, currentAtt);
                }
            }
            ////////
            std::wstring year;
            if (pFeature->getStringAttributWert(L"year_of_construction", year) || pFeature->getStringAttributWert(L"construction_date", year))
            {
                m_pLogger->log("Comment", "Year of construction availabe: " + toUtf8(year));
                pBuilding->setStringAttribut(L"bldg:yearOfConstruction", year);
            }

            std::wstring roofTypeStr;
            std::wstring roofOrientation;
            std::wstring roofDirection;
            std::wstring roofHeightValue;
            std::wstring roofAngleValue;
            std::wstring roofColour;
            std::wstring roofMaterial;

            std::wstring ridge;
            std::wstring ridgeHeight;

            if (pFeature->getStringAttributWert(L"roof:shape", roofTypeStr))
            {
                roofDeafault = false;
                if (roofTypeStr == L"3dr")
                {
                    if (pFeature->getStringAttributWert(L"3dr:type", roofTypeStr))
                    {
                        auto it2 = m_OsmParameter.MapRoofShape3drCodelist.find(roofTypeStr);
                        if (it2 != m_OsmParameter.MapRoofShape3drCodelist.end())
                        {
                            roofTypeStr = it2->second;
                        }
                    }
                }
                buildingParameter.roof.shape = roofTypeStr;
                auto it = m_OsmParameter.MapRoofShape2RoofTypeCodelist.find(roofTypeStr);

                if (it != m_OsmParameter.MapRoofShape2RoofTypeCodelist.end())
                {
                    pBuilding->setStringAttribut(L"bldg:roofType", it->second);
                }
            }

            if (pFeature->getStringAttributWert(L"roof:height", roofHeightValue) == true)
            {
                buildingParameter.roof.height = _ttof(roofHeightValue.c_str());
            }
            else if (pFeature->getStringAttributWert(L"roof:levels", roofHeightValue) == true)
            {
                buildingParameter.roof.height = (_ttof(roofHeightValue.c_str())) * m_OsmParameter.DefaultStoreyHeight;
            }

            if (buildingParameter.roof.height)
            {
                buildingParameter.roof.height = buildingParameter.roof.height/2;
            }

            double numberOfStoreys(0);

            std::wstring levelValueStr;
            std::wstring min_levelValueStr;
            std::wstring buildingColour;
            std::wstring buildingMaterial;

            if (pFeature->getStringAttributWert(L"building:levels", levelValueStr) == true)
            {
                heightDeafault = false;
                numberOfStoreys = _ttof(levelValueStr.c_str());
                pBuilding->setIntegerAttribut(_T("bldg:storeysAboveGround"), numberOfStoreys);
                buildingParameter.levels = true;
            }

            if (pFeature->getStringAttributWert(L"building:levels:underground", levelValueStr) == true)
            {
                numberOfStoreys = _ttof(levelValueStr.c_str());
                pBuilding->setIntegerAttribut(_T("bldg:storeysBelowGround"), numberOfStoreys);
            }

            if (pFeature->getStringAttributWert(L"min_height", min_levelValueStr) == true)
            {
                double minHeight = _ttof(min_levelValueStr.c_str());
                buildingParameter.BuildingStartHeight = minHeight;
            }
            else if (pFeature->getStringAttributWert(L"building:min_level", min_levelValueStr) == true)
            {
                double minLevel = _ttof(min_levelValueStr.c_str());
                buildingParameter.BuildingStartHeight = minLevel * m_OsmParameter.DefaultStoreyHeight;
            }

            std::wstring heightValueStr;
            if (pFeature->getStringAttributWert(L"height", heightValueStr) == true)
            {
                heightDeafault = false;
                buildingHeight = _ttof(heightValueStr.c_str());

                IfcDB::UOM* pUOM = m_pDB->getUomList()->getUomLength();

                pBuilding->setDoubleAttribut(_T("bldg:measuredHeight"), buildingHeight, pUOM);

                if (buildingParameter.roof.height)
                {
                    buildingHeight -= buildingParameter.roof.height;
                }
            }
            else if (numberOfStoreys > 0)
            {
                buildingHeight = m_OsmParameter.DefaultStoreyHeight * numberOfStoreys;
            }
            else if (buildingParameter.BuildingStartHeight > 0)
            {
                buildingHeight = buildingParameter.BuildingStartHeight;
            }

            if (buildingParameter.BuildingStartHeight == 0 && buildingType == _T("roof"))
            {
                buildingParameter.BuildingStartHeight = buildingHeight - 0.2;
            }

            std::wstring name;
            if (pFeature->getStringAttributWert(L"name", name) == true)
            {
                pBuilding->setName(name.c_str());
                pBuilding->setStringAttribut(L"gml:name", name);
            }
            else
            {
                IfcDB::Feature* pOsmFeature = dynamic_cast<IfcDB::Feature*>(pEntity);

                std::wstring street, number;

                if (pOsmFeature->getStringAttributWert(_T("addr:street"), street) == true)
                {
                    if (pOsmFeature->getStringAttributWert(_T("addr:housenumber"), number) == true)
                    {
                        std::wstring name(street + _T(" ") + number);

                        pBuilding->setName(name.c_str());
                        pBuilding->setStringAttribut(L"gml:name", name);
                    }
                    else
                    {
                        pBuilding->setName(street.c_str());
                        pBuilding->setStringAttribut(L"gml:name", street);
                    }
                }
                else
                {
                    pBuilding->setName(pEntity->getEntityType().c_str());
                    pBuilding->setStringAttribut(pEntity->getEntityType(), street);
                }
            }
            assignSatsType(buildingParameter,roofDeafault, heightDeafault);
        }

        buildingParameter.BuildingHeight = buildingHeight;
        createBuidlingSolid(pEntity, buildingParameter);

        if (!buildingParameter.vFacesBuilding.empty())
        {
            for (const auto& facesPart : buildingParameter.vFacesBuilding)
            {
                for (auto pFace : facesPart)
                {
                    pFace->recalcFaceTransformation();
                }
                    IfcDB::GmlSolid* pGmlSolid = new IfcDB::GmlSolid();
                    IfcDB::CompositeSurface* pCompositeSurface = new IfcDB::CompositeSurface();
                    pCompositeSurface->setFaces(facesPart);

                    pGmlSolid->setExterior(pCompositeSurface);

                    std::wstring representationType(L"bldg:" + lod + L"Solid");
                    IfcDB::sRepresentation representationLoD1Ridge(0, lod, representationType, pGmlSolid);
                    pBuilding->addRepresentation(representationLoD1Ridge);

                    m_pStates->registerEntityHash(pBuilding->getOid(), representationLoD1Ridge.m_representationIdentifier,representationLoD1Ridge.m_representationType, {});
                    if (m_BoundarySurface)
                    {
                        createBoundarySurfaces(pBuilding, L"lod2", true);
                    }
                    buildingsMap.push_back(pBuilding);
            }
        }
        else
        {
            std::stringstream message;
            message << "Failed to create building solid: " << toUtf8(pBuilding->getGmlId());

            if (buildingParameter.BuildingHeight <= 0.0)
            {
                message << ", incorrect building height = 0.0";
            }

            m_pLogger->log("Warning", message.str());
        }
        std::wstring currentAtt = _T("OSM_BuildingInfoType");
        pBuilding->setIntegerAttribut(currentAtt, buildingParameter.statisticType.typeNr);
        currentAtt = _T("OSM_BuildingCreated");

        pBuilding->setBoolAttribut(currentAtt,buildingParameter.statisticType.success);
        countStats(buildingParameter.statisticType.typeNr, buildingParameter.statisticType.success);

        return pBuilding->getOid();
    }

    return 0;
}

IfcDB::ifcOid OsmBuildingCreator::createAddress(IfcDB::ifcOid oidBuilding, IfcDB::IfcEntity* pEntity)
{
  bool foundAddress(false);

  IfcDB::Feature* pOsmFeature = dynamic_cast<IfcDB::Feature*>(pEntity);

  std::wstring city, street;

  if (pOsmFeature->getStringAttributWert(_T("addr:city"), city) == true ||
      pOsmFeature->getStringAttributWert(_T("addr:street"), street) == true)
  {
    foundAddress = true;
  }

  if (foundAddress == true)
  {
    IfcDB::Feature* pAddress = m_pDB->createFeature(_T(""), _T("core:xalAddress"), oidBuilding, true);
    pAddress->setType(IfcDB::CITYGML_ADDRESS);

    std::wstring value;

    if (pOsmFeature->getStringAttributWert(_T("addr:country"), value) == true)
    {
      pAddress->setStringAttribut(_T("xAL:CountryName"), value);
    }

    if (pOsmFeature->getStringAttributWert(_T("addr:city"), value) == true)
    {
      pAddress->setStringAttribut(_T("xAL:LocalityName"), value);
    }

    //if (pOsmFeature->getStringAttributWert(_T(""), value) == true)
    //{
    //  pAddress->setStringAttribut(_T("xAL:PostBoxNumber"), value);
    //}

    if (pOsmFeature->getStringAttributWert(_T("addr:housenumber"), value) == true)
    {
      pAddress->setStringAttribut(_T("xAL:ThoroughfareNumber"), value);
    }

    if (pOsmFeature->getStringAttributWert(_T("addr:street"), value) == true)
    {
      pAddress->setStringAttribut(_T("xAL:ThoroughfareName"), value);
    }

    if (pOsmFeature->getStringAttributWert(_T("addr:postcode"), value) == true)
    {
      pAddress->setStringAttribut(_T("xAL:PostalCodeNumber"), value);
    }

    m_pDB->createObjRelation(oidBuilding, pAddress->getOid(), _T("Address"));

    return pAddress->getOid();
  }

  return 0;
}

bool OsmBuildingCreator::createBuidlingSolid(IfcDB::IfcEntity* pOsmEntity, BuildingParameter& buildingParameter)
{
    bool state(false);

    IfcDB::Profile* pProfile = nullptr;
    std::vector<IfcDB::Profile*> vProfiles;

    IfcDB::Geometry* pGeometry = pOsmEntity->getGeometry();

    if (pGeometry->isA(IfcDB::POLYLINE))
    {
        IfcDB::CPolyline* pPolyline = dynamic_cast<IfcDB::CPolyline*>(pGeometry);

        pProfile = new IfcDB::Profile();

        if (pPolyline->isClosed())
        {
            pProfile->setOuterProfile(pPolyline->cloneCurve());
            vProfiles.push_back(pProfile);
        }
    }
    else if (pGeometry->isA(IfcDB::FACE))
    {
        IfcDB::Face* pFace = dynamic_cast<IfcDB::Face*>(pGeometry);

        if (pFace->getOuterLoop())
        {
            pProfile = new IfcDB::Profile();

            pProfile->setOuterProfile(pFace->getOuterLoop()->cloneCurve());

            for (const auto& pInnerLoop : pFace->getInnerLoops())
            {
                pProfile->addInnerProfile(pInnerLoop->cloneCurve());
            }
            vProfiles.push_back(pProfile);
        }
    }
    else if (pGeometry->isA(IfcDB::SET_OF_GEOMETRY))
    {
        IfcDB::SetOfGeometry* pSet = dynamic_cast<IfcDB::SetOfGeometry*>(pGeometry);
        for (const auto& pgeometry : pSet->getGeometries())
        {
            if (pgeometry->isA(IfcDB::POLYLINE))
            {
                IfcDB::CPolyline* pPolyline = dynamic_cast<IfcDB::CPolyline*>((pgeometry));
                pProfile = new IfcDB::Profile();
                if (pPolyline->isClosed())
                {
                    pProfile->setOuterProfile(pPolyline->cloneCurve());
                }
            }
            else if (pgeometry->isA(IfcDB::FACE))
            {
                IfcDB::Face* pFace = dynamic_cast<IfcDB::Face*>(pgeometry);

                if (pFace->getOuterLoop())
                {
                    pProfile = new IfcDB::Profile();

                    pProfile->setOuterProfile(pFace->getOuterLoop()->cloneCurve());

                    for (const auto& pInnerLoop : pFace->getInnerLoops())
                    {
                        pProfile->addInnerProfile(pInnerLoop->cloneCurve());
                    }
                    vProfiles.push_back(pProfile);
                }
            }
        }
    }

    for (auto pProfile: vProfiles)
    {
        pProfile->setPlacement(new IfcDB::Matrix4());

        buildingParameter.statisticType.success = true;
        IfcDB::Matrix4* pMatrix = new IfcDB::Matrix4();
        if ((buildingParameter.BuildingHeight == buildingParameter.BuildingStartHeight) && buildingParameter.BuildingHeight > 0)
        {
            buildingParameter.BuildingHeight += 3;
        }
        pProfile->translate(0,0,buildingParameter.BuildingStartHeight);
        IfcDB::Extrusion extrusion(pProfile, pMatrix, { 0.0, 0.0, 1.0 },  (buildingParameter.BuildingHeight + buildingParameter.roof.height - buildingParameter.BuildingStartHeight));

        IfcDB::Brep* brepClone{dynamic_cast<IfcDB::Brep*>(IfcDB::Geometry::toBrep(m_pDB->getPopulationContext(), &extrusion , true)) };
        IfcDB::GmlSolid* pGmlSolid = new IfcDB::GmlSolid();
        IfcDB::CompositeSurface* pCompositeSurface = new IfcDB::CompositeSurface();
        pGmlSolid->setExterior(pCompositeSurface);
        bool solid = brepClone->checkEdges(m_pMessageLogger);

        if (!solid) {
            buildingParameter.statisticType.success = false;
        }

        buildingParameter.vFacesBuilding.emplace_back(brepClone->getFaces());
        state = solid;
    }


    return state;
}

bool OsmBuildingCreator::createBoundarySurfaces(IfcDB::Feature* pBuilding, const std::wstring& lod, bool createGeoReference/*= true*/)
{
    bool state(false);

    // create thermal boundaries from building solid

    for (auto pGeometry : pBuilding->getGeometries())
    {
        if (pGeometry && pGeometry->getType() == IfcDB::GML_SOLID)
        {
            IfcDB::GmlSolid* pGmlSolid = dynamic_cast<IfcDB::GmlSolid*>(pGeometry);

            IfcDB::Geometry* pExterior = pGmlSolid->getExterior();

            if (pExterior && pExterior->getType() == IfcDB::COMPOSITE_SURFACE)
            {
                IfcDB::CompositeSurface* pCompositeSurface = dynamic_cast<IfcDB::CompositeSurface*>(pExterior);

                std::vector<IfcDB::Geometry*> references;

                for (auto pSurface : pCompositeSurface->getGeometries())
                {
                    if (pSurface->getType() == IfcDB::FACE)
                    {
                        IfcDB::Face* pFace = dynamic_cast<IfcDB::Face*>(pSurface);

                        IfcDB::Feature* pBoundarySurface = createBoundarySurface(pBuilding, pFace, lod, createGeoReference);

                        // create geometry reference
                        if (createGeoReference == true)
                        {
                            IfcDB::GeometryReference* pGeoReference = new IfcDB::GeometryReference();

                            if (pFace->getGmlId().empty() == true)
                            {
                                pFace->setGmlId(_T("GML_") + IfcDB::createUuid());
                            }

                            pGeoReference->setGmlReferenceId(pFace->getGmlId());
                            pGeoReference->setGeomReferenceID(pFace->getGeomID());
                            pGeoReference->setReferencedGeometry(pFace);

                            references.emplace_back(pGeoReference);
                        }
                    }
                }

                pCompositeSurface->setGeometries(references);
            }
        }
    }


    if (pBuilding->getGeometries().empty())
    {
        return state;
    }

    return state;
}

IfcDB::Feature* OsmBuildingCreator::createBoundarySurface(IfcDB::Feature* pBuilding, IfcDB::Face* pFace, const std::wstring& lod, bool createGeoReference/*= true*/)
{
    std::wstring surfaceType;

    IfcDB::Direction normal = pFace->getFaceNormal();

    if (normal.z < -0.99)       // ground plate, -10� -> -90�
    {
        surfaceType = _T("bldg:GroundSurface");
    }
    else if (normal.z > -0.0009 && normal.z < 0.0009)                         // wall surface, -10� -> 10�
    {
        surfaceType = _T("bldg:WallSurface");
    }
    else   // roof surface, 10� -> 90�
    {
        surfaceType = _T("bldg:RoofSurface");
    }

    IfcDB::Feature* pBoundarySurface = m_pDB->createFeature(_T(""), surfaceType, pBuilding->getOid(), true, pBuilding->getModelInfo());

    m_pDB->createObjRelation(pBuilding->getOid(), pBoundarySurface->getOid(), _T("bldg:boundedBy"), pBuilding->getModelInfo());

    IfcDB::MultiSurface* pMultiSurface = new IfcDB::MultiSurface();

    // create geometry reference
    if (createGeoReference == true)
    {
        pMultiSurface->addGeometry(pFace);
    }
    else
    {
        pMultiSurface->addGeometry(pFace->clone());
    }

    std::wstring representationType(L"bldg:" + lod + L"MultiSurface");

    pBoundarySurface->addGeometryItem(lod, representationType, pMultiSurface);
    pBoundarySurface->setState(IfcDB::STATE_INITIAL_GEOMETRY_TRANSFORMATION);

    return pBoundarySurface;
}

