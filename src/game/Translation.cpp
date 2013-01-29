#include <iostream>
#include <GL/glut.h>
#include "Transformation.h"
#include "Translation.h"

using namespace std;


Translation::Translation(float ex, float why, float zed)
{
	type = Transformation::TRANSLATE;
	change = -1;
	x = ex;
	y = why;
	z = zed;
	x_per_tick = 0;
	y_per_tick = 0;
	z_per_tick = 0;
	x_tot = 0;
	y_tot = 0;
	z_tot = 0;
	active = false;
}

float Translation::get_x()
{
	return x;
}

float Translation::get_y()
{
	return y;
}

float Translation::get_z()
{
	return z;
}

float Translation::get_xpt()
{
	return x_per_tick;
}

float Translation::get_ypt()
{
	return y_per_tick;
}

float Translation::get_zpt()
{
	return z_per_tick;
}

void Translation::calc_change_per_tick()
{
	x_per_tick = x/(float)(complete - begin); 
	y_per_tick = y/(float)(complete - begin); 
	z_per_tick = z/(float)(complete - begin);
}

/*
===========================
add_total_trans

	Adds (x,y,z) values to total
	translation value
===========================
*/
void Translation::add_total_trans(float x_incr, float y_incr, float z_incr)
{
	x_tot += x_incr;
	y_tot += y_incr;
	z_tot += z_incr;
}

/*
===========================
transform

	Only used for static translations
	(hence the lack of a tick param)
===========================
*/
void Translation::transform()
{
	//cout << "TRANSLATING" << endl;
	if(change == STATIC)
	{
		x_tot = x;
		y_tot = y;
		z_tot = z;
	}
	/*else if(change == DYNAMIC)
	{
		
		add_total_trans(x_per_tick, y_per_tick, z_per_tick);
		
	}*/
	//glPushMatrix();
	glTranslatef(x_tot, y_tot, z_tot);
	//glPopMatrix();
}

void Translation::transform(int tick)
{
	//cout << "TRANSLATING" << endl;
	/*if(change == STATIC)
	{
		x_tot = x;
		y_tot = y;
		z_tot = z;
	}*/
	/*else */if(change == DYNAMIC)
	{
		if(tick >= begin && tick <= complete)
		{
			add_total_trans(x_per_tick, y_per_tick, z_per_tick);
		}
	}
	
	glMatrixMode(GL_MODELVIEW);
	/*glPushMatrix();
	glLoadIdentity();*/
	glTranslatef(x_tot, y_tot, z_tot);
	/*glPopMatrix();*/
	
}

