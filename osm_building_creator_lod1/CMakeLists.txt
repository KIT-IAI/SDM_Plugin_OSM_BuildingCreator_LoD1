# CMakeList.txt: CMake-Projekt für "ifcexplorer_papermodel". Schließen Sie die Quelle ein, und definieren Sie
# projektspezifische Logik hier.
#
cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(wxWidgets CONFIG REQUIRED)

add_library(osm_building_creator_lod1 SHARED
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

target_link_libraries(osm_building_creator_lod1 PRIVATE ifcexplorer_plugin stdutils ifcdb)
target_link_libraries(osm_building_creator_lod1 PRIVATE wx::core wx::base)

add_compile_definitions(NOMINMAX)
install(FILES OsmBuildingCreator.cpp OsmBuildingCreator.h PluginOsmBuildingCreator.cpp PluginOsmBuildingCreator.h cmakelists-template/CMakeLists.txt DESTINATION .)