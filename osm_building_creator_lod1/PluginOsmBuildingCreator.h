#pragma once

#include <Plugin.hpp>
#include <ActionFeatureHelper.hpp>
#include <DocumentObserver.hpp>
#include <LiveLogFeature.hpp>
#include <ProgressbarFeature.hpp>
#include <LogDialogFeature.hpp>
#include <LogDialogFeatureHelper.hpp>
#include <ifcdb/IfcDbInclude.h>
#include <ifcdb/utils/PopulationObserver.hpp>
#include "ActionFeatureHelper.hpp"
#include <MainFrameObserverImpl.hpp>
#include <MessageDialogFeatureHelper.hpp>

class OsmBuildingCreatorAction : public sdm::plugin::ActionFeatureHelper
{
  public:
    OsmBuildingCreatorAction();
    ~OsmBuildingCreatorAction();

    void execute() const override;
    bool isActive() const override;

    void setLogger(sdm::plugin::LiveLogInterface* pLiveLogInterface) { m_pLogger = pLiveLogInterface; }
    void setProgressBar(sdm::plugin::ProgressBarInterface* pProgressBar) { m_pProgressBar = pProgressBar; }
    void setDB(IfcDB::Populationi* pDB) { m_pDB = pDB; }
    void setStates(IfcDB::utils::PopulationStates* pSates) { m_pStates = pSates; }
    void setInterface(sdm::plugin::MainFrameInterface* pInterface) { m_pMainFrameInterface = pInterface; }

    void setIsDialogOpen(bool state) { m_isDialogOpen = state; }

    sdm::plugin::LiveLogInterface* getLogger() { return m_pLogger; }
    IfcDB::Populationi* getDB() { return m_pDB; }
    IfcDB::utils::PopulationStates* getStates() { return m_pStates; }

protected:
    bool m_isDialogOpen = false;
    IfcDB::Populationi* m_pDB = nullptr;
    IfcDB::utils::PopulationStates* m_pStates = nullptr;
    sdm::plugin::LiveLogInterface* m_pLogger = nullptr;
    sdm::plugin::ProgressBarInterface* m_pProgressBar = nullptr;

    sdm::plugin::MainFrameInterface* m_pMainFrameInterface = nullptr;
};

class PluginOsmBuildingCreator : public sdm::plugin::Plugin
{
  public:
    PluginOsmBuildingCreator();
    ~PluginOsmBuildingCreator() override = default;

    sdm::plugin::Version getInterfaceVersion() const override;
    sdm::plugin::PluginInfo getInfo() const override;
    std::vector<sdm::plugin::Feature*> getFeatures() const override;

    sdm::plugin::ComponentInfo getComponentInfo(const sdm::plugin::RequiredComponent& requiredComponent) const override;
    const sdm::plugin::InitializationState& getInitializationState() const override;

  private:
    sdm::plugin::MainFrameObserverImpl m_mainframeObserver;
    sdm::plugin::DocumentObserverImpl m_documentObserver;
    sdm::plugin::LiveLogObserver m_liveLogObserver;
    sdm::plugin::ProgressBarFeature m_progressBarFeature;
    OsmBuildingCreatorAction m_OsmBuildingCreatorAction;
    sdm::plugin::InitializationState m_initState;
    IfcDB::Populationi* m_pDB = nullptr;
    IfcDB::utils::PopulationStates* m_pStates = nullptr;

};