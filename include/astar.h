#pragma once

// ------------------------------------------------------------
//  AStarPathfinder — konkretna implementacija IPathfinder
//
//  SOLID:
//    SRP — jedina odgovornost: A* algoritam sa Haversine h(n)
//    OCP — prosiruje sistem bez menjanja postojeceg koda
//    LSP — moze zameniti IPathfinder* bilo gde u programu
//
//  OOP:
//    Nasledjuje IPathfinder, override-uje findRoute() i name()
// ------------------------------------------------------------

#include "ipathfinder.h"

class AStarPathfinder : public IPathfinder {
public:
    RouteResult findRoute(const RoadGraph&   graph,
                          const std::string& start,
                          const std::string& end) const override;

    std::string name() const override;
};
