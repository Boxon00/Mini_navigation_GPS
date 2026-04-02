// ============================================================
//  main.cpp — Composition Root
//
//  Jedino mesto gde se kreiraju konkretni objekti i
//  ubrizgavaju medjusobne zavisnosti (Dependency Injection).
//
//  Ostatak programa ne zna koje su konkretne klase — sve
//  komunicira kroz interfejse (IPathfinder, IMapBuilder).
//
//  Da bi se dodao novi algoritam:
//    1. Napravi novu klasu koja nasledjuje IPathfinder
//    2. Dodaj jedan red ovde: navigator.registerAlgorithm(...)
//    3. Nista drugo se ne menja.
// ============================================================

#include "include/navigator.h"
#include "include/map_builder.h"
#include "include/console_ui.h"
#include "include/dijkstra.h"
#include "include/astar.h"
#include <memory>

int main() {
    // 1. Izgradnja mape kroz apstrakciju IMapBuilder (DIP)
    std::unique_ptr<IMapBuilder> builder =
        std::make_unique<SerbiaMapBuilder>();
    RoadGraph graph = builder->build();

    // 2. Registracija algoritama kroz apstrakciju IPathfinder (DIP)
    //    Dodaj make_unique<TvojAlgoritam>() ovde za novi algoritam
    Navigator navigator;
    navigator.registerAlgorithm(std::make_unique<DijkstraPathfinder>());
    navigator.registerAlgorithm(std::make_unique<AStarPathfinder>());

    // 3. UI prima zavisnosti kroz konstruktor, ne kreira ih sam (DI)
    ConsoleUI ui(navigator, graph);
    ui.run();

    return 0;
}
