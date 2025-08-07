#pragma once

struct BuildingParameter
{
    struct StatisticOSM
    {
        //          Roof shape  + height (h or level)
        // Type 1:      x               x
        // Type 2:      x            default
        // Type 3:   default            x
        // Type 4:   default         default

        int typeNr = 4;
        bool success = true;
    };

    struct Roof
    {
        std::wstring shape = L"flat";
        double height = 0;
        std::vector<IfcDB::Face> vFaces;
    };

//    struct BuildingPart
//    {
//        IfcDB::Profile* pProfile = nullptr;
//        IfcDB::Feature* pBuildingPartFeature = nullptr;
//        double height = 0;
//        double startHeight = 0;
//        Roof roof;
//        IfcDB::Faces vResultFaces;
//    };
    double BuildingHeight;
    double BuildingStartHeight = 0;
    Roof roof;
//    std::vector<BuildingPart> buildingPart;
    std::vector<IfcDB::Faces> vFacesBuilding;

    bool simple = false;
    bool levels = false;
    StatisticOSM statisticType;
};

//struct Colour
//{
//    double r, g, b, a;
//
//    Colour(double red, double green, double blue, double alpha)
//            : r(red), g(green), b(blue), a(alpha) {}
//};

//struct MaterialSettings
//{
//    double transparancy, shininess, intensity;
//    bool smooth;
//
//    MaterialSettings(double tr, double sh, double in, bool sm)
//            : transparancy(tr), shininess(sh), intensity(in), smooth(sm) {}
//};

struct OSMStatistics
{
    //          Roof shape  + height (h or level)
    // Type 1:      x               x
    // Type 2:      x            default
    // Type 3:   default            x
    // Type 4:   default         default

    int Type1_total = 0;
    int Type1_success = 0;

    int Type2_total = 0;
    int Type2_success = 0;

    int Type3_total = 0;
    int Type3_success = 0;

    int Type4_total = 0;
    int Type4_success = 0;

};

struct OsmParameter
{
  double DefaultBuildingHeight = 10.0;
  double DefaultStoreyHeight = 3;

  std::unordered_map<std::wstring, double> MapDefaultHeights = { {L"OSM_building",        10.0},
                                                                 {L"OSM_building_garage",  2.5},
                                                                 {L"garage",               2.5},
                                                                 {L"garages",              2.5},
                                                                 {L"hut",                  3.0},
                                                                 {L"roof",                 3.0},
                                                                 {L"OSM_building_hut",     3.0} };

  std::unordered_map<std::wstring, std::wstring> MapRoofShape2RoofTypeCodelist{ {L"gabled",   L"1030"},
                                                                                {L"skillion", L"1020"},
                                                                                {L"flat",     L"1000"},
                                                                                {L"hipped",   L"1040"} };

    std::unordered_map<std::wstring, std::wstring> MapRoofShape3drCodelist{ {L"0.0",   L"flat"},
                                                                            {L"1.0",   L"skillion"},
                                                                            {L"2.0",   L"gabled"},
                                                                            {L"2.3",   L"half-hipped"},
                                                                            {L"2.4",   L"hipped"},
                                                                            {L"2.5",   L"pyramidal"},
                                                                            {L"4.0",   L"gambrel"},
                                                                            {L"4.1",   L"mansard"},
                                                                            {L"4.2",   L"mansard"},
                                                                            {L"5.0",   L"round"},
                                                                            {L"5.6",   L"dome"},
                                                                            {L"6.2",   L"crosspitched"},
                                                                            {L"8.0",   L"cone"},
//                                                                            {L"2.5",   L"pyramidal"},
//                                                                            {L"2.5",   L"pyramidal"},
                                                                            {L"7.1",   L"sawtooth"} };

//  std::unordered_map<std::wstring, int> MapDirections = { {L"N",    360},
//                                                          {L"north",360},
//                                                          {L"NNE",   22},
//                                                          {L"NE",    45},
//                                                          {L"ENE",   65},
//                                                          {L"E",     90},
//                                                          {L"east",  90},
//                                                          {L"ESE",  112},
//                                                          {L"SE",   135},
//                                                          {L"SSE",  157},
//                                                          {L"S",    180},
//                                                          {L"south",180},
//                                                          {L"SSW",  202},
//                                                          {L"SW",   225},
//                                                          {L"WSW",  247},
//                                                          {L"W",    270},
//                                                          {L"west", 270},
//                                                          {L"WNW",  292},
//                                                          {L"NW",   315},
//                                                          {L"NNW",  337},
//                                                          {L"n",    360},
//                                                          {L"NORTH",360},
//                                                          {L"nne",   22},
//                                                          {L"ne",    45},
//                                                          {L"ene",   65},
//                                                          {L"e",     90},
//                                                          {L"EAST",  90},
//                                                          {L"ese",  112},
//                                                          {L"se",   135},
//                                                          {L"sse",  157},
//                                                          {L"s",    180},
//                                                          {L"SOUTH",180},
//                                                          {L"ssw",  202},
//                                                          {L"sw",   225},
//                                                          {L"wsw",  247},
//                                                          {L"w",    270},
//                                                          {L"WEST", 270},
//                                                          {L"wnw",  292},
//                                                          {L"nw",   315},
//                                                          {L"nnw",  337} };
};
