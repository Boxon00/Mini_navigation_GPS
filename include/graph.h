#pragma once

// ------------------------------------------------------------
//  RoadGraph — graf puteva (lista susedstva)
//
//  SOLID:
//    SRP — jedina odgovornost: cuvanje i pristup podacima grafa
//    OCP — otvorena za prosirenje (addCity/addRoad), zatvorena
//          za izmene jezgra
// ------------------------------------------------------------

#include "types.h"
#include <map>
#include <string>
#include <vector>

class RoadGraph {
public:
    void addCity(const std::string& name, double lat, double lon);
    void addRoad(const std::string& from, const std::string& to, int distance);

    const std::vector<Edge>& neighbors(const std::string& city) const;
    const City&              getCity(const std::string& name)   const;
    std::vector<std::string> allCities()                        const;
    bool                     hasCity(const std::string& name)   const;

private:
    std::map<std::string, City>              cities_;
    std::map<std::string, std::vector<Edge>> adjList_;
};
