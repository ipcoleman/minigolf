#ifndef POLYGON_H
#define POLYGON_H

#include <vector>
#include "Graphics.h"


namespace minigolf
{
	class Polygon
	{
	protected:					
		std::vector<vert_3f> vertices;
		RGBA color;
		float norm[3];
	public:
		Polygon();
		Polygon(std::vector<vert_3f>);
		virtual std::vector<vert_3f> get_vertices();
		virtual std::vector<vert_3f>* get_verts();
		virtual RGBA get_color();
		virtual void set_color(RGBA);		
		virtual float* get_normal();
		virtual void add_vert(vert_3f);
		virtual void clear_verts();
		virtual void calc_normal_vector();		
		virtual float calc_slope();	
		virtual void draw();	
	};
}

#endif