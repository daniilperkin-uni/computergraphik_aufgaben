#version 440 core

layout(binding = 0) uniform PerFrame {
    mat4 view; // world coords -> camera coords
    mat4 proj; // camera coords -> clip coords
    vec4 light_position[4];
    vec4 light_color_intensity[4];
}
per_frame;

uniform sampler2D albedo_tx2D;
uniform sampler2D normal_tx2D;
uniform sampler2D metallic_roughness_tx2D;

layout(location = 0) in vec3 world_pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 tangent;
layout(location = 3) in float bitangent_sign;
layout(location = 4) in vec2 uv;

layout(location = 0) out vec4 out_color;

vec3 phongLighting(vec3 normal, vec3 light_direction, vec3 view_direction, vec3 k_d, vec3 k_s, float shininess) {
    // diffuse term
    vec3 color = k_d * max(dot(normal, light_direction), 0.0);

    // specular term
    vec3 reflect_dir = reflect(-light_direction, normal);
    color += pow(clamp(dot(reflect_dir, view_direction), 0.0, 1.0), shininess) * k_s;

    return color;
}

// Source: https://gamedev.stackexchange.com/questions/92015/optimized-linear-to-srgb-glsl
// Converts a color from sRGB gamma to linear light gamma
vec4 toLinear(vec4 sRGB) {
    bvec4 cutoff = lessThan(sRGB, vec4(0.04045));
    vec4 higher = pow((sRGB + vec4(0.055)) / vec4(1.055), vec4(2.4));
    vec4 lower = sRGB / vec4(12.92);

    return mix(higher, lower, cutoff);
}

void main() {
    vec4 albedo_rgba = toLinear(texture(albedo_tx2D, uv));
    vec4 normal_rgba = texture(normal_tx2D, uv);
    vec4 metallic_roughness_rgba = texture(metallic_roughness_tx2D, uv);

    if (albedo_rgba.a < 0.01) {
        discard;
    }

    // Translate PBR material params to phong:
    // No diffuse reflection for metals.
    // Metallic value is not a good basis for k_s and shininess of phong, so derive both from roughness.
    vec3 k_d = albedo_rgba.rgb * (metallic_roughness_rgba.g) * (1.0 - metallic_roughness_rgba.b);
    vec3 k_s = albedo_rgba.rgb * (1.0 - metallic_roughness_rgba.g);
    float shininess = mix(1.0, 50.0, pow((1.0 - metallic_roughness_rgba.g), 4.0));

    // initialize surface reflection with fixed ambient term
    vec3 reflected_light = 0.05 * albedo_rgba.rgb;

    // initialize used normal n with per-vertex normal
    vec3 n = normal;

    // use and transform normal map entries to world space
    // TODO [Aufgabe 2.4]: Calculate the tangent-space-matrix.
    // Interpolierte Vektoren erneut normalisieren
    vec3 T = normalize(tangent);
    vec3 N = normalize(normal);
    // Bitangente orthogonal zu Tangente und Normale berechnen
    vec3 B = normalize(cross(N, T)) * bitangent_sign;
    // TBN-Matrix konstruieren (Tangentenraum -> Weltraum)
    mat3 TBN = mat3(T, B, N);

    // TODO [Aufgabe 2.5]: Apply the tangent-space-matrix.
    // Normalen aus der Textur von [0, 1] auf [-1, 1] umrechnen
    vec3 n_tangent = normal_rgba.rgb * 2.0 - 1.0;
    // Normale mit der TBN-Matrix in den Weltraum transformieren und normalisieren
    n = normalize(TBN * n_tangent);

    // reconstruct view direction using view normal
    // TODO [Aufgabe 2.2]: Determine the view_direction.
    // Kameraposition in Weltkoordinaten aus der inversen View-Matrix extrahieren
    vec3 cam_pos_world = inverse(per_frame.view)[3].xyz;
    // Vektor vom Fragment zur Kamera berechnen und normalisieren
    vec3 view_direction = normalize(cam_pos_world - world_pos);

    // iterate over all light sources and compute phong lighting
    // TODO [Aufgabe 2.3]: Calculate local lighting for all 4 light sources and add to reflected_light.
    //      You can use the phongLighting() function
    for (int i = 0; i < 4; ++i) {
        // Vektor von Fragment zur Lichtquelle
        vec3 light_diff = per_frame.light_position[i].xyz - world_pos;
        float distance = length(light_diff);
        vec3 light_dir = normalize(light_diff);

        // Lichtintensität nimmt quadratisch mit dem Abstand ab
        float intensity = per_frame.light_color_intensity[i].a / (distance * distance);
        vec3 light_color = per_frame.light_color_intensity[i].rgb;

        // Phong-Beleuchtung berechnen und aufsummieren
        reflected_light += phongLighting(n, light_dir, view_direction, k_d, k_s, shininess) * light_color * intensity;
    }


    // exposure
    reflected_light *= 1.5;
    // tone mapping (Reinhard et al.)
    reflected_light = reflected_light / (vec3(1.0) + reflected_light);
    // gamma correction
    reflected_light = vec3(pow(reflected_light, vec3(1.0 / 2.2)));

    out_color = vec4(reflected_light, 1.0);
}
