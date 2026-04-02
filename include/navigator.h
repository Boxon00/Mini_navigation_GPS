#pragma once

// ------------------------------------------------------------
//  Navigator — visoki nivo; orkestrira pretragu ruta
//
//  SOLID:
//    SRP — jedina odgovornost: pokretanje pretrage i uporedjivanje
//    DIP — zavisi od IPathfinder apstrakcije, ne od konkretnih
//          klasa; prima algoritme izvana (Dependency Injection)
//    OCP — da bi se dodao novi algoritam, Navigator se NE menja
//
//  OOP:
//    Cuva kolekciju IPathfinder* polimorfno.
//    Koristi unique_ptr za RAII upravljanje zivotnim vekom.
// ------------------------------------------------------------

#include "ipathfinder.h"
#include "graph.h"
#include <memory>
#include <string>
#include <vector>

class Navigator {
public:
    // Dependency Injection: algoritam se ubrizgava spolja
    void registerAlgorithm(std::unique_ptr<IPathfinder> algo);

    // Pokrece algoritam po indeksu (0-based)
    RouteResult run(int algoIndex,
                    const RoadGraph&   graph,
                    const std::string& start,
                    const std::string& end) const;

    // Pokrece sve registrovane algoritme i vraca rezultate
    std::vector<RouteResult> runAll(const RoadGraph&   graph,
                                    const std::string& start,
                                    const std::string& end) const;

    // Vraca listu naziva registrovanih algoritama
    std::vector<std::string> algorithmNames() const;

    int algorithmCount() const;

private:
    std::vector<std::unique_ptr<IPathfinder>> algorithms_;
};
