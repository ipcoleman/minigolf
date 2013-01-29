#include <iostream>
#include <GL/glut.h>
#include "Transformation.h"
#include "Rotation.h"

using namespace std;

Rotation::Rotation(int ang, int ax_x, int ax_y, int ax_z)
{
	type = Transformation::ROTATE;
	angle = ang;
	//angle_per_tick = calc_angle_per_tick();
	angle_rot = 0;
	angle_per_tick = 0;
	axis_x = ax_x;
	axis_y = ax_y;
	axis_z = ax_z;
	active = false;
}

int Rotation::get_angle_per_tick()
{
	return angle_per_tick;
}

void Rotation::calc_change_per_tick()
{
	if((complete - begin) > 0)
		angle_per_tick = angle/(complete - begin);
	else
		angle_per_tick = 0;
}

void Rotation::add_angle_rot(int ang_incr)
{
	angle_rot += ang_incr;
}

//void Rotation::angle_plus(int angle_incr)
//{
//	angle_per_tick += angle_incr;
//}

/*
===========================
transform

	Only used for static rotations
	(hence the lack of a tick param)
===========================
*/
void Rotation::transform()
{
	if(change == STATIC)
	{		
		angle_rot = angle;
	}
	//else if (change == DYNAMIC)
	//{
	//	// check if current tick falls in interval
	//	// of current transformation
	//	if(tick >= begin && tick <= complete)
	//	{
	//		if(angle_rot > 360)
	//			angle_rot -= 360; // reset after each full rotation
	//		add_angle_rot(angle_per_tick);	
	//	}
	//}
	//glPushMatrix();
	glRotatef(angle_rot, axis_x, axis_y, axis_z);	
	//glPopMatrix();
}

void Rotation::transform(int tick)
{
	/*if(change == STATIC)
	{		
		angle_rot = angle;
	}*/
	/*else */if (change == DYNAMIC)
	{
		// check if current tick falls in interval
		// of current transformation
		if(tick >= begin && tick <= complete)
		{
			if(angle_rot > 360)
				angle_rot -= 360; // reset after each full rotation
			add_angle_rot(angle_per_tick);	
		}
	}
	//glPushMatrix();
	glRotatef(angle_rot, axis_x, axis_y, axis_z);	
	//glPopMatrix();
}