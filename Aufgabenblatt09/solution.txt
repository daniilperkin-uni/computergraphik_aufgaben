# Aufgabenblatt 09 - Detaillierte Lösungen (2.x Shading)

Hier ist eine sehr ausführliche Aufschlüsselung aller **2.x Aufgaben (Shading & Lighting)**.

Wir befinden uns hier im Bereich der **Shader-Programmierung** (GLSL). Das Ziel ist es, von einer einfachen, flachen Darstellung zu einer realistischen Darstellung mit physikalisch anmutender Beleuchtung und Oberflächendetails (Normal Mapping) zu kommen.

---

## Aufgabe 2.1: Korrekte Normalen-Transformation

**Wo:** `code/shaders/vert_scene_objects.glsl` (Vertex Shader)

**Das Problem:**
Wir haben 3D-Modelle, die im Raum platziert werden (Translation, Rotation, Skalierung). Diese Infos stecken in der `model_matrix`.
Wenn wir die Position eines Vertex transformieren, nutzen wir einfach `model_matrix * position`.
Bei **Normalen** (Vektoren, die senkrecht auf der Oberfläche stehen) funktioniert das nicht,
wenn wir **ungleichmäßig skalieren** (z.B. eine Kugel zu einem flachen Pfannkuchen stauchen).
Würden wir die Normale auch stauchen, würde sie nicht mehr senkrecht auf der Oberfläche stehen.

**Die Lösung:**
Die Mathematik besagt: Um die Richtung korrekt zu erhalten, muss man die **Inverse Transponierte** der oberen linken 3x3-Teilmatrix der Model-Matrix verwenden.

**Die Lösung für die Klasse:**
Wir berechnen die `normal_matrix` als Inverse Transponierte der Model-Matrix, um sicherzustellen,
dass die Normalen auch bei verzerrter Skalierung (z.B. Stauchung) ihre senkrechte Ausrichtung zur Oberfläche behalten.

**Der Code:**

```glsl
// --- In void main() ---

// 1. Wir extrahieren die Rotations- und Skalierungsanteile (mat3) aus der 4x4 Model-Matrix.
// 2. Wir berechnen die Inverse dieser Matrix.
// 3. Wir transponieren das Ergebnis (Zeilen und Spalten vertauschen).
// Dies ist die mathematisch korrekte "Normal-Matrix".
mat3 normal_matrix = transpose(inverse(mat3(model_matrix)));

// Wir multiplizieren die ursprüngliche Normale (in_normal) mit dieser Matrix.
// Danach normalisieren wir das Ergebnis, da die Länge durch Skalierung verändert sein könnte,
// wir für Beleuchtung aber immer Vektoren der Länge 1 brauchen.
out_normal = normalize(normal_matrix * in_normal);
```

---

## Aufgabe 2.2: Berechnung der Blickrichtung (View Direction)

**Wo:** `code/shaders/frag_scene_objects.glsl` (Fragment Shader)

**Das Problem:**
Für das **Phong-Beleuchtungsmodell** (speziell für den spekularen Glanzpunkt) müssen wir wissen, in welchem Winkel wir auf die Oberfläche schauen.
Wir brauchen einen Vektor vom aktuellen Punkt auf dem Objekt (`world_pos`) zur Position der Kamera.

**Die Lösung:**
Wir haben die Kameraposition nicht direkt als Variable. Aber wir haben die `per_frame.view` Matrix.
Diese Matrix transformiert die ganze Welt so, dass die Kamera im Ursprung (0,0,0) steht.
Die **Inverse** dieser Matrix macht das Gegenteil: Sie bewegt den Ursprung an die Stelle, wo die Kamera in der Welt steht.
Die Position steht dann in der 4. Spalte (Index 3).

**Die Lösung für die Klasse:**
Um den Vektor zur Kamera (`view_direction`) zu erhalten, holen wir uns die Kameraposition aus der 4.
Spalte der inversen View-Matrix und ziehen davon die Fragment-Position ab.

**Der Code:**

```glsl
// --- In void main() ---

// Wir holen uns die Inverse der View-Matrix.
// Der Zugriff [3] gibt uns die 4. Spalte zurück (GLSL ist spaltenbasiert).
// .xyz extrahiert die X, Y, Z Koordinaten der Kamera im Weltraum.
vec3 cam_pos_world = inverse(per_frame.view)[3].xyz;

// Wir berechnen den Differenzvektor: Ziel (Kamera) minus Start (Fragment-Position).
// normalize() sorgt dafür, dass der Vektor die Länge 1 hat (reiner Richtungsvektor).
vec3 view_direction = normalize(cam_pos_world - world_pos);
```

---

## Aufgabe 2.3: Lokale Beleuchtung (Lighting Loop)

**Wo:** `code/shaders/frag_scene_objects.glsl` (Fragment Shader)

**Das Problem:**
Wir haben 4 Punktlichtquellen (`light_position[4]`). Wir müssen für jedes Pixel berechnen, wie viel Licht von jeder dieser Quellen ankommt und reflektiert wird. Dabei muss das Licht schwächer werden, je weiter die Lampe weg ist (physikalisches Gesetz: Intensität proportional zu 1 / Abstand^2).

**Die Lösung:**
Wir iterieren durch alle 4 Lichter, berechnen Richtung und Abstand, bestimmen die Dämpfung und rufen die Hilfsfunktion `phongLighting` auf.

**Die Lösung für die Klasse:**
Wir laufen in einer Schleife über alle 4 Lichtquellen, berechnen für jede die Distanz-Dämpfung ($1/d^2$) und addieren den jeweiligen Phong-Anteil zur Gesamthelligkeit.

**Der Code:**

```glsl
// --- In void main(), innerhalb der Schleife for (int i = 0; i < 4; ++i) ---

// 1. Vektor vom Fragment zur i-ten Lichtquelle berechnen (Ziel - Start)
vec3 light_diff = per_frame.light_position[i].xyz - world_pos;

// 2. Den Abstand (Länge des Vektors) berechnen. Wichtig für die Dämpfung.
float distance = length(light_diff);

// 3. Den Richtungsvektor zum Licht normalisieren (Länge auf 1 bringen).
// Die Phong-Funktion erwartet normalisierte Vektoren.
vec3 light_dir = normalize(light_diff);

// 4. Attenuation (Lichtdämpfung) berechnen.
// .a (Alpha-Kanal) enthält die Basis-Intensität der Lampe.
// Wir teilen durch das Quadrat des Abstands. Je weiter weg, desto dunkler.
float intensity = per_frame.light_color_intensity[i].a / (distance * distance);

// 5. Die Farbe des Lichts aus dem RGB-Kanal holen.
vec3 light_color = per_frame.light_color_intensity[i].rgb;

// 6. Phong-Beleuchtung berechnen:
// n              = Die Normale der Oberfläche (aus Aufgabe 2.5 oder interpoliert)
// light_dir      = Richtung zum Licht
// view_direction = Richtung zur Kamera (aus Aufgabe 2.2)
// k_d            = Diffuse Materialfarbe
// k_s            = Spekulare Materialfarbe (Glanz)
// shininess      = Wie eng/scharf der Glanzpunkt ist
//
// Das Ergebnis multiplizieren wir mit der Lichtfarbe und der berechneten Intensität.
// += addiert das Licht dieser Quelle zum gesamten reflektierten Licht.
reflected_light += phongLighting(n, light_dir, view_direction, k_d, k_s, shininess) * light_color * intensity;
```

---

## Aufgabe 2.4: Tangent-Space-Matrix (TBN) vorbereiten

Hier arbeiten Vertex- und Fragment-Shader zusammen.

**Ziel:**
Normal Maps speichern Vektoren in einem eigenen Koordinatensystem, das "auf der Haut" des Objekts klebt. Die Z-Achse zeigt aus der Haut raus (Normale), X und Y laufen entlang der Textur (Tangente und Bitangente). Wir müssen eine Matrix bauen, die Vektoren von diesem "Haut-System" (Tangent Space) in das "Welt-System" (World Space) umrechnet. Das ist die **TBN-Matrix** (Tangent-Bitangent-Normal).

### Teil A: Vertex Shader
**Wo:** `code/shaders/vert_scene_objects.glsl`

Wir müssen die Tangente (liegt als Vertex-Attribut vor) genau wie die Normale in den Weltraum drehen.

```glsl
// --- In void main() ---

// Die Tangente (in_tangent.xyz) muss mit der Normal-Matrix transformiert werden,
// damit sie korrekt zur rotierten Oberfläche passt.
out_tangent = normalize(normal_matrix * in_tangent.xyz);

// Die Bitangente (der dritte Vektor) wird oft nicht extra gespeichert, um Platz zu sparen.
// Stattdessen speichern wir ein Vorzeichen (-1 oder 1) in der W-Komponente der Tangente.
// Das brauchen wir, um gespiegelte Texturen korrekt darzustellen.
out_bitangent_sign = in_tangent.w;
```

### Teil B: Fragment Shader
**Wo:** `code/shaders/frag_scene_objects.glsl`

Hier bauen wir die Matrix zusammen. Da die Vektoren durch die Interpolation zwischen den Vertices ihre Länge verändert haben könnten, müssen wir sie erneut normalisieren.

**Die Lösung für die Klasse:**
Wir konstruieren im Fragment-Shader die TBN-Matrix (Tangent-Bitangent-Normal), indem wir die Bitangente per Kreuzprodukt rekonstruieren. Diese Matrix brauchen wir, um die Normal-Map-Daten korrekt in den Weltraum zu drehen.

**Der Code:**

```glsl
// --- In void main() ---

// 1. Die interpolierte Tangente normalisieren
vec3 T = normalize(tangent);

// 2. Die interpolierte Normale normalisieren
vec3 N = normalize(normal);

// 3. Die Bitangente berechnen.
// Sie steht senkrecht auf N und T. Das Kreuzprodukt (cross) liefert genau das.
// Wir multiplizieren mit dem Vorzeichen (bitangent_sign), damit sie in die richtige Richtung zeigt
// (wichtig für symmetrische Modelle, wo UV-Koordinaten gespiegelt sind).
vec3 B = normalize(cross(N, T)) * bitangent_sign;

// 4. Die TBN-Matrix konstruieren.
// Diese 3x3 Matrix transformiert einen Vektor vom Tangentenraum in den Weltraum.
mat3 TBN = mat3(T, B, N);
```

---

## Aufgabe 2.5: Normal Mapping anwenden

**Wo:** `code/shaders/frag_scene_objects.glsl` (Fragment Shader)

**Das Problem:**
Die Variable `n` ist bisher nur die "langweilige" Normale der Geometrie (z.B. flach auf einem Dreieck). Wir wollen sie durch die detaillierte, unruhige Normale aus der blauen Textur (`normal_tx2D`) ersetzen.

**Die Lösung:**
1. Texturwert lesen (0 bis 1).
2. Umrechnen in Vektor (-1 bis 1).
3. Mit TBN-Matrix in den Weltraum drehen.
4. `n` überschreiben.

**Die Lösung für die Klasse:**
Wir nehmen den Farbwert aus der Normal-Map, rechnen ihn von $[0, 1]$ in den Vektor-Bereich $[-1, 1]$ um und transformieren ihn mit der TBN-Matrix, damit das Licht feine Details "sieht", die geometrisch gar nicht da sind.

**Der Code:**

```glsl
// --- In void main(), VOR der Lighting-Schleife ---

// 1. & 2. Remapping:
// Die Textur liefert Farben im Bereich [0.0, 1.0].
// Eine Normale kann aber in negative Richtung zeigen [-1.0, 1.0].
// Formel: Wert * 2.0 - 1.0
// Beispiel: Textur 0.5 -> 1.0 - 1.0 = 0.0 (Mitte)
//           Textur 0.0 -> 0.0 - 1.0 = -1.0
vec3 n_tangent = normal_rgba.rgb * 2.0 - 1.0;

// 3. Transformation:
// Wir nehmen den Vektor aus der Textur (n_tangent) und multiplizieren ihn mit der TBN-Matrix.
// Das Ergebnis ist eine Normale, die in den Weltraum zeigt, aber die "Huckel" der Textur hat.
n = normalize(TBN * n_tangent);

// Diese Variable 'n' wird jetzt gleich in Aufgabe 2.3 (siehe oben) verwendet!
```