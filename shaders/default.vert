#version 330 core
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec2 a_uvs;
layout (location = 2) in vec4 a_colors;
layout (location = 3) in vec3 a_normal;


// texture
out vec2 uvs;
out vec4 colors;

// lighting
out vec3 normal;
out vec3 pos;

// position and size
uniform mat4 model;
uniform mat4 camera;
uniform float scale;


void main() {
    uvs = a_uvs;
    colors = a_colors;
    pos = vec3(model * vec4(a_pos, scale));
    normal = a_normal;

    vec4 position = camera * vec4(pos, scale);
    gl_Position = position;
}