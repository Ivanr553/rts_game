#include <math.h>

#include "constants.h"
#include "../global.h"

/**
 * @brief Sets the position of a given coordinates to the current location of the mouse
 *
 * Used to claculate the true x and y on the screen from the current mouse coordinates
 *
 * @param x x position to update
 * @param y y position to update
 */
void set_pos_from_mouse_pos(float *x, float *y)
{
    *x = global.mouse.x;
    *y = viewportHeight - global.mouse.y;
};

void normalize_vector(vec3 *vector)
{
    float w = sqrt(*vector[0] * *vector[0] + *vector[1] * *vector[1] + *vector[2] * *vector[2]);
    *vector[0] /= w;
    *vector[1] /= w;
    *vector[2] /= w;
};

void cross_vector(vec3 *dest, vec3 *v1, vec3 *v2)
{
    *dest[0] = *v1[1] * *v2[2] - *v1[2] * *v2[1];
    *dest[1] = -(*v1[0] * *v2[2] - *v1[2] * *v2[0]);
    *dest[2] = *v1[0] * *v2[1] - *v1[1] * *v2[0];
};

void invert_vector(vec3 *dest, vec3 *v)
{
    *dest[0] = -*v[0];
    *dest[1] = -*v[1];
    *dest[2] = -*v[2];
};

void add_vector(vec3 *dest, vec3 *v1, vec3 *v2)
{
    *dest[0] = *v1[0] + *v2[0];
    *dest[1] = *v1[1] + *v2[1];
    *dest[2] = *v1[2] + *v2[2];
};

int numPlaces(int n)
{
    if (n < 0) n = (n == INT_MIN) ? INT_MAX : -n;
    if (n < 10) return 1;
    if (n < 100) return 2;
    if (n < 1000) return 3;
    if (n < 10000) return 4;
    if (n < 100000) return 5;
    if (n < 1000000) return 6;
    return 10;
}