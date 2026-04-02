#include "../include/map_builder.h"

RoadGraph SerbiaMapBuilder::build() const {
    RoadGraph g;

    // --- Gradovi (ime, lat, lon) ---
    g.addCity("Beograd",    44.82, 20.46);
    g.addCity("Novi Sad",   45.26, 19.83);
    g.addCity("Nis",        43.32, 21.90);
    g.addCity("Kragujevac", 44.01, 20.92);
    g.addCity("Subotica",   46.10, 19.67);
    g.addCity("Zrenjanin",  45.38, 20.39);
    g.addCity("Pancevo",    44.87, 20.64);
    g.addCity("Cacak",      43.89, 20.35);
    g.addCity("Leskovac",   42.99, 21.95);
    g.addCity("Uzice",      43.86, 19.85);
    g.addCity("Vranje",     42.55, 21.90);
    g.addCity("Kraljevo",   43.72, 20.69);

    // --- Putevi (od, do, km) ---
    g.addRoad("Beograd",    "Novi Sad",    86);
    g.addRoad("Beograd",    "Pancevo",     14);
    g.addRoad("Beograd",    "Zrenjanin",   70);
    g.addRoad("Beograd",    "Kragujevac", 140);
    g.addRoad("Beograd",    "Nis",        240);
    g.addRoad("Novi Sad",   "Subotica",   100);
    g.addRoad("Novi Sad",   "Zrenjanin",   55);
    g.addRoad("Subotica",   "Zrenjanin",   95);
    g.addRoad("Pancevo",    "Zrenjanin",   60);
    g.addRoad("Kragujevac", "Nis",        135);
    g.addRoad("Kragujevac", "Cacak",       80);
    g.addRoad("Kragujevac", "Kraljevo",    90);
    g.addRoad("Cacak",      "Kraljevo",    50);
    g.addRoad("Cacak",      "Uzice",       70);
    g.addRoad("Kraljevo",   "Nis",        130);
    g.addRoad("Kraljevo",   "Uzice",      110);
    g.addRoad("Nis",        "Leskovac",    45);
    g.addRoad("Nis",        "Vranje",     125);
    g.addRoad("Leskovac",   "Vranje",      55);

    return g;
}
