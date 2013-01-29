#ifndef ROTATION_H
#define ROTATION_H

class Rotation : public Transformation
{
private:
	int angle;	
	int angle_per_tick;
	int angle_rot;
	int axis_x, axis_y, axis_z;
public:
	Rotation(int, int, int, int);
	virtual int get_angle_per_tick();
	virtual void calc_change_per_tick();
	virtual void add_angle_rot(int);
	//void angle_plus(int);
	virtual void transform();
	virtual void transform(int);
};

#endif