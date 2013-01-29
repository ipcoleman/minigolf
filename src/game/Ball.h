/* Ball.h */
#ifndef BALL_H
#define BALL_H

#ifdef _M_IX86
#include <windows.h>
#else
#include <iostream>
#endif

#include <string>
#include <vector>
#include <GL/glut.h>
#include "Graphics.h"
#include "GolfCourse.h"
#include "GolfHole.h"
#include "Tile.h"
#include "physics.h"
#include "Polygon.h"


class Ball 
{
private:
	float x,y,z;	
	float radius;
	float RGBA[4];
	float vel[3];
	float accel;
	//GolfCourse *course;
	GolfHole *hole;
	Tile *currentTile;
	float surface_tension;
	int status;	

public:
	enum Status{STOPPED=1, HIT=2, IN_MOTION=4, WAITING_TO_HIT=8 };
	Ball();
	Ball(float, float, float); // x,y,z
	Ball(float, float, float, float); //x,y,z,radius	
	Ball(float, float, float, float, Tile *); // x,y,z,radius,color, tile
	virtual float get_x();
	virtual float get_y();
	virtual float get_z();
	virtual void set_x(float);
	virtual void set_y(float);
	virtual void set_z(float);
	virtual int get_status();
	virtual void set_status(int);
	virtual void add_status(int);
	virtual void clear_status();
	virtual float* get_vel();
	virtual float* get_color();
	virtual void set_color(float *);
	virtual float get_surface_tension();
	virtual float* get_coordinates();
	virtual GolfHole* get_hole();
	virtual void set_hole(GolfHole *);
	virtual Tile* get_tile();
	virtual void set_tile(Tile *); 
	virtual int get_tile_id();
	virtual void draw();
	virtual void hitv(float *);
	virtual void hit(float, float, float);
	virtual void move();
	virtual void apply_grav();
	virtual void apply_surface_resistance(float);
	virtual bool on_cup_tile();
	virtual float* get_dir_from_mouse(int, int, float*);
	virtual bool is_stopped();
	virtual bool on_tile(Tile *);
	virtual void check_poly_collision(minigolf::Polygon *);
	//virtual void check_border_collision(std::vector<Tile> *);
	virtual void check_border_collision(std::vector<minigolf::Polygon *>);
	virtual void check_object_collision(std::vector<ExternalObject *>);
	virtual float* get_bounce_dir(float *);
	virtual bool in_cup();
	virtual void vertex_trace();
	virtual bool stationary();
	virtual void stop();
	virtual bool finished_with_stroke();
	virtual void set_finished_with_stroke();
	virtual void move_away_from_hole();
	

};

#endif