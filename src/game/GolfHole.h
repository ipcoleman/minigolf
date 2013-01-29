#ifndef HOLE_H
#define HOLE_H

//#include "Golf.h"
#include "Tile.h"

class GolfHole
{
private:
	std::string title;
	int par;
	std::vector<Tile> tiles;
	//Tile *holeTile;
	int cup_tile_id;
	//std::vector<GolfHole> holes;
	vert_3f tee;
	vert_3f cup;
	bool is_finished;
public:
	GolfHole();
	GolfHole(std::vector<Tile>);
	virtual std::string get_title();
	virtual void set_title(std::string);
	virtual Tile* get_tile(int t_id);
	virtual int get_par();
	virtual void set_par(int);
	virtual int get_cup_tile_id();
	virtual void set_cup_tile_id(int);
	virtual void set_tee(vert_3f);
	virtual vert_3f get_cup();
	virtual void set_cup(vert_3f);
	virtual bool finished();
	virtual void set_finished();
	virtual void draw();
	virtual void draw_tee();
	virtual void draw_cup();	
	virtual std::vector<Tile>* get_tiles();
	virtual void set_tiles(std::vector<Tile>);
	virtual void add_tile(Tile *);	

};

#endif