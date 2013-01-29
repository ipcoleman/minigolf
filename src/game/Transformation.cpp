#include "Transformation.h"

int Transformation::get_type()
{
	return type;
}

int Transformation::get_change()
{
	return change;
}

void Transformation::set_change(int ch)
{
	change = ch;
}

int Transformation::get_begin()
{
	return begin;
}

void Transformation::set_begin(int beg)
{
	begin = beg;
}

int Transformation::get_complete()
{
	return complete;
}

void Transformation::set_complete(int comp)
{
	complete = comp;
}

bool Transformation::get_active()
{
	return active;
}

void Transformation::activate()
{
	active = true;
}

void Transformation::deactivate()
{
	active = false;
}