AUFGABE 1.5

## (a) Z-Buffer

**Wofür wird der Z-Buffer gebraucht?**
Der Z-Buffer (Tiefenpuffer) dient der Lösung des **Sichtbarkeitsproblems**. Er speichert für jedes Pixel auf dem Bildschirm den Tiefenwert ($z$-Wert) des Objekts, das der Kamera am nächsten liegt.
* Vor dem Setzen eines neuen Pixels wird dessen Tiefe mit dem Wert im Buffer verglichen.
* Das Pixel wird nur überschrieben, wenn das neue Fragment näher an der Kamera liegt als das bisher gespeicherte.
* Dadurch werden Verdeckungen korrekt dargestellt, unabhängig von der Reihenfolge, in der die Dreiecke gezeichnet werden.

**Problematik beim parallelen Rendern (z.B. GPU):**
Beim parallelen Rendern entsteht eine **Race Condition** (Wettlaufsituation).
* Wenn mehrere Threads (oder GPU-Kerne) gleichzeitig Dreiecke rendern, die dasselbe Pixel überlappen, lesen sie möglicherweise gleichzeitig den aktuellen Z-Wert aus ("Read-Modify-Write"-Konflikt).
* Ohne Synchronisation (z.B. durch atomare Operationen oder Locks) kann ein Thread, der ein weiter entferntes Objekt rendert, fälschlicherweise das Ergebnis eines Threads überschreiben, der ein näheres Objekt rendert.
* Dies führt zu visuellen Fehlern wie Flimmern (Z-Fighting) oder falschen Verdeckungen.

-----------------------------------------------------------------------------------------------------------------------------------------------------

## (b) Clipping

**Verwendete Ansätze für verschiedene Modi:**
* **Points:** Meist einfaches Culling (Verwerfen des Punktes, wenn er nicht im Frustum liegt).
* **Wireframe & Filled:** In einfachen Implementierungen wird oft nur **Screen-Space Clipping** (Scissoring) verwendet. Dabei werden die Primitive projiziert und erst beim Setzen der Pixel (`setPixel`) wird geprüft, ob die $x, y$-Koordinaten innerhalb der Bildgrenzen liegen.

**Warum sind diese Implementierungen unzureichend?**
Ein reines "Abschneiden" im Bildraum reicht nicht aus, sobald Objekte die **Near-Plane** (die Ebene direkt vor der Kamera) schneiden oder sich **hinter der Kamera** befinden.

1.  **Perspektivische Division:**
    Um von 3D-Koordinaten auf 2D-Bildschirmkoordinaten zu kommen, wird durch die Tiefe $z$ (bzw. $w$) geteilt.
    * Liegt ein Punkt auf der Kameraebene ($z=0$), entsteht eine **Division durch Null**.
    * Liegt ein Punkt hinter der Kamera ($z < 0$), wird er mathematisch oft fälschlicherweise wieder in den sichtbaren Bereich projiziert (z.B. "auf dem Kopf stehend").

2.  **Geometrisches Clipping (Notwendigkeit):**
    * **Wireframe:** Eine Linie, die von "vor der Kamera" nach "hinter die Kamera" verläuft, geht durch die Singularität der Projektion. Sie muss *vor* der Projektion geometrisch gekürzt werden (z.B. mittels Cohen-Sutherland-Algorithmus), da sie sonst im projizierten Raum ins Unendliche liefe.
    * **Filled:** Ein Dreieck, das teilweise hinter der Kamera liegt, kann nicht einfach projiziert werden. Es muss an der Near-Plane zerschnitten werden (Clipping im Objektraum, z.B. mittels Sutherland-Hodgman), wodurch neue Eckpunkte und Kanten entstehen. Ohne diesen Schritt entstehen massive Artefakte oder Programmabstürze.