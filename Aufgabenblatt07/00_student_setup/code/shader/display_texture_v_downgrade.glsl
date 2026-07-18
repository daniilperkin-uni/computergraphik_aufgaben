#version 330

in vec4 vertex;
out vec2 uvCoord;

void main() {
    uvCoord = vertex.zw;
    gl_Position =  vec4(vertex.xy, -1.0, 1.0);
}
