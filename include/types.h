#pragma once

#include <string>
#include <vector>

// ------------------------------------------------------------
//  City — cvor grafa sa geografskim koordinatama
// ------------------------------------------------------------
struct City {
    std::string name;
    double lat;   // geografska sirina  (koristi A* heuristika)
    double lon;   // geografska duzina  (koristi A* heuristika)
};

// ------------------------------------------------------------
//  Edge — ivica grafa (put do susednog grada)
// ------------------------------------------------------------
struct Edge {
    std::string to;
    int distance;   // km
};

// ------------------------------------------------------------
//  RouteResult — rezultat pretrage rute
// ------------------------------------------------------------
struct RouteResult {
    bool                     found;
    std::vector<std::string> path;
    int                      totalDistance;   // km
    int                      nodesVisited;    // pregledanih cvorova
    std::string              algorithmName;
};
