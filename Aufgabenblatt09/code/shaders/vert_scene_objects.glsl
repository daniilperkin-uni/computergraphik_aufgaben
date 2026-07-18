#version 440 core

layout(binding = 0) uniform PerFrame {
    mat4 view; // world coords -> camera coords
    mat4 proj; // camera coords -> clip coords
    vec4 light_position[4];
    vec4 light_color_intensity[4];
}
per_frame;

uniform mat4 model_matrix;

layout(location = 0) in vec3 in_normal;
layout(location = 1) in vec3 in_pos;
layout(location = 2) in vec4 in_tangent;
layout(location = 3) in vec2 in_uv;

layout(location = 0) out vec3 out_world_pos;
layout(location = 1) out vec3 out_normal;
layout(location = 2) out vec3 out_tangent;
layout(location = 3) out float out_bitangent_sign;
layout(location = 4) out vec2 out_uv;

void main() {
    gl_Position = per_frame.proj * per_frame.view *  model_matrix * vec4(in_pos, 1.0);
    out_world_pos = (model_matrix * vec4(in_pos, 1.0)).xyz;
    out_uv = in_uv;

    // Compute normal matrix
    // TODO [Aufgabe 2.1]: Replace by correct transformation with normal matrix.
    mat3 normal_matrix = transpose(inverse(mat3(model_matrix)));
    out_normal = normalize(normal_matrix * in_normal);

    // TODO [Aufgabe 2.4]: Calculate the tangent transformation and store bitangent sign.
    // Tangente in Weltkoordinaten transformieren und normalisieren
    out_tangent = normalize(normal_matrix * in_tangent.xyz);
    // Vorzeichen der Bitangente für die spätere Berechnung im Fragment-Shader übergeben
    out_bitangent_sign = in_tangent.w;
}
