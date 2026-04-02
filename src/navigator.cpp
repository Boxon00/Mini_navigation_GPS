#include "../include/navigator.h"
#include <stdexcept>

void Navigator::registerAlgorithm(std::unique_ptr<IPathfinder> algo) {
    algorithms_.push_back(std::move(algo));
}

RouteResult Navigator::run(int                algoIndex,
                           const RoadGraph&   graph,
                           const std::string& start,
                           const std::string& end) const
{
    if (algoIndex < 0 || algoIndex >= static_cast<int>(algorithms_.size()))
        throw std::out_of_range("Neispravan indeks algoritma.");

    // Polimorfni poziv — Navigator ne zna koji je algoritam unutra
    return algorithms_[algoIndex]->findRoute(graph, start, end);
}

std::vector<RouteResult> Navigator::runAll(const RoadGraph&   graph,
                                           const std::string& start,
                                           const std::string& end) const
{
    std::vector<RouteResult> results;
    results.reserve(algorithms_.size());
    for (const auto& algo : algorithms_)
        results.push_back(algo->findRoute(graph, start, end));
    return results;
}

std::vector<std::string> Navigator::algorithmNames() const {
    std::vector<std::string> names;
    names.reserve(algorithms_.size());
    for (const auto& algo : algorithms_)
        names.push_back(algo->name());
    return names;
}

int Navigator::algorithmCount() const {
    return static_cast<int>(algorithms_.size());
}
