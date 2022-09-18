#version 330 core
out vec4 frag_color;

// texture
in vec4 colors;

void main() {
    vec4 texture_color = colors;

    frag_color = texture_color;
}