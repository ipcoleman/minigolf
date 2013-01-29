/* GolfCourse.h */
#ifndef GOLFCOURSE_H
#define GOLFCOURSE_H

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
#include "GolfHole.h"
#include "Tile.h"
#include "ExternalObject.h"



//typedef struct _Hole
//{
//	int par;
//} Hole;



class GolfCourse 
{
private:	
	std::string course_title;
	std::vector<GolfHole *> holes;	
	std::vector<ExternalObject *> objects;
public:
	GolfCourse();
	GolfCourse(std::vector<GolfHole *>);
	virtual void draw();	
	virtual std::vector<GolfHole *> get_holes();
	virtual void set_holes(std::vector<GolfHole *>);
	virtual void add_hole(GolfHole *);	
	virtual void add_external(ExternalObject *);
};

#endif