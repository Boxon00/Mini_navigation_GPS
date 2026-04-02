#pragma once

// ------------------------------------------------------------
//  Haversine heuristika
//
//  SOLID:
//    SRP — jedina odgovornost: racunanje vazdusne udaljenosti
//          izmedju dve geo-tacke
//
//  Koristi se kao h(n) u A* algoritmu.
//  Formula je admissible: vazdusna udaljenost <= drumska,
//  sto garantuje optimalnost A*.
// ------------------------------------------------------------

double haversineKm(double lat1, double lon1,
                   double lat2, double lon2);
