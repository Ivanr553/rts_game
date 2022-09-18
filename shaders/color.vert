#version 330 core
layout (location = 0) in vec3 a_pos;

out vec4 colors;

// position and size
uniform mat4 camera;
uniform float scale;
uniform vec4 color;

void main() {
    colors = color;
    gl_Position = camera * vec4(a_pos, scale);
}