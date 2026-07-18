#version 330

uniform sampler2D display_tx2D;

in vec2 uvCoord;
out vec4 frag_color;

void main()
{
    frag_color = texture(display_tx2D, vec2(uvCoord.x, 1.0 - uvCoord.y));
    //frag_color = vec4(uvCoord, 0.0, 1.0);
}
