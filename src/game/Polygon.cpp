//#include <GL/glut.h>

#include "Polygon.h"
#include "vector.h"
#include <iostream>

using namespace minigolf;

Polygon::Polygon()
{
	vertices.clear();
}

Polygon::Polygon(std::vector<vert_3f> v)
{
	vertices = v;
}

std::vector<vert_3f> Polygon::get_vertices()
{
	return vertices;
}

std::vector<vert_3f>* Polygon::get_verts()
{
	return &vertices;
}

float* Polygon::get_normal()
{
	return norm;
}

RGBA Polygon::get_color()
{
	return color;
}

void Polygon::set_color(RGBA col)
{
	color.r = col.r;	
	color.g = col.g;
	color.b = col.b;
	color.a = col.a;
}

void Polygon::draw()
{
	int vertArrLength = (int)this->vertices.size();
	float r,g,b;
	//float r = (float)rand()/(float)RAND_MAX;
	//float green_float = 0.45f; // dark green
	//green_float = r;
	//r = color.r / 255;
	//g = color.g / 255;
	//b = color.b / 255;

	GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat mat_diffuse[] = {1.0, 1.0, 1.0, 1.0}; 

	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	glEnable(GL_COLOR_MATERIAL);	

	//glPushMatrix();
	glBegin(GL_POLYGON);	
	glColor3f(color.r, color.g, color.b);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); 
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse); 	
	//glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE); 

	
	this->calc_normal_vector();
	// calc_normal_vector must be called before this	
	glNormal3fv(this->norm);
	
	//glTranslatef(0, 0.5, 0);
	//glRotatef(-90, 1, 0, 0);
	for(int i=0; i < vertArrLength; i++)
	{		
		
		//std::cout << "Drawing poly @ " <<  vertices[i].x << ", " <<  vertices[i].y << ", " << vertices[i].z << std::endl;
		glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);		
	}

    glEnd();
	//glPopMatrix();
}

void Polygon::add_vert(vert_3f v)
{	
	vertices.push_back(v);		
}

void Polygon::clear_verts()
{
	vertices.clear();
}

void Polygon::calc_normal_vector()
{

	calculate_normal(vertices[0], vertices[1], vertices[2], norm);

	/*float v1_x = vertices[0].x - vertices[1].x;
    float v1_y = vertices[0].y - vertices[1].y;
    float v1_z = vertices[0].z - vertices[1].z;

    float v2_x= -(vertices[2].x - vertices[1].x);
    float v2_y= -(vertices[2].y - vertices[1].y);
    float v2_z= -(vertices[2].z - vertices[1].z);

	float v1[] = {v1_x, v1_y, v1_z};
	float v2[] = {v2_x, v2_y, v2_z};*/
	//float norm[3] = {0};
    
	
	//norm_cross_prod(v1, v2, this->norm);
	//cout << "Normal: " << norm[0] << ", " << norm[1] << ", " << norm[2] << endl;

    /*normX = v1_y * v2_z -   v1_z * v2_y;
    normY =  -( v1_x * v2_z - v1_z *v2_x);
    normZ = (v1_x * v2_y - v1_y * v2_x);
    
    float length = (float)(sqrt((normX*normX ) + (normY * normY) + (normZ * normZ)));
    normX /= length; 
    normY /= length;
    normZ /= length;*/
	//cout << "Calculating normal vector..." << endl;
}

float Polygon::calc_slope()
{
	int n = 0;
	float slope;
	float x,y;
	vert_3f v1, v2;
	std::vector<vert_3f> verts; 

	verts = get_vertices();
	n = verts.size();

	for(int i=0; i<n-1; i++)
	{
		v1 = verts.at(i);
		v1 = verts.at(i+1);

		if(abs(v1.y - v2.y) > 0)
		{
			y = abs(v1.y - v2.y);
		}
	}

	slope = y/x;

	return slope;
}

