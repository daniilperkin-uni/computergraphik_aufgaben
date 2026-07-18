#include "LineStrip3D.hpp"
#include <cmath>

// Fügt einen Punkt am Ende des Linienzuges hinzu
void LineStrip3D::addPoint(const Point3D& point) {
    points.push_back(point);
}

// Entfernt den letzten Punkt des Linienzuges (falls vorhanden)
void LineStrip3D::removePoint() {
    if (!points.empty()) {
        points.pop_back();
    }
}

// Berechnet die Gesamtlänge des Linienzuges
double LineStrip3D::computeLength() const {
    if (points.size() < 2) {
        return 0.0;
    }

    double totalLength = 0.0;
    // Iteriert über alle Paare von aufeinanderfolgenden Punkten
    for (size_t i = 0; i < points.size() - 1; ++i) {
        totalLength += points[i].distanceTo(points[i + 1]);
    }
    return totalLength;
}