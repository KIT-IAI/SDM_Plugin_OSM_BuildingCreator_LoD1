#pragma once

#include <ifcdb/IfcDbInclude.h>
#include <ifcdb/utils/PopulationObserver.hpp>
#include <MessageDialogFeature.hpp>
#include <MessageDialogFeatureHelper.hpp>
#include <unordered_map>

#include "OsmParameter.h"
#include <wx/wx.h>

namespace sdm
{
  namespace plugin
  {
    class LiveLogInterface;
    class ProgressBarInterface;
    class MainFrameInterface;
  }
}

class OsmBuildingCreator
{
  public:
    OsmBuildingCreator(IfcDB::Populationi* pDB, IfcDB::utils::PopulationStates* pStates, sdm::plugin::LiveLogInterface* pLogger, sdm::plugin::ProgressBarInterface* pProgressBar, sdm::plugin::MainFrameInterface* pInterface, IfcDB::MessageLogger* pMessageLogger = nullptr);
    ~OsmBuildingCreator() = default;

    std::vector<IfcDB::Feature*> buildingsMap;

    void build();

    OSMStatistics m_OSMstats;
    bool m_BoundarySurface = true;
    void countStats(int typeNr, bool success);
    void assignSatsType(BuildingParameter& buildingParameter, bool roofDefault, bool heightDefault);

  private:
    IfcDB::ifcOid createCityModel();
    IfcDB::ifcOid createBuilding(IfcDB::ifcOid oidCityModel, IfcDB::IfcEntity* pEntity, const std::wstring& lod);

    bool createBoundarySurfaces(IfcDB::Feature* pBuilding, const std::wstring& lod, bool createGeoReference = true);
    IfcDB::Feature* createBoundarySurface(IfcDB::Feature* pBuilding, IfcDB::Face* pFace, const std::wstring& lod, bool createGeoReference = true);

    IfcDB::ifcOid createAddress(IfcDB::ifcOid oidBuilding, IfcDB::IfcEntity* pEntity);
    bool createBuidlingSolid(IfcDB::IfcEntity* pOsmEntity, BuildingParameter& buildingParameter);

    IfcDB::Populationi* m_pDB = nullptr;
    IfcDB::utils::PopulationStates* m_pStates = nullptr;
    sdm::plugin::LiveLogInterface* m_pLogger = nullptr;
    sdm::plugin::ProgressBarInterface* m_pProgressBar = nullptr;
    sdm::plugin::MainFrameInterface* m_pMainFrameInterface = nullptr;
    IfcDB::MessageLogger* m_pMessageLogger = nullptr;
    OsmParameter m_OsmParameter;
    std::map<std::wstring, IfcDB::AppearanceColor*> m_mapOID2AppearanceColor;
    IfcDB::GeometrySettings m_geometrySettings;

  protected:
};
