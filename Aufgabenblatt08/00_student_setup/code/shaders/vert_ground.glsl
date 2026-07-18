#version 440 core

layout(binding = 0) uniform PerFrame {
    mat4 view; // world coords -> camera coords
    mat4 proj; // camera coords -> clip coords
}
per_frame;

layout(location = 0) in vec3 in_pos; // position in world coords
layout(location = 1) in vec3 in_color;

layout(location = 1) out vec3 frag_color;

void main() {
    gl_Position = per_frame.proj * per_frame.view * vec4(in_pos, 1.0);
    frag_color = in_color;
}
