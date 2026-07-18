# Detaillierte Lösungen zu Aufgabenblatt 09 - Scene Shading

Dieses Dokument beschreibt die implementierten Lösungen für das 9. Übungsblatt. Es deckt den Aufbau des Szenengraphen, das Laden von Vertex-Daten sowie die Implementierung eines vollständigen Phong-Lighting-Modells inklusive Normal Mapping ab.

---

## Aufgabe 1: Transformationshierarchie und Vertex-Daten

In dieser Aufgabe ging es darum, die Datenstrukturen für die Szene aufzubauen, damit OpenGL die Objekte korrekt anordnen und zeichnen kann.

### 1.1 Szenehierarchie und Transformationsmatrizen (`main.cpp`)

**Ziel:** Aufbau einer Baumstruktur für die Szenenobjekte, damit sich Kinder relativ zu ihren Eltern bewegen (z.B. Räder an einem Auto). Zusätzlich muss die lokale Transformation in eine globale Welt-Matrix umgerechnet werden.

**Implementierung:**
Wir nutzen eine index-basierte verkettete Liste innerhalb eines flachen Arrays (`std::vector`).
1.  **Verkettung:**
    *   `parent`: Zeigt auf den Elternknoten.
    *   `first_child`: Zeigt auf das erste Kind eines Knotens.
    *   `next_sibling`: Zeigt auf das nächste Geschwisterkind (für Listen von Kindern).
2.  **Welt-Matrix:**
    *   Berechnung der lokalen Matrix aus Position (Translation), Orientierung (Rotation) und Skalierung.
    *   Falls ein Elternteil existiert: `WeltMatrix = ElternWeltMatrix * LokaleMatrix`.

**Code-Ausschnitt (`addGltfNode`):**
```cpp
// Setze den Elternknoten
scene.transform_data[transform_idx].parent = parent_transform_idx;

// Verkettung in die Kinder-Liste des Elternknotens
if (parent_transform_idx != NONE_INDEX) {
    auto& parent_transform = scene.transform_data[parent_transform_idx];

    if (parent_transform.first_child == NONE_INDEX) {
        // Erstes Kind
        parent_transform.first_child = transform_idx;
    } else {
        // An das Ende der Geschwister-Liste anhängen
        auto sibling_idx = parent_transform.first_child;
        while (scene.transform_data[sibling_idx].next_sibling != NONE_INDEX) {
            sibling_idx = scene.transform_data[sibling_idx].next_sibling;
        }
        scene.transform_data[sibling_idx].next_sibling = transform_idx;
    }
}

// Berechnung der Welt-Transformation
glm::mat4 local_transform = glm::translate(glm::mat4(1.0f), scene.transform_data[transform_idx].position) *
                            glm::toMat4(scene.transform_data[transform_idx].orientation) *
                            glm::scale(glm::mat4(1.0f), scene.transform_data[transform_idx].scale);

if (parent_transform_idx != NONE_INDEX) {
    scene.transform_data[transform_idx].world_transform = scene.transform_data[parent_transform_idx].world_transform * local_transform;
} else {
    scene.transform_data[transform_idx].world_transform = local_transform;
}
```

### 1.2 OpenGL Vertex Data (`main.cpp`)

**Ziel:** Hochladen der Geometriedaten (Positionen, Normalen, UVs, Tangenten) auf die Grafikkarte.

**Implementierung:**
Da die Daten "non-interleaved" vorliegen (separate Arrays für jedes Attribut), erstellen wir für jedes Attribut einen eigenen Vertex Buffer (VBO). Wichtig ist hier das korrekte Mapping der semantischen Namen (aus der glTF-Datei) auf die Shader-Locations (definiert im Vertex Shader).

**Code-Ausschnitt (`CreateMesh`):**
```cpp
for (const auto& desc : vertices) {
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, desc.byte_size, desc.data, GL_STATIC_DRAW);

    // Mapping basierend auf Shader-Layout (vert_scene_objects.glsl)
    GLuint location = 0;
    bool valid_attribute = false;

    if (desc.semantic_name == "NORMAL") { location = 0; valid_attribute = true; }
    else if (desc.semantic_name == "POSITION") { location = 1; valid_attribute = true; }
    else if (desc.semantic_name == "TANGENT") { location = 2; valid_attribute = true; }
    else if (desc.semantic_name == "TEXCOORD_0") { location = 3; valid_attribute = true; }

    if (valid_attribute) {
        glEnableVertexAttribArray(location);
        glVertexAttribPointer(location, desc.size, desc.type, desc.normalized, desc.stride, (void*)desc.offset);
    }
}
```

---

## Aufgabe 2: Shading

Hier wurde das visuelle Erscheinungsbild durch Beleuchtung und Oberflächendetails realisiert.

### 2.1 Normal-Matrix (`vert_scene_objects.glsl`)

**Ziel:** Korrekte Transformation der Normalen, auch wenn Objekte ungleichmäßig skaliert sind.

**Erklärung:** Normale Vektoren dürfen nicht einfach mit der Model-Matrix multipliziert werden,
da dies bei Skalierung die Winkel verfälscht. Die korrekte Matrix ist die Transponierte der Inversen der Model-Matrix.

**Code:**
```glsl
// Vertex Shader
mat3 normal_matrix = transpose(inverse(mat3(model_matrix)));
out_normal = normalize(normal_matrix * in_normal);
```

### 2.2 View-Direction (`frag_scene_objects.glsl`)

**Ziel:** Bestimmung der Blickrichtung für glanzartige Reflexionen (Specular Highlights).

**Erklärung:** Wir benötigen den Vektor vom Fragment zur Kamera. Die Kameraposition in Weltkoordinaten entspricht der Translation der *inversen* View-Matrix (4. Spalte).

**Code:**
```glsl
// Fragment Shader
vec3 cam_pos_world = inverse(per_frame.view)[3].xyz;
vec3 view_direction = normalize(cam_pos_world - world_pos);
```

### 2.3 Lighting (`frag_scene_objects.glsl`)

**Ziel:** Summierung des Lichts aller 4 Lichtquellen unter Berücksichtigung der Entfernung.

**Erklärung:**
*   Wir iterieren durch alle 4 Lichter.
*   **Abstandsgesetz:** Lichtintensität fällt quadratisch mit der Entfernung ab ($I \propto 1/d^2$).
*   Die Funktion `phongLighting` berechnet dann diffuse und spekulare Anteile.

**Code:**
```glsl
for (int i = 0; i < 4; ++i) {
    vec3 light_diff = per_frame.light_position[i].xyz - world_pos;
    float distance = length(light_diff);
    vec3 light_dir = normalize(light_diff);

    // Attenuation (Dämpfung)
    float intensity = per_frame.light_color_intensity[i].a / (distance * distance);
    vec3 light_color = per_frame.light_color_intensity[i].rgb;

    reflected_light += phongLighting(n, light_dir, view_direction, k_d, k_s, shininess) * light_color * intensity;
}
```

### 2.4 Tangent-Space-Matrix (`vert_scene_objects.glsl` & `frag_scene_objects.glsl`)

**Ziel:** Vorbereitung für Normal Mapping. Wir benötigen eine Basis (TBN-Matrix), um Vektoren aus dem Tangentenraum (Textur) in den Weltraum zu transformieren.

**Vertex Shader:**
Wir transformieren die Tangente in den Weltraum und reichen das Vorzeichen der Bitangente weiter.
```glsl
out_tangent = normalize(normal_matrix * in_tangent.xyz);
out_bitangent_sign = in_tangent.w;
```

**Fragment Shader:**
Wir rekonstruieren die Bitangente mittels Kreuzprodukt (`N x T`) und bauen die TBN-Matrix.
```glsl
vec3 T = normalize(tangent);
vec3 N = normalize(normal);
vec3 B = normalize(cross(N, T)) * bitangent_sign;
mat3 TBN = mat3(T, B, N);
```

### 2.5 Normal-Map (`frag_scene_objects.glsl`)

**Ziel:** Nutzung der Normal Map für feine Oberflächendetails.

**Erklärung:**
1.  Normalen in der Textur sind als Farben `[0...1]` gespeichert. Wir mappen sie auf Vektoren `[-1...1]`.
2.  Wir multiplizieren diesen lokalen Vektor mit der TBN-Matrix, um die "echte" Ausrichtung im Weltraum zu erhalten.
3.  Diese neue Normale `n` überschreibt die interpolierte Vertex-Normale für die Beleuchtungsberechnung.

**Code:**
```glsl
// Mapping von [0,1] auf [-1,1]
vec3 n_tangent = normal_rgba.rgb * 2.0 - 1.0;
// Transformation in Weltkoordinaten
n = normalize(TBN * n_tangent);
```