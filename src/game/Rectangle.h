#ifndef RECTANGLE_H
#define RECTANGLE_H

//#include <vector>
#include "Graphics.h"

namespace minigolf 
{
	class Rectangle
	{
	private:	
		RGBA color;
		vert_3f vert;
	public:
		Rectangle();
		Rectangle(vert_3f);
		virtual void draw();	
	};
}

#endif