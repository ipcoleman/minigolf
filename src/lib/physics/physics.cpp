#include "physics.h"
#include "Ball.h"

/*
===========================
get_border_normal

	Gets normal vector for course
	border for calculating ball 
	reflect vector
===========================
*/
void get_border_normal(float x, float y, float z, float width, float height, float *outVector)
{
	
}

/*
===========================
get_outgoing_collision_angle
===========================
*/
float get_outgoing_collision_angle(float *dirIn, float *norm, float *dirOut)
{
	// FILL THIS OUT
	return 0;
	
}

void add_vectors(float *v1, float *v2, float *vOut)
{
	float x_out, y_out, z_out;

	x_out = v1[0] + v2[0];
	y_out = v1[1] + v2[1];
	z_out = v1[2] + v2[2];
	
	vOut[0] = x_out;
	vOut[1] = y_out;
	vOut[2] = z_out;	
}

void neg_vector(float *v)
{
	v[0] = -v[0];
	v[1] = -v[1];
	v[2] = -v[2];
}


//void transform_vert(vert_3f *v, float *mat)
//{
//	v = (vert_3f *)malloc(sizeof(vert_3f));
//	v->x = 1;
//	v->y = 2; 
//	v->z = 3;
//}
// used to transform vert by matrix (IN PLACE)
void transform_vert(vert_3f *v, float *mat)
{
	float x, y, z, w;
	//float w = 1;
	float x_t, y_t, z_t, w_t;

	x = v->x;
	y = v->y;
	z = v->z;
	w = 1;

	x_t = x*mat[0] + y*mat[4] + z*mat[8] + mat[12];// + w*mat[12];		
	y_t = x*mat[1] + y*mat[5] + z*mat[9] + mat[13];// + w*mat[13];		
	z_t = x*mat[2] + y*mat[6] + z*mat[10] + mat[14];// + w*mat[14];
	w_t = x*mat[3] + y*mat[7] + z*mat[11] + mat[15];// + w*mat[14];

	x_t /= w_t;
	y_t /= w_t;
	z_t /= w_t;

	// save back to vert
	v->x = x_t;
	v->y = y_t;
	v->z = z_t;

	//vert_3f result = {x_t, y_t, z_t};
	//return result;
}

//void transform_point(float *x, float *y, float *z, mat_x, mat_y, mat_z, mat_w)
//{
//	//float x, y, z, w;
//	float x_t, y_t, z_t, w_t;
//
//	x_t = x*mat[0] + y*mat[4] + z*mat[8] + mat[12];// + w*mat[12];		
//	y_t = x*mat[1] + y*mat[5] + z*mat[9] + mat[13];// + w*mat[13];		
//	z_t = x*mat[2] + y*mat[6] + z*mat[10] + mat[14];// + w*mat[14];
//	w_t = x*mat[3] + y*mat[7] + z*mat[11] + mat[15];// + w*mat[14];
//
//	x_t /= w_t;
//	y_t /= w_t;
//	z_t /= w_t;
//}

//float* get_ball_power(Ball *ball, int mouse_x, int mouse_y, int width, int height)
//{
//	float power[] = {0,0,0};
//
//	return power;
//}
