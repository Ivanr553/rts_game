#pragma once

#include <stdio.h>
#include <linmath.h>

#include "constants.h"

#define ERROR_EXIT(...) fprintf(stderr, __VA_ARGS__); exit(1)
#define ERROR_RETURN(R, ...) fprintf(stderr, __VA_ARGS__); return R

void set_pos_from_mouse_pos(float *x, float *y);
void normalize_vector(vec3 *vector);
void cross_vector(vec3 *dest, vec3 *v1, vec3 *v2);
void invert_vector(vec3 *dest, vec3 *v);
void add_vector(vec3 *dest, vec3 *v1, vec3 *v2);
int numPlaces(int n);