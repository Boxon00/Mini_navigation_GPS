#include "../include/graph.h"

void RoadGraph::addCity(const std::string& name, double lat, double lon) {
    cities_[name] = {name, lat, lon};
    adjList_[name]; // inicijalizuj praznu listu susedstva
}

void RoadGraph::addRoad(const std::string& from,
                        const std::string& to,
                        int distance)
{
    adjList_[from].push_back({to,   distance});
    adjList_[to].push_back({from, distance});
}

const std::vector<Edge>& RoadGraph::neighbors(const std::string& city) const {
    return adjList_.at(city);
}

const City& RoadGraph::getCity(const std::string& name) const {
    return cities_.at(name);
}

std::vector<std::string> RoadGraph::allCities() const {
    std::vector<std::string> result;
    result.reserve(cities_.size());
    for (const auto& [k, _] : cities_)
        result.push_back(k);
    return result;
}

bool RoadGraph::hasCity(const std::string& name) const {
    return cities_.count(name) > 0;
}
