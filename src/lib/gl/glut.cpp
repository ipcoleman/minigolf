#ifdef _M_IX86
#include <windows.h>
#else
#include <iostream>
#endif

#include "glut.h"
#include <GL/glut.h>
#include <math.h>
#include <string>


#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

extern int window_id;

/*
===========================
mouse state
===========================
*/ 
int btn_state[3] = {-1}; // state of mouse button

/*
===========================
translation helpers
===========================
*/
int cur_menu_mode = -1; // nothing is default
int mouse_x, mouse_y;  // current mouse position

float  rot_mat[ 16 ] = // Current composite rotation matrix 
{	
	1, 0, 0, 0, 
	0, 1, 0, 0, 
	0, 0, 1, 0, 
	0, 0, 0, 1	
};
float  translate[ 3 ] = { 0 };	// Current translation values

/*
===========================
reshape
===========================
*/
void reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	//gluOrtho2D(0, width, 0, height);

	/*glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    glMatrixMode(GL_MODELVIEW);*/
}




///*
//===========================
//handle_keyboard
//===========================
//*/
//void handle_keyboard(unsigned char key, int x, int y)
//{
//	switch(key)
//	{
//	case 27: // ESCAPE
//		exit(0); 
//	case GLUT_KEY_F1:
//		current_ball->stop();
//		break;
//	default:
//		break;
//	}
//}

/*
===========================
handle_menu
===========================
*/
void handle_menu( int ID )
{
	if(QUIT == ID)
		exit(0);
	cur_menu_mode = ID;
}

/*
===========================
handle_projection

	Handles mouse events
	x: mouse x pos
	y: mouse y pos
===========================
*/
void handle_transform( int x, int y )
{
  float	 x_ratchet;			// X ratchet value
  float	 y_ratchet;			// Y ratchet value


  float  ang;
  float  cos_ang;
  float  local_rot_mat[ 16 ] = 
  {
    1, 0, 0, 0, 
	0, 1, 0, 0, 
	0, 0, 1, 0, 
	0, 0, 0, 1
  };
  float  sin_ang;


  if ( !btn_state[ 0 ] ) // Left button not depressed?
  {			
    return;
  }

  x_ratchet = glutGet( GLUT_WINDOW_WIDTH ) / 10.0;
  y_ratchet = glutGet( GLUT_WINDOW_HEIGHT ) / 10.0;

  //  Windows XP has y = 0 at top, GL has y = 0 at bottom, so reverse y

  y = glutGet( GLUT_WINDOW_HEIGHT ) - y;

  //  If rotating, build sin and cosine angles for rotation matrix

	if ( cur_menu_mode == ROTATE_X || cur_menu_mode == ROTATE_Y ) 
	{
		x_ratchet /= 10.0;

		ang = float( mouse_x - x ) / x_ratchet;
		cos_ang = cos( ang / 180.0 * M_PI );
		sin_ang = sin( ang / 180.0 * M_PI );
	}

	switch( cur_menu_mode ) {
		case TRANSLATE:			// XY translation
			translate[ 0 ] += (float) ( x - mouse_x ) / x_ratchet;
			translate[ 2 ] += (float) -( y - mouse_y ) / y_ratchet;	
			break;	

		// !!!!!!!!!!
		// FIX THIS
		// !!!!!!!!!!
		case ZOOM:
			translate[1] += (float) ( x - mouse_x ) / x_ratchet;
			break;

		case ROTATE_X:			// X rotation
			local_rot_mat[ 5 ] = cos_ang;	// Build X-axis rotation matrix
			local_rot_mat[ 6 ] = -sin_ang;
			local_rot_mat[ 9 ] = sin_ang;
			local_rot_mat[ 10 ] = cos_ang;
			break;

		case ROTATE_Y:			// Y rotation
			local_rot_mat[ 0 ] = cos_ang;	// Build Y-axis rotation matrix
			local_rot_mat[ 2 ] = sin_ang;
			local_rot_mat[ 8 ] = -sin_ang;
			local_rot_mat[ 10 ] = cos_ang;
			glMatrixMode( GL_MODELVIEW );
			break;

		//case ROTATE_Z:			// Z rotation
		//  local_rot_mat[ 0 ] = cos_ang;	// Build Z-axis rotation matrix
		//  local_rot_mat[ 1 ] = -sin_ang;
		//  local_rot_mat[ 4 ] = sin_ang;
		//  local_rot_mat[ 5 ] = cos_ang;
		//  break;
  }

	// Post-multiple new rotation matrix against current rotation

	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	glLoadMatrixf( rot_mat );
	glMultMatrixf( local_rot_mat );
	glGetFloatv( GL_MODELVIEW_MATRIX, rot_mat );
	glPopMatrix();

	mouse_x = x;				// Update cursor position
	mouse_y = y;

	glutPostRedisplay();
}

/*
===========================
handle_mouse
===========================
*/
void handle_mouse( int b, int s, int x, int y )

  //  This routine acts as a callback for GLUT mouse events
  //
  //  b:     Mouse button (left, middle, or right)
  //  s:     State (button down or button up)
  //  x, y:  Cursor position
{
  if ( s == GLUT_DOWN ) {		// Store button state if mouse down
    btn_state[ b ] = 1;
  } else {
    btn_state[ b ] = 0;
  }

  if ( s == GLUT_UP ) {		// Store button state if mouse up
    btn_state[ b ] = 0;
  } else {
    btn_state[ b ] = 1;
  }

  mouse_x = x;
  mouse_y = glutGet( GLUT_WINDOW_HEIGHT ) - y;
}

//void handle_timer(int data)
//{
//	// not sure what to do here...
//}

void draw_string( float x, float y, float z, std::string txt )

  //  This routine draws the text string at the given (x,y,z) position
  //
  //  x,y,z:  Raster position for text
  //  txt:    String to draw
{
  glRasterPos3f( x, y, z );
  for(int i=0; i<txt.length(); i++)
  {
	  glutBitmapCharacter( GLUT_BITMAP_9_BY_15, txt.at(i) );
  }
  /*while( *txt != '\0' ) {
    glutBitmapCharacter( GLUT_BITMAP_9_BY_15, *txt );
    txt++;
  }*/
}