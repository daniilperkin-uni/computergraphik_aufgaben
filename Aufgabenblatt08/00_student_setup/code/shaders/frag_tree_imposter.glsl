#version 440 core

uniform sampler2D tree_sprite_tx2D;

layout(location = 1) in vec2 uv;

layout(location = 0) out vec4 out_color;

void main() {

    // TODO 2.4

    // out_color auf den aus der Textur abgerufenen Wert setzen

    out_color = texture(tree_sprite_tx2D, uv);

    // Wenn der Alpha-Wert unter einem Schwellenwert liegt, das Fragment verwerfen

    // Mit verschiedenen Schwellenwerten experimentieren und einen finden, der

    // optisch ansprechende Ergebnisse liefert

    if (out_color.a < 0.5) {

        discard;

    }

}
