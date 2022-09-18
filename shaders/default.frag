#version 330 core
out vec4 frag_color;

// texture
in vec2 uvs;
in vec4 colors;

// lighting
in vec3 normal;
in vec3 pos;

uniform sampler2D texture_id;
uniform vec4 light_color;
uniform vec3 light_pos;

void main() {

    float ambient = 0.2;

    vec3 normal = normalize(normal);
    vec3 light_direction = normalize(light_pos - pos);

    float diffuse = max(dot(normal, light_direction), 0);
    
    vec4 texture_color = texture(texture_id, uvs) * light_color * (diffuse + ambient);
    if(texture_color.a < 0.1)
        discard;

    frag_color = texture_color;
}