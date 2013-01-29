#ifndef VECTOR_H
#define VECTOR_H

#include "Graphics.h"
#include "Polygon.h"
#include "Tile.h"


float vector_magnitude(float *);
float vector_magnitude(vert_3f);

float* vector_mult(float *, float);
//float* vector_mult(float *, float);
void vector_add(float *, float *, float *);

void normalize(float *);
void norm_cross_prod(float *, float *, float *);
void calculate_normal(vert_3f, vert_3f, vert_3f, float *);

float dist_point_to_plane(vert_3f, Tile *);
float dist_point_to_plane(vert_3f, minigolf::Polygon *);

float ang_between(float *, float *);



#endif