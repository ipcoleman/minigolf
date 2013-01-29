#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

class Transformation
{
protected:
	int type;
	int change;
	int begin;
	int complete;
	bool active;
	int cycle_time;
public:
	enum TransformType {TRANSLATE, ROTATE};
	enum ChangeType {STATIC, DYNAMIC};
	virtual int get_type();
	virtual int get_change();
	virtual void set_change(int);
	virtual int get_begin();
	virtual void set_begin(int);
	virtual int get_complete();
	virtual void set_complete(int);
	virtual bool get_active();
	virtual void activate();
	virtual void deactivate();
	virtual void transform() = 0;
	virtual void transform(int) = 0;
	virtual void calc_change_per_tick() = 0;
};

#endif
