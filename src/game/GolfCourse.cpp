/* GolfCourse.cpp */
#include "GolfCourse.h"
#include "vector.h"
#include "physics.h"
#include <iostream>
#include <math.h>
using namespace std;

/*
===========================
GolfCourse
===========================
*/
GolfCourse::GolfCourse()
{
	
}

/*
===========================
GolfCourse
===========================
*/
GolfCourse::GolfCourse(std::vector<GolfHole *> _holes)
{
	holes = _holes;
}

/*
===========================
get_holes
===========================
*/
std::vector<GolfHole *> GolfCourse::get_holes()
{
	return this->holes;	
}

/*
===========================
set_holes
===========================
*/
void GolfCourse::set_holes(std::vector<GolfHole *> allHoles)
{
	holes = allHoles;
}

/*
===========================
draw
===========================
*/
void GolfCourse::draw()
{
	int numHoles = (int)holes.size();

	for(int i=0; i<numHoles; i++)
	{
		holes.at(i)->draw();
	}
}

/*
===========================
add_tile
===========================
*/
void GolfCourse::add_hole(GolfHole *newHole)
{
	holes.push_back(newHole);
}

void GolfCourse::add_external(ExternalObject *obj)
{
	objects.push_back(obj);
	//cout << "Current # of external objects" << objects.size() << endl;
}


