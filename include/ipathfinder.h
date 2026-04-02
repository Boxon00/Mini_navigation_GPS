#pragma once

// ------------------------------------------------------------
//  IPathfinder — apstraktni interfejs za algoritme pretrage
//
//  SOLID:
//    OCP — novi algoritam = nova klasa koja nasledjuje IPathfinder,
//          main.cpp se NE menja
//    DIP — visoki nivo (Navigator) zavisi od ove apstrakcije,
//          ne od konkretnih klasa (Dijkstra, AStar)
//    ISP — interfejs je minimalan: samo ono sto klijent treba
//    LSP — svaka konkretna klasa moze da zameni IPathfinder*
//          bez promene ponasanja programa
//
//  OOP:
//    Apstraktna klasa sa cistom virtualnom metodom.
//    Virtualni destruktor obezbedjuje ispravno brisanje
//    kroz pokazivac na baznu klasu.
// ------------------------------------------------------------

#include "types.h"
#include "graph.h"
#include <string>

class IPathfinder {
public:
    // Pronalazi najkraci put od start do end u datom grafu
    virtual RouteResult findRoute(const RoadGraph&    graph,
                                  const std::string&  start,
                                  const std::string&  end) const = 0;

    // Vraca naziv algoritma (za prikaz u UI-u)
    virtual std::string name() const = 0;

    // Virtualni destruktor — obavezno za apstraktne bazne klase
    virtual ~IPathfinder() = default;
};
