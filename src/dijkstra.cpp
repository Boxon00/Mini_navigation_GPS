#include "../include/dijkstra.h"
#include <queue>
#include <unordered_map>
#include <limits>
#include <algorithm>

std::string DijkstraPathfinder::name() const {
    return "Dijkstra";
}

RouteResult DijkstraPathfinder::findRoute(const RoadGraph&   graph,
                                          const std::string& start,
                                          const std::string& end) const
{
    // Min-heap: {udaljenost, ime_grada}
    using PQItem = std::pair<int, std::string>;
    std::priority_queue<PQItem,
                        std::vector<PQItem>,
                        std::greater<PQItem>> pq;

    std::unordered_map<std::string, int>         dist;
    std::unordered_map<std::string, std::string> prev;
    int nodesVisited = 0;

    for (const auto& city : graph.allCities())
        dist[city] = std::numeric_limits<int>::max();

    dist[start] = 0;
    pq.push({0, start});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        if (d > dist[u]) continue; // zastareli unos
        ++nodesVisited;

        if (u == end) break;

        for (const Edge& e : graph.neighbors(u)) {
            int newDist = dist[u] + e.distance;
            if (newDist < dist[e.to]) {
                dist[e.to] = newDist;
                prev[e.to] = u;
                pq.push({newDist, e.to});
            }
        }
    }

    if (dist[end] == std::numeric_limits<int>::max())
        return {false, {}, 0, nodesVisited, name()};

    std::vector<std::string> path;
    for (std::string cur = end; !cur.empty();
         cur = prev.count(cur) ? prev[cur] : "")
        path.push_back(cur);
    std::reverse(path.begin(), path.end());

    return {true, path, dist[end], nodesVisited, name()};
}
