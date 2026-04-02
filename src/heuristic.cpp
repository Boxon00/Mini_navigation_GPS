#include "../include/heuristic.h"
#include <cmath>

double haversineKm(double lat1, double lon1,
                   double lat2, double lon2)
{
    constexpr double R  = 6371.0;
    constexpr double PI = 3.14159265358979323846;

    double dLat = (lat2 - lat1) * PI / 180.0;
    double dLon = (lon2 - lon1) * PI / 180.0;

    double a = std::sin(dLat / 2) * std::sin(dLat / 2)
             + std::cos(lat1 * PI / 180.0)
             * std::cos(lat2 * PI / 180.0)
             * std::sin(dLon / 2) * std::sin(dLon / 2);

    return R * 2.0 * std::atan2(std::sqrt(a), std::sqrt(1.0 - a));
}
