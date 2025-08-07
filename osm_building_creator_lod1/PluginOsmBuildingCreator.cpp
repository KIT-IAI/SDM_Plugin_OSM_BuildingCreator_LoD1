#include "PluginOsmBuildingCreator.h"
#include "OsmBuildingCreator.h"
#include "IFCDBInterfaceVersion.hpp"
#include "PluginInterfaceVersion.hpp"
#include "ComponentRegistry.hpp"

using namespace std;
using namespace sdm::plugin;

IMPLEMENT_PLUGIN(PluginOsmBuildingCreator);

OsmBuildingCreatorAction::OsmBuildingCreatorAction()
  : ActionFeatureHelper("OSM Building Creator LOD1")
{
    wxInitialize();
}

OsmBuildingCreatorAction::~OsmBuildingCreatorAction()
{
    wxUninitialize();
}

void OsmBuildingCreatorAction::execute() const
{
    OsmBuildingCreator osmBuildingCreator(m_pDB, m_pStates, m_pLogger, m_pProgressBar, m_pMainFrameInterface);
    osmBuildingCreator.build();

}

bool OsmBuildingCreatorAction::isActive() const
{
  if (m_pDB && !m_isDialogOpen && m_pDB->hasModelInfo(IfcDB::ModelInfo::MT_OSM))
  {
    return true;
  }

  return false;
}

PluginOsmBuildingCreator::PluginOsmBuildingCreator()
{
    m_mainframeObserver.attach([this](MainFrameInterface* pInterface){m_OsmBuildingCreatorAction.setInterface(pInterface);});
    m_liveLogObserver.attach([this](LiveLogInterface* pLogger) { m_OsmBuildingCreatorAction.setLogger(pLogger); });
  m_progressBarFeature.attach([this](ProgressBarInterface* pProgressBar) { m_OsmBuildingCreatorAction.setProgressBar(pProgressBar); });
  m_documentObserver.attach([this](IfcDB::Populationi* pDB) { m_OsmBuildingCreatorAction.setDB(pDB); IfcDB::assignGlobalStates(pDB); });
  m_documentObserver.attach([this](IfcDB::utils::PopulationSubject* pStates) { m_OsmBuildingCreatorAction.setStates(dynamic_cast<IfcDB::utils::PopulationStates*>(pStates)); });
}

Version PluginOsmBuildingCreator::getInterfaceVersion() const
{
  return Version();
}

PluginInfo PluginOsmBuildingCreator::getInfo() const
{
  PluginInfo info;
  info.name = "OSM Building Creator LOD1";
  info.description = "";
  info.version = { 1, 0 };

  return info;
}

std::vector<Feature*> PluginOsmBuildingCreator::getFeatures() const
{
  std::vector<Feature*> features;

  features.emplace_back(const_cast<OsmBuildingCreatorAction*>(&m_OsmBuildingCreatorAction));
  features.emplace_back(const_cast<MainFrameObserverImpl*>(&m_mainframeObserver));
  features.emplace_back(const_cast<LiveLogObserver*>(&m_liveLogObserver));
  features.emplace_back(const_cast<ProgressBarFeature*>(&m_progressBarFeature));
  features.emplace_back(const_cast<DocumentObserverImpl*>(&m_documentObserver));

  return features;
}

ComponentInfo PluginOsmBuildingCreator::getComponentInfo(const RequiredComponent& requiredComponent) const
{
  ComponentRegistry availableComponents;
  availableComponents.addAvailable(IFCDB_INTERFACE_COMPONENT_NAME, IFCDB_INTERFACE_COMPONENT_VERSION, IFCDB_INTERFACE_COMPONENT_HINT, std::atoi(IFCDB_INTERFACE_COMPONENT_DATE.data()));
  availableComponents.addAvailable(PLUGIN_INTERFACE_COMPONENT_NAME, PLUGIN_INTERFACE_COMPONENT_VERSION, PLUGIN_INTERFACE_COMPONENT_HINT, std::atoi(PLUGIN_INTERFACE_COMPONENT_DATE.data()));

  return availableComponents.getInfo(requiredComponent);
}

const sdm::plugin::InitializationState& PluginOsmBuildingCreator::getInitializationState() const
{
  return m_initState;
}

