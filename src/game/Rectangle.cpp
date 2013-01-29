//#include <GL/glut.h>
#include "vector.h"
#include "Rectangle.h"

using namespace minigolf;

Rectangle::Rectangle()
{
	vert.x = vert.y = vert.z = 0;
}

Rectangle::Rectangle(vert_3f v)
{
	vert = v;
}

void Rectangle::draw()
{
	float x,y,z;
	x = vert.x;
	y = vert.y;
	z = vert.z;

	// get normal for tee
	float tee_norm[3];	
	vert_3f v1 = {x,y,z};
	vert_3f v2 = {x,y,z+0.1};
	vert_3f v3 = {x+0.1,y,z};
	calculate_normal(v1, v2, v3, tee_norm);

	glPushMatrix();
	glBegin(GL_QUADS);
	glColor3f(0,0,0.5);	
	glNormal3fv(tee_norm);
	glVertex3f(x,y,z);
	glVertex3f(x,y,z+0.1);
	glVertex3f(x+0.1,y,z+0.1);
	glVertex3f(x+0.1,y,z);		
	glEnd();
	glPopMatrix();
}

