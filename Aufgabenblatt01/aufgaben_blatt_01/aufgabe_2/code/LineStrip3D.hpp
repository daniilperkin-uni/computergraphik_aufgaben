// LineStrip3D.hpp

#ifndef LINESTRIP3D_HPP
#define LINESTRIP3D_HPP

#include <vector> // Für std::vector
#include "Point3D.hpp" // Da LineStrip3D Point3D verwendet

class LineStrip3D {

    /*  Begründung: std::vector wird verwendet,
        da die Anzahl der Punkte variabel ist
        und zur Laufzeit wachsen kann.
        Es ist effizient beim Hinzufügen am Ende und bietet gute Speicherlokalität.
    */
    std::vector<Point3D> points;

public:

    void addPoint(const Point3D& point);
    void removePoint();
    double computeLength() const;
};

#endif