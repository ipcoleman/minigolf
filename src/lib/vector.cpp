#include "vector.h"
#include "Graphics.h"
#include "physics.h"
#include <math.h>

/*
===========================
magnitude

	Return the magnitude
	of a vector
===========================
*/ 
float vector_magnitude(float *vec)
{
	return sqrt( pow(vec[0], 2) + pow(vec[1], 2) + pow(vec[2], 2) );
}

/*
===========================
magnitude

	Return the magnitude
	of a vector whose direction
	is denoted by a vertex
===========================
*/ 
float vector_magnitude(vert_3f v)
{
	return sqrt( pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2) );
}

float* vector_mult(float *v, float scalar)
{
	v[0] *= scalar;
	v[1] *= scalar;
	v[2] *= scalar;

	return v;
}

void vector_add(float *v1, float *v2, float *vOut)
{
	vOut[0] = v1[0] + v2[0];
	vOut[1] = v1[1] + v2[1];
	vOut[2] = v1[2] + v2[2];
}

/*
===========================
normalize
===========================
*/
void normalize(float v[3]) {    
	float d = sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]); 
	if (d == 0.0) 
	{
		//error("zero length vector");    
		//can't normalize
		return;
	}

	v[0] /= d; 
	v[1] /= d; 
	v[2] /= d; 
}

/*
===========================
norm_cross_prod
===========================
*/
void norm_cross_prod(float v1[3], float v2[3], float out[3])
{
	int i, j; 
	float length;

	out[0] = v1[1]*v2[2] - v1[2]*v2[1]; 
	out[1] = v1[2]*v2[0] - v1[0]*v2[2]; 
	out[2] = v1[0]*v2[1] - v1[1]*v2[0]; 
	normalize(out); 
}

void calculate_normal(vert_3f v1, vert_3f v2, vert_3f v3, float *norm)
{
	float v1_x = v1.x - v2.x;
    float v1_y = v1.y - v2.y;
    float v1_z = v1.z - v2.z;

    float v2_x= v3.x - v2.x;
    float v2_y= v3.y - v2.y;
    float v2_z= v3.z - v2.z;

	float vec1[] = {v1_x, v1_y, v1_z};
	float vec2[] = {v2_x, v2_y, v2_z};
	//float norm[3] = {0};
    
	norm_cross_prod(vec1, vec2, norm);
}

float dist_point_to_plane(vert_3f v, Tile *t)
{
	//dist = n . r  + D
	float dist;
	float dist_vec[3]; 
	float *tile_norm;	
	float temp;
	float D;
	vert_3f v0;
	
	v0.x = t->get_vertices().at(0).x;
	v0.y = t->get_vertices().at(0).y;
	v0.z = t->get_vertices().at(0).z;

	
	t->calc_normal_vector();
	tile_norm = t->get_normal();

	D = -tile_norm[0] * v0.x + -tile_norm[1] * v0.y + -tile_norm[2] * v0.z;

	//norm_cross_prod(tile_norm, v, temp);
	dist = tile_norm[0] * v.x + tile_norm[1] * v.y + tile_norm[2] * v.z;
	dist += D;

	return dist;

}

float dist_point_to_plane(vert_3f v, minigolf::Polygon *p)
{
	//dist = n . r  + D
	float dist;
	float dist_vec[3]; 
	float *poly_norm;
	//float temp[3];
	float temp;
	float D;
	vert_3f v0;
	
	v0.x = p->get_vertices().at(0).x;
	v0.y = p->get_vertices().at(0).y;
	v0.z = p->get_vertices().at(0).z;

	
	p->calc_normal_vector();
	poly_norm = p->get_normal();

	D = -poly_norm[0] * v0.x + -poly_norm[1] * v0.y + -poly_norm[2] * v0.z;

	//norm_cross_prod(tile_norm, v, temp);
	dist = poly_norm[0] * v.x + poly_norm[1] * v.y + poly_norm[2] * v.z;
	dist += D;
	/*temp[0] = tile_norm[0] * v[0];
	temp[1] = tile_norm[1] * v[1];
	temp[2] = tile_norm[2] * v[2];*/


	//temp[0] = temp[0] + 

	return dist;
}

float ang_between(float *v1, float *v2)
{
	// cos theta = (AxBx + AyBy + AzBz) / AB
	float v1Xv2;
	float v1_mag, v2_mag;
	float theta;

	v1Xv2 = v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2];
	v1_mag = vector_magnitude(v1);
	v2_mag = vector_magnitude(v2);
	theta = acos(v1Xv2/(v1_mag * v2_mag)) * (180/M_PI);

	return theta;
}

