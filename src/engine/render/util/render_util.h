#pragma once

float *get_mouse_ray(float x, float y);
float did_circles_collide(float x1, float y1, float r1, float x2, float y2, float r2);
short is_point_within_square(float square_x, float square_y, float square_width, float square_height, float x, float y);
short is_point_within_circle(float circle_x, float circle_y, float radius, float x, float y);
short is_point_within_circle(float circle_x, float circle_y, float radius, float x, float y);
float get_slope_of_line(float x_1, float y_1, float x_2, float y_2);
float get_line_constant(float x, float y, float slope);
float *get_point_on_line(float x, float y, float x_1, float y_1, float x_2, float y_2);