#ifndef PHYSICS_H
#define PHYSICS_H

#include "glut.h"
#include "Ball.h"


#ifndef GRAV
#define GRAV 9.81 // m/s^2
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


// collisions
void get_border_normal(float , float, float, float, float, float *);
float get_outgoing_collision_angle(float *, float *, float *);

// vectors
void add_vectors(float *, float *, float *);
void neg_vector(float *);

// used to transform vert by matrix
void transform_vert(vert_3f *, float *);
//void transform_point(float *, float *, float *, float, float, float, float);

// ball control
//float* get_ball_power(Ball *, int, int, int, int);

#endif