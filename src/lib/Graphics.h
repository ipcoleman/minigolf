#ifndef GRAPHICS_H
#define GRAPHICS_H

//typedef int vert_2i[2];
//typedef int vert_3i[3];
//typedef float vert_2f[2];
//typedef float vert_3f[3];

typedef struct _vert_2i
{
	int x,y;
	//unsigned char r,g,b,a;
} vert_2i;

typedef struct _vert_3i
{
	int x,y,z;
	//unsigned char r,g,b,a;
} vert_3i;

typedef struct _vert_2f
{
	float x,y;
	//unsigned char r,g,b,a;
} vert_2f;

typedef struct _vert_3f
{
	float x,y,z;
	//unsigned char r,g,b,a;
} vert_3f;

typedef struct _RGBA
{
	float r, g, b, a;

} RGBA;

//class Point
//{
//private:
//	float x,y,z;
//public:
//	void draw();
//};

#endif