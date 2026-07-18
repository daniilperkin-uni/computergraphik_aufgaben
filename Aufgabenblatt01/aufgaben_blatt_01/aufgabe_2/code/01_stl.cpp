
#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include "LineStrip3D.hpp"

int main() {
    /*
        Begründung: std::vector wird verwendet, da die Anzahl der Linienzüge
        nicht von vornherein feststeht. Es ist die Standardwahl für dynamische
        Listen, insbesondere wenn Objekte hinzugefügt, aber nicht entfernt werden.
    */
    std::vector<LineStrip3D> linienzuege;

    // 2. Erstellen und Hinzufügen mehrerer Linienzüge
    LineStrip3D strip1;
    strip1.addPoint(Point3D(0, 0, 0));
    strip1.addPoint(Point3D(1, 0, 0));
    strip1.addPoint(Point3D(2, 0, 0));
    linienzuege.push_back(strip1);

    LineStrip3D strip2;
    strip2.addPoint(Point3D(0, 0, 0));
    strip2.addPoint(Point3D(0, 3, 0));
    linienzuege.push_back(strip2);

    LineStrip3D strip3;
    strip3.addPoint(Point3D(0, 0, 0));
    strip3.addPoint(Point3D(1, 1, 0));
    linienzuege.push_back(strip3);

    // Ausgabe der Längen vor dem Sortieren
    std::cout << "Unsortierte Längen der Linienzüge:" << std::endl;
    for (size_t i = 0; i < linienzuege.size(); ++i) {
        std::cout << "Linienzug " << i << ": " << linienzuege[i].computeLength() << std::endl;
    }
    std::cout << "------------------------------------" << std::endl;

    // 3. Sortieren des Containers nach der Länge der Linienzüge
    std::sort(linienzuege.begin(), linienzuege.end(),
        [](const LineStrip3D& a, const LineStrip3D& b) {
            return a.computeLength() < b.computeLength();
        }
    );

    // Ausgabe der Längen nach dem Sortieren zur Überprüfung
    std::cout << "Sortierte Längen der Linienzüge:" << std::endl;

    for (size_t i = 0; i < linienzuege.size(); ++i) {
        std::cout << "Linienzug " << i << ": " << linienzuege[i].computeLength() << std::endl;
    }

    return 0;
}