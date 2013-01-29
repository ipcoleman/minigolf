#ifndef EXTERNALOBJECT_H
#define EXTERNALOBJECT_H

//#ifdef _M_IX86
//#include <windows.h>
//#else
//#include <iostream>
//#endif

#include <vector>
#include <iostream>
#include "Polygon.h"
#include "Transformation.h"
#include "Translation.h"
#include "Rotation.h"

using namespace std;


class ExternalObject
{
private:
	std::vector<minigolf::Polygon *> polys;
	//std::vector<Transformation *> trans;
	std::vector<Translation *> translations;
	std::vector<Rotation *> rotations;
	std::vector<vert_3f> bbox_verts;	
	std::vector<int> bbox_tile_ids;
	std::vector<minigolf::Polygon *> bbox_polys;
	int total_ticks;
	int prev_change_tick;
	int newest_change_tick;
	bool dynamic;
	bool identity;
	//int angle_rot;
public:
	ExternalObject();	
	ExternalObject(std::vector<minigolf::Polygon *>);
	virtual std::vector<minigolf::Polygon *> get_polys();
	//virtual std::vector<Transformation *> get_trans();
	//std::ostream& operator<<(std::ostream &, ExternalObject &);
	virtual void add_bbox_vert(vert_3f);
	virtual void add_bbox_tile_id(int);
	virtual std::vector<int> get_bbox_tile_ids();
	virtual void add_poly(minigolf::Polygon *);
	//virtual void add_trans(Transformation *);
	virtual void add_translation(Translation *);
	virtual void add_rotation(Rotation *);	
	virtual void add_bbox_poly(minigolf::Polygon *);
	virtual void transform_bbox_polys(float *);
	virtual std::vector<minigolf::Polygon *> get_bbox_polys();
	virtual void build_bbox_polys();
	virtual void draw();	
	//virtual void transform();
	virtual void transform_static();
	virtual void transform_dynamic();
	virtual void translate();
	virtual void translate_static();
	virtual void translate_dynamic();
	virtual void rotate();
	virtual void rotate_static();
	virtual void rotate_dynamic();
	virtual int get_trans_cycle_end();
	virtual int get_ticks();
	virtual void incr_ticks();
	virtual void reset_ticks();
	virtual void toggle_dynamic();
	virtual void toggle_identity();
	//virtual void transform(int);
	
	//virtual void handle_timer(int);
	//virtual void add_angle_rot(int);
};

//std::ostream& operator<<(std::ostream &stream, ExternalObject &eo)
//{
//	stream << "Poly Count: " << eo.get_polys().size() << endl;
//	stream << "Transform Count: " << eo.get_trans().size() << endl;
//
//	return stream;
//}

#endif