#include <vector>
#include <iostream>

#include "Rectangle.h"
//#include "Polygon.h"
#include "Tile.h"
//#include "Graphics.h"
#include "vector.h"

using namespace std;

//#include <cstdlib>


Tile::Tile()
{	
	id = -1;
}

Tile::Tile(std::vector<vert_3f> v)
{
	vertices = v;
	id = -1;
}

Tile::Tile(std::vector<vert_3f> v, int tID)
{
	vertices = v;
	id = tID;
}

Tile::Tile(vert_3f v1, vert_3f v2, vert_3f v3, vert_3f v4)
{
	id = -1;
}

Tile::Tile(vert_3f v1, vert_3f v2, vert_3f v3, vert_3f v4, RGBA c)
{
	color = c;
	id = -1;
}

int Tile::get_id()
{
	return id;
}

void Tile::set_id(int _id)
{
	id = _id;
}

std::vector<vert_3f> Tile::get_vertices()
{
	return vertices;
}

std::vector<int> Tile::get_border_tiles()
{
	return borderTileIDs;
}

void Tile::add_border_tile_id(int t_id)
{
	borderTileIDs.push_back(t_id);
}

void Tile::add_tile_border(minigolf::Polygon *p)
{
	tile_borders.push_back(p);	
}

std::vector<minigolf::Polygon *> Tile::get_tile_borders()
{
	return tile_borders;
}

void Tile::draw()
{
	int vertArrLength = (int)this->vertices.size();
	//float r = (float)rand()/(float)RAND_MAX;
	float green_float = 0.45f; // dark green
	//green_float = r;
	GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat mat_diffuse[] = {1.0, 1.0, 1.0, 1.0}; 

	glEnable(GL_COLOR_MATERIAL);	

	glPushMatrix();
	glBegin(GL_POLYGON);
	glColor3f(0.0f,green_float,0.0f);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); 
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse); 	
	//glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE); 

	
	this->calc_normal_vector();
	// calc_normal_vector must be called before this	
	glNormal3fv(this->norm);
	
	for(int i=0; i < vertArrLength; i++)
	{		
		glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);		
	}

    glEnd();
	glPopMatrix();

	//cout << "Drawing Tile " << id << endl;

	/*for(int i=0; i<(int)borderTiles.size(); i++)
	{
		cout << "Bordering Tile ID for Tile " << id << ": " << borderTiles.at(i) << endl;
	}*/
}

//void Tile::draw_border(vert_3f v1, vert_3f v2)
//{
//	glBegin(GL_LINE);
//	glColor3f(1.0, 0.0, 0.0);
//	glLineWidth(5.0);
//	glVertex3f(v1.x, v1.y, v1.z);
//	glVertex3f(v2.x, v2.y, v2.z);
//	glEnd();
//
//	/*glBegin(GL_QUADS);
//	glColor3f(1.0, 0.0, 0.0);
//	glVertex3f(v1.x, v1.y, v1.z);
//	glVertex3f(v2.x, v2.y, v2.z);
//	glVertex3f(v2.x, v2.y+0.5, v2.z);
//	glVertex3f(v1.x, v1.y+0.5, v1.z);
//	glEnd();
//
//	glBegin(GL_QUADS);
//	glColor3f(1.0, 0.0, 0.0);
//	glVertex3f(v1.x, v1.y, v1.z);
//	glVertex3f(v2.x, v2.y, v2.z);
//	glVertex3f(v2.x, v2.y+0.5, v2.z);
//	glVertex3f(v1.x, v1.y+0.5, v1.z);
//	glEnd();*/
//}
//
//void Tile::draw_border_poly(vert_3f v1, vert_3f v2)
//{
//	// draw polygon border
//	glBegin(GL_QUADS);
//	glColor3f(1,0,0);
//	glVertex3f(v1.x, v1.y, v1.z);
//	glVertex3f(v2.x, v2.y, v2.z);
//	glVertex3f(v2.x, v1.y+0.5, v1.z);
//	glVertex3f(v1.x, v1.y+0.5, v1.z);
//	glEnd();
//}

//void Tile::add_vert(vert_3f v)
//{	
//	this->vertices.push_back(v);		
//}

//void Tile::calc_normal_vector()
//{
//	float v1_x = vertices[0].x - vertices[1].x;
//    float v1_y = vertices[0].y - vertices[1].y;
//    float v1_z = vertices[0].z - vertices[1].z;
//
//    float v2_x= -(vertices[2].x - vertices[1].x);
//    float v2_y= -(vertices[2].y - vertices[1].y);
//    float v2_z= -(vertices[2].z - vertices[1].z);
//
//	float v1[] = {v1_x, v1_y, v1_z};
//	float v2[] = {v2_x, v2_y, v2_z};
//	//float norm[3] = {0};
//    
//	norm_cross_prod(v1, v2, this->norm);
//	//cout << "Normal: " << norm[0] << ", " << norm[1] << ", " << norm[2] << endl;
//
//    /*normX = v1_y * v2_z -   v1_z * v2_y;
//    normY =  -( v1_x * v2_z - v1_z *v2_x);
//    normZ = (v1_x * v2_y - v1_y * v2_x);
//    
//    float length = (float)(sqrt((normX*normX ) + (normY * normY) + (normZ * normZ)));
//    normX /= length; 
//    normY /= length;
//    normZ /= length;*/
//	//cout << "Calculating normal vector..." << endl;
//}

//float Tile::calc_slope()
//{
//	int n = 0;
//	float slope;
//	float x,y;
//	vert_3f v1, v2;
//	std::vector<vert_3f> *verts; 
//
//	verts = get_vertices();
//	n = verts->size();
//
//	for(int i=0; i<n-1; i++)
//	{
//		v1 = verts->at(i);
//		v1 = verts->at(i+1);
//
//		if(abs(v1.y - v2.y) > 0)
//		{
//			y = abs(v1.y - v2.y);
//		}
//	}
//
//	slope = y/x;
//
//	return slope;
//}

