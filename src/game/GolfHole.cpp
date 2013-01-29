#include "GolfHole.h"
#include "vector.h"
#include <math.h>
//#include <iostream>
using namespace std;

/*
===========================
GolfHole
===========================
*/
GolfHole::GolfHole()
{
	cout << "Creating default GolfHole" << endl;
	title = "";
	par = -1;
	is_finished = false;
}

/*
===========================
GolfCourse
===========================
*/
GolfHole::GolfHole(std::vector<Tile> _tiles)
{
	title = "";
	par = -1;
	tiles = _tiles;
	is_finished = false;
}

/*
===========================
get_title
===========================
*/
std::string GolfHole::get_title()
{
	return title;
}

/*
===========================
set_title
===========================
*/
void GolfHole::set_title(std::string t)
{
	title = t;
}

/*
===========================
get_par
===========================
*/
int GolfHole::get_par()
{
	return par;
}

/*
===========================
set_par
===========================
*/
void GolfHole::set_par(int p)
{
	par = p;
}

/*
===========================
get_tile
===========================
*/
Tile* GolfHole::get_tile(int t_id)
{
	return &tiles.at(t_id - 1);
}

/*
===========================
get_cup_tile_id
===========================
*/
int GolfHole::get_cup_tile_id()
{
	return cup_tile_id;
}

/*
===========================
set_cup_tile_id
===========================
*/
void GolfHole::set_cup_tile_id(int ct_id)
{	
	if(ct_id >= 0 && tiles.size() > 0)
		cup_tile_id = ct_id;
}

/*
===========================
set_tee
===========================
*/
void GolfHole::set_tee(vert_3f t)
{
	tee = t;
}

/*
===========================
get_cup
===========================
*/
vert_3f GolfHole::get_cup()
{
	return cup;
}

/*
===========================
set_cup
===========================
*/
void GolfHole::set_cup(vert_3f c)
{
	cup = c;
}

/*
===========================
finished
===========================
*/
bool GolfHole::finished()
{
	return is_finished;
}


/*
===========================
set_finished
===========================
*/
void GolfHole::set_finished()
{
	is_finished = true;;
}

/*
===========================
get_tiles
===========================
*/
std::vector<Tile>* GolfHole::get_tiles()
{
	return &this->tiles;	
}

/*
===========================
set_tiles
===========================
*/
void GolfHole::set_tiles(std::vector<Tile> allTiles)
{
	tiles = allTiles;
}

/*
===========================
draw
===========================
*/
void GolfHole::draw()
{
	int numTiles = (int)tiles.size();

	for(int i=0; i<numTiles; i++)
	{
		tiles.at(i).draw();
	}

	draw_tee();
	draw_cup();
}

/*
===========================
draw_tee
===========================
*/
void GolfHole::draw_tee()
{
	float x,y,z;
	x = tee.x;
	y = tee.y;
	z = tee.z;
	GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat mat_diffuse[] = {1.0, 1.0, 1.0, 1.0}; 

	// get normal for tee
	float tee_norm[3];	
	vert_3f v1 = {x,y,z};
	vert_3f v2 = {x,y,z+0.1};
	vert_3f v3 = {x+0.1,y,z};
	calculate_normal(v1, v2, v3, tee_norm);

	glPushMatrix();
	glBegin(GL_QUADS);
	glColor3f(0,0,0.5);	
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); 
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse); 
	glNormal3fv(tee_norm);
	glVertex3f(x,y,z);
	glVertex3f(x,y,z+0.1);
	glVertex3f(x+0.1,y,z+0.1);
	glVertex3f(x+0.1,y,z);		
	glEnd();
	glPopMatrix();

	
}

/*
===========================
draw_cup
===========================
*/
void GolfHole::draw_cup()
{
	float x,y,z;

	x = cup.x;
	y = cup.y;
	z = cup.z;

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
    glColor3f(0.0f, 0.0f, 0.0f); // black

    int i;
    int sections = 20; //number of triangles to use to estimate a circle
                        // (a higher number yields a more perfect circle)
    GLfloat radius = 0.05f; //radius
    GLfloat twoPi = 2.0f * 3.14159f;

    glBegin(GL_TRIANGLE_FAN);	
	glTranslatef(x,y,z);
            glVertex3f(x, y, z); // origin			
            for(i = 0; i <= sections;i++) { // make $section number of circles
                    glVertex3f(x + radius * cos(i *  twoPi / sections), y,
                            z + radius* sin(i * twoPi / sections));
					//glTranslatef(x, y, z);
            }
    glEnd();
	glPopMatrix();
    //glutSwapBuffers();


}

/*
===========================
add_tile
===========================
*/
void GolfHole::add_tile(Tile *newTile)
{
	tiles.push_back(*newTile);
}