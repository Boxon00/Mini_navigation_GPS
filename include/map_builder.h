#pragma once

// ------------------------------------------------------------
//  IMapBuilder — apstraktni interfejs za gradnju mapa
//
//  SOLID:
//    OCP — novi skup gradova = nova klasa koja nasledjuje
//          IMapBuilder; Navigator i main.cpp se ne menjaju
//    DIP — main.cpp zavisi od apstrakcije IMapBuilder,
//          ne od konkretne SerbiaMapBuilder klase
//
//  OOP:
//    Apstraktna bazna klasa (interfejs) sa cistom virtualnom
//    metodom build() i virtualnim destruktorom.
// ------------------------------------------------------------

#include "graph.h"

class IMapBuilder {
public:
    virtual RoadGraph build() const = 0;
    virtual ~IMapBuilder()          = default;
};

// ------------------------------------------------------------
//  SerbiaMapBuilder — konkretna implementacija za Srbiju
//
//  12 gradova | 19 dvosmerna puta | realne udaljenosti (km)
// ------------------------------------------------------------
class SerbiaMapBuilder : public IMapBuilder {
public:
    RoadGraph build() const override;
};
