#include "../include/astar.h"
#include "../include/heuristic.h"
#include <queue>
#include <unordered_map>
#include <limits>
#include <algorithm>

std::string AStarPathfinder::name() const {
    return "A* (A-star)";
}

RouteResult AStarPathfinder::findRoute(const RoadGraph&   graph,
                                       const std::string& start,
                                       const std::string& end) const
{
    using PQItem = std::pair<double, std::string>;
    std::priority_queue<PQItem,
                        std::vector<PQItem>,
                        std::greater<PQItem>> open;

    std::unordered_map<std::string, double>      gScore;
    std::unordered_map<std::string, std::string> prev;
    std::unordered_map<std::string, bool>        visited;
    int nodesVisited = 0;

    const double INF    = std::numeric_limits<double>::infinity();
    const City&  endCity = graph.getCity(end);

    for (const auto& city : graph.allCities())
        gScore[city] = INF;

    gScore[start] = 0.0;
    double h0 = haversineKm(graph.getCity(start).lat,
                            graph.getCity(start).lon,
                            endCity.lat, endCity.lon);
    open.push({h0, start});

    while (!open.empty()) {
        auto [f, u] = open.top();
        open.pop();

        if (visited[u]) continue;
        visited[u] = true;
        ++nodesVisited;

        if (u == end) break;

        for (const Edge& e : graph.neighbors(u)) {
            if (visited[e.to]) continue;

            double tentG = gScore[u] + e.distance;
            if (tentG < gScore[e.to]) {
                prev[e.to]   = u;
                gScore[e.to] = tentG;
                double h = haversineKm(graph.getCity(e.to).lat,
                                       graph.getCity(e.to).lon,
                                       endCity.lat, endCity.lon);
                open.push({tentG + h, e.to});
            }
        }
    }

    if (gScore[end] == INF)
        return {false, {}, 0, nodesVisited, name()};

    std::vector<std::string> path;
    for (std::string cur = end; !cur.empty();
         cur = prev.count(cur) ? prev[cur] : "")
        path.push_back(cur);
    std::reverse(path.begin(), path.end());

    return {true, path,
            static_cast<int>(gScore[end]),
            nodesVisited, name()};
}
