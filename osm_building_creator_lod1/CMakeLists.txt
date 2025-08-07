<<<<<<< HEAD
﻿# CMakeList.txt: CMake-Projekt für "ifcexplorer_papermodel". Schließen Sie die Quelle ein, und definieren Sie
# projektspezifische Logik hier.
#
cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(wxWidgets CONFIG REQUIRED)

add_library(osm_building_creator SHARED
        PluginOsmBuildingCreator.cpp
        PluginOsmBuildingCreator.h
        OsmBuildingCreator.cpp
        OsmBuildingCreator.h
        OsmParameter.h
        RoofBuildingCreator.h
        RoofBuildingCreator.cpp
        GabledRoof.cpp
        GabledRoof.h
        SkillionRoof.cpp
        SkillionRoof.h
        hippedRoof.cpp
        hippedRoof.h
        PyramidalRoof.cpp
        PyramidalRoof.h
        RoofActions.cpp
        RoofActions.h
        DomeRoof.cpp
        DomeRoof.h
        GabledHeightMoved.cpp
        GabledHeightMoved.h
        RoundRoof.cpp
        RoundRoof.h
        ButterflyRoof.cpp
        ButterflyRoof.h
        CrosspitchedRoof.cpp
        CrosspitchedRoof.h
        HalfHipped.cpp
        HalfHipped.h
        MansardRoof.cpp
        MansardRoof.h
        RoofWithLines.cpp
        RoofWithLines.h
        SideHippedRoof.cpp
        SideHippedRoof.h
        GambrelRoof.cpp
        GambrelRoof.h
        OnionRoof.cpp
        OnionRoof.h
        SawtoothRoof.cpp
        SawtoothRoof.h
        StraightSkeleton.cpp
        StraightSkeleton.h
        OSMbcDlgImpl.cpp
        OSMbcDlgImpl.h
        OSMBldgDlg.h
        OSMBldgDlg.cpp)

find_package(CGAL REQUIRED)
target_link_libraries(osm_building_creator PRIVATE ifcexplorer_plugin stdutils ifcdb CGAL::CGAL)
target_link_libraries(osm_building_creator PRIVATE wx::core wx::base)

add_compile_definitions(NOMINMAX)
=======
﻿# CMakeList.txt: CMake-Projekt für "ifcexplorer_papermodel". Schließen Sie die Quelle ein, und definieren Sie
# projektspezifische Logik hier.
#
cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(wxWidgets CONFIG REQUIRED)

add_library(osm_building_creator-lod1 SHARED
        PluginOsmBuildingCreator.cpp
        PluginOsmBuildingCreator.h
        OsmBuildingCreator.cpp
        OsmBuildingCreator.h
        OsmParameter.h
        lodDlgImpl.cpp
        lodDlgImpl.h
        Lod1Dlg.cpp
        Lod1Dlg.h

        osm-building-creator.fbp
)

target_link_libraries(osm_building_creator-lod1 PRIVATE ifcexplorer_plugin stdutils ifcdb)
target_link_libraries(osm_building_creator-lod1 PRIVATE wx::core wx::base)

add_compile_definitions(NOMINMAX)
>>>>>>> c360937e53d589abe998e28874b455ebbec9af27
install(FILES OsmBuildingCreator.cpp OsmBuildingCreator.h PluginOsmBuildingCreator.cpp PluginOsmBuildingCreator.h cmakelists-template/CMakeLists.txt DESTINATION .)