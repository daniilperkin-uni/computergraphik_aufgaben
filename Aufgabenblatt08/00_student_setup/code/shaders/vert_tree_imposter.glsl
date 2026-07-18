#version 440 core

layout(binding = 0) uniform PerFrame {
    mat4 view; // world coords -> camera coords
    mat4 proj; // camera coords -> clip coords
}
per_frame;

layout(binding = 1) uniform Trees {
    mat4 model[192]; // object coords -> world coords
}
trees;

layout(location = 0) in vec3 in_pos; // position in object coords
layout(location = 1) in vec3 in_uv;  // uv coordinates for texture mapping

layout(location = 1) out vec2 uv;

// Calculates a pseudo random number in [0, 1]
float rand(vec2 co) {
    return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

void main() {
    // TODO 2.2 
    // Lösung aus vert_trees.glsl wiederverwenden!
    mat4 model_view = per_frame.view * trees.model[gl_InstanceID];
    model_view[0] = vec4(1, 0, 0, 0);
    model_view[2] = vec4(0, 0, 1, 0);

    // TODO 2.5
    // Für ein überzeugenderes Ergebnis eine Zufallszahl zwischen 0,7 und 1,0 berechnen, um die Baum-Vertices zu skalieren
    // Die mitgelieferte Funktion rand mit der xz-Position des Baums in der Szene als Eingabe verwenden
    float scale = 0.7 + 0.3 * rand(trees.model[gl_InstanceID][3].xz);

    // Ausgabe
    gl_Position = per_frame.proj * model_view * vec4(in_pos * scale, 1);
    uv = in_uv.rg;
}
