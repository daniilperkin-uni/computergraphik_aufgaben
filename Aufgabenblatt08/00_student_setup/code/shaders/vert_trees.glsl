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
layout(location = 1) in vec3 in_color;

layout(location = 1) out vec3 frag_color;

void main() {
    // TODO 1.4
    // View * Model berechnen
    mat4 model_view = per_frame.view * trees.model[gl_InstanceID];

    // TODO 1.5
    // X-Achse "gerade" machen
    model_view[0] = vec4(1, 0, 0, 0);
    // Z-Achse "gerade" machen
    model_view[2] = vec4(0, 0, 1, 0); 

    // Ausgabe
    gl_Position = per_frame.proj * model_view * vec4(in_pos, 1);
    frag_color = in_color;
}
