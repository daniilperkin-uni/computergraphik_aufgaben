
#ifndef POINT3D_HPP
#define POINT3D_HPP

#include <array> // Für std::array
#include <cmath> // Für std::sqrt, std::pow

struct Point3D {
    /*  Ich nutze Container für die drei Koordinaten.
        Begründung: std::array wird verwendet, da die Anzahl der Koordinaten (3)
        zur Compilezeit feststeht und sich nicht ändert.
    */

    std::array<double, 3> coordinates;

    // Constructor zur Initialisierung der Koordinaten
    Point3D(double x, double y, double z) {
        coordinates[0] = x;
        coordinates[1] = y;
        coordinates[2] = z;
    }

    // Getter-Funktionen für die Koordinaten
    double getX() const { return coordinates[0]; }
    double getY() const { return coordinates[1]; }
    double getZ() const { return coordinates[2]; }

    // Berechnet die euklidische Distanz zu einem anderen Punkt
    double distanceTo(const Point3D& other) const {
        return std::sqrt(
            std::pow(coordinates[0] - other.coordinates[0], 2) +
            std::pow(coordinates[1] - other.coordinates[1], 2) +
            std::pow(coordinates[2] - other.coordinates[2], 2)
        );
    }
};

#endif