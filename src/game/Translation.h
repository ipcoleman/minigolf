#ifndef TRANSLATION_H
#define TRANSLATION_H

#include "Transformation.h"

class Translation : public Transformation
{
private:
	float x, y, z;
	float x_per_tick, y_per_tick, z_per_tick;
	float x_tot, y_tot, z_tot;

	/*int angle;	
	int angle_per_tick;
	int angle_rot;
	int axis_x, axis_y, axis_z;*/
public:
	Translation(float, float, float);
	virtual float get_x();
	virtual float get_y();
	virtual float get_z();
	virtual float get_xpt();
	virtual float get_ypt();
	virtual float get_zpt();
	virtual void calc_change_per_tick();
	virtual void add_total_trans(float, float, float);
	virtual void transform();
	virtual void transform(int);
};

#endif