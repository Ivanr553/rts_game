#include <glad/glad.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <linmath.h>

#include "render_util.h"

#include "../../util/util.h"
#include "../../io/io.h"
#include "../render_internal.h"
#include "../../global.h"

u32 render_shader_create(const char *path_vert, const char *path_frag)
{
    int success;
    char log[512];

    File file_vertex = io_file_read(path_vert);
    if (!file_vertex.is_valid)
    {
        ERROR_EXIT("Error reading shader: %s\n", path_vert);
    }

    u32 shader_vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(shader_vertex, 1, (const char *const *)&file_vertex, NULL);
    glCompileShader(shader_vertex);
    glGetShaderiv(shader_vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader_vertex, 512, NULL, log);
        ERROR_EXIT("Error compiling vertex shader. %s\n", log);
    }

    File file_fragment = io_file_read(path_frag);
    if (!file_fragment.is_valid)
    {
        ERROR_EXIT("Error reading shader: %s\n", path_frag);
    }

    u32 shader_fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shader_fragment, 1, (const char *const *)&file_fragment, NULL);
    glCompileShader(shader_fragment);
    glGetShaderiv(shader_fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader_fragment, 512, NULL, log);
        ERROR_EXIT("Error compiling vertex fragment. %s\n", log);
    }

    u32 shader = glCreateProgram();
    glAttachShader(shader, shader_vertex);
    glAttachShader(shader, shader_fragment);
    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shader, 512, NULL, log);
        ERROR_EXIT("Error linking shader. %s\n", log);
    }

    free(file_vertex.data);
    free(file_fragment.data);

    return shader;
}

float *get_mouse_ray(float x, float y)
{
    float *mouse_ray = calloc(1, sizeof(vec3));
    vec4 clip_coords = {
        x,
        y,
        -1,
        1};

    mat4x4 inverted_projection;
    mat4x4_invert(&inverted_projection[0], global.camera.projection);

    if (!global.camera.projection)
    {
        mouse_ray[0] = 0;
        mouse_ray[1] = 0;
        mouse_ray[2] = 0;
        return mouse_ray;
    }

    vec4 temp_eye_coords;
    mat4x4_mul_vec4(&temp_eye_coords[0], &inverted_projection[0], &clip_coords[0]);

    vec4 eye_coords = {temp_eye_coords[0], temp_eye_coords[1], -1, 0};

    mat4x4 inverted_view;
    mat4x4_invert(&inverted_view[0], global.camera.view);

    vec4 ray_world;
    mat4x4_mul_vec4(&ray_world[0], inverted_view, &eye_coords[0]);

    vec3 _mouse_ray = {ray_world[0], ray_world[1], ray_world[2]};

    mouse_ray[0] = _mouse_ray[0];
    mouse_ray[1] = _mouse_ray[1];
    mouse_ray[2] = _mouse_ray[2];

    return mouse_ray;
};

float *get_ray_from_line(float *start, float *end)
{
    float *ray = calloc(1, sizeof(vec3));

    float ray_0 = end[0] - start[0];
    float ray_1 = end[1] - start[1];
    float ray_2 = end[2] - start[2];

    float average = (ray_0 + ray_1, +ray_2) / 3;

    ray[0] = ray_0;
    ray[1] = ray_1;
    ray[2] = ray_2;

    float w = sqrt(ray[0] * ray[0] + ray[1] * ray[1] + ray[2] * ray[2]);
    ray[0] /= w;
    ray[1] /= w;
    ray[2] /= w;

    return ray;
}

float did_circles_collide(float x1, float y1, float r1, float x2, float y2, float r2)
{
    return fabs((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)) < (r1 + r2) * (r1 + r2);
}

short is_point_within_square(float square_x, float square_y, float square_width, float square_height, float x, float y)
{
    float left_bound = square_x - square_width / 2;
    float right_bound = square_x + square_width / 2;
    float top_bound = square_y - square_height / 2;
    float bottom_bound = square_y + square_height / 2;

    short within_x = 0;
    short within_y = 0;

    if ((x >= left_bound) && (x <= right_bound))
    {
        // printf("Within x bound\n");
        within_x = 1;
    }

    if ((y >= bottom_bound) && (y <= top_bound))
    {
        // printf("Within y bound\n");
        within_y = 1;
    }

    if ((within_x + within_y) > 1)
    {
        return 1;
    }

    return 0;
}

short is_point_within_circle(float circle_x, float circle_y, float radius, float x, float y)
{
    float dist = sqrtf(powf(x - circle_x, 2) + powf(y - circle_y, 2));

    return dist <= radius;
}

float get_slope_of_line(float x_1, float y_1, float x_2, float y_2)
{
    float d_x = x_2 - x_1;
    float d_y = y_2 - y_1;

    return d_y / d_x;
}

float get_line_constant(float x, float y, float slope)
{
    return y - (x * slope);
}

float *get_point_on_line(float x, float y, float x_1, float y_1, float x_2, float y_2)
{
    float slope = get_slope_of_line(x_1, y_1, x_2, y_2);
    float line_constant = get_line_constant(x_1, y_1, slope);

    float return_y = slope * x + line_constant;
    float return_x = (y - line_constant) / slope;

    float *point = calloc(2, sizeof(float));
    point[0] = return_x;
    point[1] = return_y;

    return point;
};