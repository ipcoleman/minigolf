#include <sstream>

/*
===========================
str_to_int

	Returns int version of
	string
===========================
*/
int str_to_int(std::string str)
{
	int i;

	i = atoi(str.c_str());

	return i;
}

/*
===========================
str_to_float

	Returns float version of
	string
===========================
*/
float str_to_float(std::string str)
{
	float f;

	f = atof(str.c_str());
	
	return f;
}

/*
===========================
int_to_str

	Returns string version of
	int value
===========================
*/
std::string int_to_str(int i)
{	
	std::stringstream out;
	out << i;
	return out.str();	
}

/*
===========================
float_to_str

	Returns string version of
	float value
===========================
*/
std::string float_to_str(float f)
{
	std::stringstream out;
	out << f;
	return out.str();
}

// replaces newline in a string with null character
void trim_str(char *str)
{
	char *newline; // pointer to newline in str

	// get pointer to newline
	newline = strchr(str, '\n');
	// set value to null char
	if(newline)
		*newline = '\0';
}