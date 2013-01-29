/* Tile.h */

#ifndef TILE_H
#define TILE_H

#ifdef _M_IX86
#include <windows.h>
#else
#include <iostream>
#endif

#include <string>
#include <iostream>
#include <vector>
#include <GL/glut.h>
#include "Graphics.h"
#include "Polygon.h"

//using namespace minigolf;

class Tile : public minigolf::Polygon
{
private:
	int id;
	//std::vector<vert_3f> vertices;
	std::vector<int> borderTileIDs;
	std::vector<minigolf::Polygon *> tile_borders;
	//RGBA color;
	//float norm[3];		

public:
	Tile();
	Tile(std::vector<vert_3f>);
	Tile(std::vector<vert_3f>, int);
	Tile(vert_3f, vert_3f, vert_3f, vert_3f);
	Tile(vert_3f, vert_3f, vert_3f, vert_3f, RGBA);
	int get_id();
	void set_id(int);
	std::vector<vert_3f> get_vertices();
	std::vector<int> get_border_tiles();
	void add_border_tile_id(int);
	void add_tile_border(minigolf::Polygon *);
	std::vector<minigolf::Polygon *> get_tile_borders();
	//float* get_normal();
	void draw();
	//virtual void add_vert(vert_3f);
	//virtual void calc_normal_vector();
	//virtual float calc_slope();	

	/*virtual std::vector<vert_3f>* get_vertices();
	float* get_normal();
	virtual void add_vert(vert_3f);
	virtual void calc_normal_vector();		
	virtual float calc_slope();	
	virtual void draw();*/
};

#endif
