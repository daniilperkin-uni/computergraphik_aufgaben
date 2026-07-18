/*
Ursache des Problems: Definition in einem Header.
Der Fehler tritt auf, weil die Funktion sum in der Datei Definition.hpp definiert wurde.
Der Präprozessor verarbeitet die #include-Direktiven.
Wenn der Compiler source1.cpp kompiliert, wird der Inhalt von Definition.hpp direkt hineinkopiert.
Das Gleiche passiert bei source2.cpp.
Compiler: Nach dem Präprozessor sieht der Compiler zwei Dateien, die beide die Definition von sum enthalten:
source1.cpp wird zu source1.obj kompiliert.
source2.cpp wird zu source2.obj kompiliert.
Linker: Der Linker nimmt alle .obj-Dateien und versucht, sie zu einer einzigen .exe-Datei zu verbinden.
Er findet das Symbol sum(int, int) in source1.obj und dann erneut in source2.obj.
Dies verstößt gegen die One Definition Rule und führt zum Fehler multiple definition of 'sum(int, int)'.
Eine Header-Datei soll eine Schnittstelle (Deklaration) bereitstellen, nicht die Implementierung (Definition).
*/

// 01_functions.cpp
#include "source1.hpp"
#include "source2.hpp"
#include <iostream>
using namespace std;

int main() {

    cout << a() << endl;
    cout << b() << endl;

}