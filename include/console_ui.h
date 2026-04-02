#pragma once

// ------------------------------------------------------------
//  ConsoleUI — konzolni korisnicki interfejs
//
//  SOLID:
//    SRP — jedina odgovornost: sve sto se tice ispisa i unosa
//    DIP — prima Navigator& i RoadGraph& kroz konstruktor;
//          ne kreira objekte sam
//
//  OOP:
//    Klasa enkapsuluje sav UI kod koji je ranije bio u
//    slobodnim funkcijama. Metode su logicki grupisane.
// ------------------------------------------------------------

#include "navigator.h"
#include "graph.h"

class ConsoleUI {
public:
    // Konstruktor prima zavisnosti spolja (DI)
    ConsoleUI(const Navigator& navigator, const RoadGraph& graph);

    // Pokrece glavnu petlju programa
    void run();

private:
    const Navigator&  navigator_;
    const RoadGraph&  graph_;

    void printMenu()                          const;
    void printResult(const RouteResult& res)  const;
    void printComparison(const std::vector<RouteResult>& results) const;
    void printGraph()                         const;
    void printSeparator()                     const;

    // Trazi unos dva grada od korisnika; vraca false ako su neispravni
    bool readCities(std::string& start, std::string& end) const;
};
