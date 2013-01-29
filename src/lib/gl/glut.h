#ifndef GLUT_H
#define GLUT_H

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include <string>

/*
===========================
menu options - 
transformation, lighting, quit
===========================
*/ 
enum
{
	NEW_GAME,
	TRANSLATE,
	ZOOM,
	ROTATE_X, 
	ROTATE_Y, 
	LIGHT_ON,
	LIGHT_OFF,
	PLAY,
	QUIT
};

/*
===========================
mouse state
===========================
*/
extern int btn_state[3]; // state of mouse button

/*
===========================
translation helpers
===========================
*/
extern int cur_menu_mode; // translate is default
extern int mouse_x, mouse_y;  // current mouse position
extern float rot_mat[ 16 ]; // Current composite rotation matrix 
extern float translate[ 3 ];	// Current translation values

/*
===========================
 function declarations
===========================
*/
void reshape(int, int);
//void idle();
//void handle_keyboard(unsigned char, int, int);
void handle_mouse(int, int, int, int);
void handle_menu(int);
void handle_transform( int, int);
//void handle_timer(int);
void draw_string(float, float, float, std::string);

#endif