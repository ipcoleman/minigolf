#include "net.h"
#include "Client.h" /// needs to be included before windows.h 
#include "Server.h"
//(http://stackoverflow.com/questions/1372480/c-redefinition-header-files)

#ifdef _M_IX86
#include <windows.h>
#else
#include <iostream>
#endif

#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <iterator>
#include <math.h>
#include <GL/glui.h>
#include <GL/glut.h>

#include "GolfGame.h"
#include "GolfCourse.h"
#include "GolfHole.h"
#include "Tile.h"
#include "ExternalObject.h"
#include "Polygon.h"
#include "Graphics.h"
#include "Ball.h"
#include "Player.h"
#include "Transformation.h"
#include "Translation.h"
#include "Rotation.h"
#include "physics.h"
#include "glut.h"
//#include "glui.h"
#include "vector.h"
#include "convert.h"
#include "operators.h"



using namespace std;
//using namespace minigolf;

/* window size */
#define WIDTH 800
#define HEIGHT 600
//#define MAX_PLAYER_COUNT 4
#define DEFAULT 1
#define CUSTOM 2
//#define CLIENT 1
//#define SERVER 2
//#define CLIENT_DEF 3
//#define SERVER_DEF 4


//------------------
// File I/O
//------------------
string inputFileName = ""; // stores name of input file from cl args
static vector<string> inputLines; // stores each line from input file
vector<std::string> high_score_lines;
//------------------
// Course objects
//------------------
GolfGame *game = new GolfGame("", NULL, GolfGame::PREGAME);
GolfCourse *course = new GolfCourse();
GolfHole *current_hole = new GolfHole();
GolfHole *new_hole = new GolfHole();
Tile *tile;
minigolf::Polygon *current_poly;
ExternalObject *current_object = new ExternalObject();
Player *player1, *player2, *player3, *player4;
Player *current_player;
Ball* balls[MAX_PLAYER_COUNT];
Player* players[MAX_PLAYER_COUNT];
GolfHole* holes[MAX_HOLE_COUNT]; 
Ball *ball1, *ball2, *ball3, *ball4;
Ball *current_ball;
std::vector<ExternalObject *> all_objects;
std::vector<Transformation *> all_trans;
int current_tile_id = -1;

float ball_v = 0.01;
float accel = 0.00001;
std::string course_title = "";
vert_3f tee_vert;
vert_3f cup_vert;
float glCoord_x;
float glCoord_z; // viewplane coordinates of mouse

//------------------
// border polys
//------------------
//std::vector<Tile> tileBorders;
std::vector<minigolf::Polygon *> tileBorders;
minigolf::Polygon *border; // 

//------------------
// File I/O variables
//------------------
int buffSize = 0;
char inputFileLine[255];
string strInputLine = "";	
ifstream inputFile;	
std::vector<string> inputTokens;	
string keyword; 
string tile_id;
int tile_numEdges;	
int poly_numEdges;
int currentTokenIndex = 0;
std::vector<vert_3f> tempVerts;	
vector<int> borderTileIDs;

//------------------
// GLUI objects
//------------------
GLUI *glui;
GLUI *high_score_glui;
GLUI *scoreboard_glui;
GLUI *hole_descr_glui;
GLUI_Panel *panel;
GLUI_FileBrowser *browser;
GLUI_EditText *txt_player_count;
GLUI_EditText *txt_player_name1;
GLUI_EditText *txt_player_name2;
GLUI_EditText *txt_player_name3;
GLUI_EditText *txt_player_name4;
GLUI_FileBrowser *file_browser;
GLUI_Button *btn_start;
GLUI_Button *btn_quit;
GLUI_Button *btn_high_scores;
GLUI_Button *btn_start_host;
GLUI_Button *btn_start_client;
GLUI_RadioGroup *rb_group;
GLUI_RadioButton *rb_option1;
int window_id;
int high_scores_win_id;
int obj = 0;
int player_count = 0;
int hole_count = 0;
std::string player_name1 = "";
std::string player_name2 = "";
std::string player_name3 = "";
std::string player_name4 = "";
std::string file_name = "";
int current_player_strokes = 0;
std::string current_player_name;
int current_player_index = 0;
int current_hole_index = 0;
std::string current_hole_title;
int current_hole_par;
std::string player_scores_str = "";
int poly_count = 0;
int total_ticks = 0;

// Winsock variables
SOCKET ConnectSocket;
SOCKET ListenSocket;
SOCKET ClientSocket;
Client *client;
Server *server;

/*
===========================
forward function declarations
===========================
*/
void init(string, int, int);
void idle();
void get_input_lines(string, std::vector<string>*);
void load_course(string);
void load_players();
void load_balls();
void set_current_ball(Ball *);
void display();
void draw_scene();
void draw_border(float, float, float, float, float, float, float);
void draw_border_poly(vert_3f, vert_3f);
void draw_border_polys();
void draw_splash();
void light_scene();
void draw_tile(std::vector<vert_3f>, int);
void draw_tee(float, float, float);
void draw_cup(float, float, float);
void get_mouse_input();
void play();
// GLUI functions
void open_file_dialog(int, GLUI *);
void get_player_count(int);
void get_radio_selection(int);
void get_high_scores(std::string, vector<std::string> *);
void display_high_scores();
void display_scoreboard();
void display_hole_descr();
void close_high_scores(int);
void close_window(GLUI *);
void handle_keyboard(unsigned char, int, int);
void init_gui();
void handle_timer(int);
void handle_timer_old(int);
void handle_timer2(int);
void draw_loading();
void load_timers();
//void trim_str(char *);
void check_client_server();
void setup_client();
void setup_server();
void send_ball_state();
void draw_shot_arrow(float, float, float);



/*
===========================
init

	Set OpenGL display mode, 
	window size, polygon mode,
	window title
===========================
*/
void init(string title, int w, int h)
{		    
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(w, h);
	//glPolygonMode(GL_FRONT, GL_FILL);
	//glPolygonMode(GL_BACK, GL_FILL);
	glViewport(0, 0, w, h);
	//glFrustum(-5, 5, -5, 5, -5, 5);
    //gluOrtho2D(0, width, 0, height);

	//high_scores_win_id = glutCreateWindow("High Scores");
	window_id = glutCreateWindow(title.c_str());	
}

/*
===========================
idle
===========================
*/
void idle()
{
	if ( glutGetWindow() != window_id )
		glutSetWindow(window_id);
	glutPostRedisplay();	
}


/*
===========================
get_input_lines
	
	Returns a std::vector
	with each line from file
	as strings
===========================
*/
void get_input_lines(string fileName, std::vector<string> *stringBuffer)
{	
	int buffSize = 0;
	char inputFileLine[255];
	string strInputFileLine = "";
	ifstream inputFile;	

	//-------------------------
	// Golf Course File I/O
	//-------------------------
	if(inputFileName.length() == 0)
		inputFileName = "test_course.00.db"; // TESTING PURPOSES ONLY
	
	inputFile.open(inputFileName.c_str(), ios::in);
	buffSize = sizeof(inputFileLine)/sizeof(char);

	//TODO: maybe check for EOF
	while(inputFile.good())
	{
		inputFile.getline(inputFileLine, buffSize);		
		strInputFileLine = (string)inputFileLine;

		stringBuffer->push_back(strInputFileLine);
	}

	inputFile.close();	
	//cout << "Finished getting lines." << endl;
}

/*
===========================
load_course
	
	Loads all tiles, cup, tee, 
	and initial ball position
	from hole file
===========================
*/
void load_course(std::string fileName)
{	
	//cout << "Restting golf hole" << endl;
	course = new GolfCourse();
	current_hole = new GolfHole();
	new_hole = new GolfHole();
	ball1 = new Ball();
	char str[255];
	char *tok;
	char *next_tok;
	RGBA current_obj_color = {0,0,0,0};
	RGBA current_poly_color = {0,0,0,0};


	get_input_lines(fileName, &inputLines);		
	tileBorders.clear();
	
	for(int j=0; j<(int)inputLines.size(); j++)
	{						
		strInputLine = inputLines.at(j);
		inputTokens.clear(); // reset set of tokens for current line

		if(strInputLine.length() > 0)
		{
			strcpy_s(str, strInputLine.c_str());					
			tok = strtok_s(str," ", &next_tok);
			inputTokens.clear();
			while(tok)
			{
				inputTokens.push_back(tok);
				tok = strtok_s(NULL, " ", &next_tok);				
			}

			currentTokenIndex = 0; // reset token counter to beginning of line
			if(inputTokens.at(0).compare("course") == 0)
			{				
				int num_holes = 0;

				//-------------------------
				// scan for course title
				//-------------------------				
				currentTokenIndex++;
				course_title = "";
				if(inputTokens.at(currentTokenIndex).find_first_of('"') == 0)
				{					
					course_title.append(inputTokens.at(currentTokenIndex));
					course_title.append(" ");
					currentTokenIndex++;

					while(inputTokens.at(currentTokenIndex).find_last_of('"') >= string::npos)
					{
						course_title.append(inputTokens.at(currentTokenIndex));
						course_title.append(" ");
						currentTokenIndex++;
					}		
					
					course_title.append(inputTokens.at(currentTokenIndex));		
					course_title = course_title.substr(1, course_title.length()-2);										
				}
				else
				{
					cout << "Couldn't find course name." << endl;
				}	

				currentTokenIndex++;
				num_holes = str_to_int(inputTokens.at(currentTokenIndex));				
			} // Create each hole
			else if(inputTokens.at(0).compare("begin_hole") == 0)
			{
				// reset hole
				new_hole = new GolfHole();	
				//--------------
				// Get hole components
				// (tiles, cup, tee, borders)
				//--------------				
				j++; // move to next line					
				strInputLine = inputLines.at(j);
				strcpy_s(str, strInputLine.c_str());					
				tok = strtok_s(str," ", &next_tok);
				inputTokens.clear();
				while(tok)
				{
					inputTokens.push_back(tok);
					tok = strtok_s(NULL, " ", &next_tok);				
				}

				while(inputTokens.at(0).compare("end_hole") != 0)
				{					
					// clear temp vertex buffer		
					tempVerts.clear();
					borderTileIDs.clear();
					//---------
					// TILE
					//---------					
					currentTokenIndex = 0;					
					if(inputTokens.at(currentTokenIndex++).compare("tile") == 0)
					{									
						tile_id = inputTokens.at(currentTokenIndex++);			
						tile_numEdges = str_to_int(inputTokens.at(currentTokenIndex++));						
						// create & add vertices for tile
						for(int i=0; i<tile_numEdges; i++)
						{
							float x,y,z;					
							// get vertex coordinates
							x = str_to_float(inputTokens.at(currentTokenIndex++));
							y = str_to_float(inputTokens.at(currentTokenIndex++));
							z = str_to_float(inputTokens.at(currentTokenIndex++));
							vert_3f v1 = {x,y,z};
							// store vertex
							tempVerts.push_back(v1);
						}
											
						tile = new Tile(tempVerts, str_to_int(tile_id));												

						//--------------------------
						// DRAW BORDER
						//--------------------------
					
						// get border tile id's

						for(int i=0; i<tile_numEdges; i++)
						{							
							borderTileIDs.push_back(str_to_int(inputTokens.at(currentTokenIndex++)));								
							// add id to current tiles list of bordering tiles
							//tile->add_border_tile_id(str_to_int(inputTokens.at(currentTokenIndex++)));

						}
						for(int i=0; i<(int)borderTileIDs.size(); i++)
						{							
							float x1 = 0.0;
							float y1 = 0.0;
							float z1 = 0.0; 
							float x2 = 0.0; 
							float y2 = 0.0; 
							float z2 = 0.0;

							// add border tile id to list
							// in current tile
							tile->add_border_tile_id(borderTileIDs.at(i));
						
							if(borderTileIDs.at(i) == 0)
							{
								// start of border
								x1 = tempVerts.at(i).x;
								y1 = tempVerts.at(i).y;
								z1 = tempVerts.at(i).z;
								// end of border
								if(i == (int)borderTileIDs.size() - 1)
								{
									x2 = tempVerts.at(0).x;
									y2 = tempVerts.at(0).y;
									z2 = tempVerts.at(0).z;
								}
								else
								{
									x2 = tempVerts.at(i+1).x;
									y2 = tempVerts.at(i+1).y;
									z2 = tempVerts.at(i+1).z;
								}	

								vert_3f v1 = {x1, y1, z1};
								vert_3f v2 = {x2, y2, z2};	
								vert_3f v3 = {v2.x, v2.y+0.1, v2.z};	
								vert_3f v4 = {v1.x, v1.y+0.1, v1.z};				
																
								border  = new minigolf::Polygon();
								border->add_vert(v1);
								border->add_vert(v2);
								border->add_vert(v3);
								border->add_vert(v4);
								tileBorders.push_back(border);
								tile->add_tile_border(border);
								// Add tile to hole and...
								new_hole->add_tile(tile);	
							}								
						}				
					}
					//---------
					// TEE
					//---------
					else if(inputTokens.at(0).compare("tee") == 0)
					{									
						float x, y, z;
						int tile_id;
												
						tile_id = str_to_int(inputTokens.at(currentTokenIndex++));
						x = str_to_float(inputTokens.at(currentTokenIndex++));
						y = str_to_float(inputTokens.at(currentTokenIndex++));
						z = str_to_float(inputTokens.at(currentTokenIndex++));

						tee_vert.x = x;
						tee_vert.y = y;
						tee_vert.z = z;

						new_hole->set_tee(tee_vert);				
					}
					//---------
					// CUP
					//---------
					else if(inputTokens.at(0).compare("cup") == 0)
					{											
						float x, y, z;
						int cup_tile_id;			

						cup_tile_id = str_to_int(inputTokens.at(currentTokenIndex++));
						x = str_to_float(inputTokens.at(currentTokenIndex++));
						y = str_to_float(inputTokens.at(currentTokenIndex++));
						z = str_to_float(inputTokens.at(currentTokenIndex++));
					
						cup_vert.x = x;
						cup_vert.y = y;
						cup_vert.z = z;
					
						new_hole->set_cup(cup_vert);
						new_hole->set_cup_tile_id(cup_tile_id);																				
					} 

					//****************************


					//currentTokenIndex = 0;
					else if(inputTokens.at(0).compare("name") == 0)
					{
						std::string hole_title = "";
						currentTokenIndex++;
						if(inputTokens.at(currentTokenIndex).find_first_of('"') == 0)
						{						
							hole_title.append(inputTokens.at(currentTokenIndex));
							hole_title.append(" ");
							currentTokenIndex++;

							while(inputTokens.at(currentTokenIndex).find_last_of('"') >= string::npos)
							{
								hole_title.append(inputTokens.at(currentTokenIndex));
								hole_title.append(" ");
								currentTokenIndex++;
							}		
							
							hole_title.append(inputTokens.at(currentTokenIndex));		
							hole_title = hole_title.substr(1, hole_title.length()-2);
							
							//---------------------
							// Set name of current hole
							//---------------------
							new_hole->set_title(hole_title);
						}
						else
						{
							cout << "Couldn't find hole name." << endl;
						}
					}

					//currentTokenIndex = 0;
					else if(inputTokens.at(0).compare("par") == 0)
					{
						currentTokenIndex = 0; // TODO: centralize this
						int hole_par = 0;
						currentTokenIndex++;
						hole_par = str_to_int(inputTokens.at(currentTokenIndex));
						//---------------------
						// Set part of current hole
						//---------------------
						new_hole->set_par(hole_par);
					}
					
					else if(inputTokens.at(0).compare("begin_object") == 0)
					{
						current_object = new ExternalObject();

						j++; // move to next line					
						strInputLine = inputLines.at(j);
						strcpy_s(str, strInputLine.c_str());					
						tok = strtok_s(str, " ", &next_tok);
						inputTokens.clear();
						while(tok)
						{
							inputTokens.push_back(tok);
							tok = strtok_s(NULL, " ", &next_tok);				
						}
						// get external object polys
						while(inputTokens.at(0).compare("end_object") != 0)
						{
							currentTokenIndex = 0;	
							// POLYGON
							if(inputTokens.at(currentTokenIndex++).compare("poly") == 0)
							{
								tempVerts.clear();
								float x,y,z;	
								
								poly_numEdges = str_to_int(inputTokens.at(currentTokenIndex++));

								for(int i=0; i<poly_numEdges; i++)
								{
									x = str_to_float(inputTokens.at(currentTokenIndex++));
									y = str_to_float(inputTokens.at(currentTokenIndex++));
									z = str_to_float(inputTokens.at(currentTokenIndex++));
									vert_3f v1 = {x,y,z};
									// store vertex
									tempVerts.push_back(v1);																		
								}
								current_poly_color.r = str_to_float(inputTokens.at(currentTokenIndex++));
								current_poly_color.g = str_to_float(inputTokens.at(currentTokenIndex++));
								current_poly_color.b = str_to_float(inputTokens.at(currentTokenIndex++));

								current_poly = new minigolf::Polygon(tempVerts);
								current_poly->set_color(current_poly_color);								
								poly_count++;
								
								// add polygon to external object
								current_object->add_poly(current_poly);
							} // BOUNDING BOX
							else if(inputTokens.at(0).compare("bbox") == 0)
							{
								int bbox_edge_count;
								currentTokenIndex = 0;
								float bbox_x, bbox_y, bbox_z;
								

								currentTokenIndex++;
								bbox_edge_count = str_to_int(inputTokens.at(currentTokenIndex++));

								for(int i=0; i<bbox_edge_count; i++)
								{
									bbox_x = str_to_float(inputTokens.at(currentTokenIndex++));
									bbox_y = str_to_float(inputTokens.at(currentTokenIndex++));
									bbox_z = str_to_float(inputTokens.at(currentTokenIndex++));
									vert_3f bbox_vert = {bbox_x, bbox_y, bbox_z};
									current_object->add_bbox_vert(bbox_vert);
								}
							} // INTERSECTION TILE
							else if(inputTokens.at(0).compare("intersect") == 0)
							{
								currentTokenIndex = 0;
								int bbox_tile_id;

								currentTokenIndex++;
								bbox_tile_id = str_to_int(inputTokens.at(currentTokenIndex++));
								current_object->add_bbox_tile_id(bbox_tile_id);

							}// TRANSFORMATION
							else if(inputTokens.at(0).compare("transformation") == 0)
							{
								currentTokenIndex = 0;
								std::string change_type;
								std::string trans_type;
								float trans_x, trans_y, trans_z;
								float rot_angle;
								int rot_axis_x, rot_axis_y, rot_axis_z;	
								int begin, complete;
								Transformation *current_trans;
									
								currentTokenIndex++;
								change_type = inputTokens.at(currentTokenIndex++);
								trans_type = inputTokens.at(currentTokenIndex++);

								if(trans_type.compare("translate") == 0)
								{
									
									trans_x = str_to_float(inputTokens.at(currentTokenIndex++));
									trans_y = str_to_float(inputTokens.at(currentTokenIndex++));
									trans_z = str_to_float(inputTokens.at(currentTokenIndex++));									
			
									current_trans = new Translation(trans_x, trans_y, trans_z);									
								}
								else if(trans_type.compare("rotate") == 0)
								{
									rot_angle = str_to_int(inputTokens.at(currentTokenIndex++));
									rot_axis_x = str_to_int(inputTokens.at(currentTokenIndex++));
									rot_axis_y = str_to_int(inputTokens.at(currentTokenIndex++));
									rot_axis_z = str_to_int(inputTokens.at(currentTokenIndex++));
																		
									current_trans = new Rotation(rot_angle, rot_axis_x, rot_axis_y, rot_axis_z);									
								}
								// get beginning & completion for dynamic transformation
								if(change_type.compare("dynamic") == 0)
								{
									begin = str_to_int(inputTokens.at(currentTokenIndex++));
									complete = str_to_int(inputTokens.at(currentTokenIndex++));

									current_trans->set_change(Transformation::DYNAMIC);
									current_trans->set_begin(begin);
									current_trans->set_complete(complete);
									current_trans->calc_change_per_tick();
								}
								else // set to static
								{
									current_trans->set_change(Transformation::STATIC);									
								}
								//cout << "Printing object..." << endl;								
								//current_object->add_trans(current_trans);								
								if(current_trans->get_type() == Transformation::TRANSLATE)
									current_object->add_translation((Translation *)current_trans);
								else if(current_trans->get_type() == Transformation::ROTATE)
									current_object->add_rotation((Rotation *)current_trans);

								all_trans.push_back(current_trans);
							}

							currentTokenIndex = 0;	// reset to read start of line

							// next line
							j++; // move to next line					
							strInputLine = inputLines.at(j);
							strcpy_s(str, strInputLine.c_str());					
							tok = strtok_s(str, " ", &next_tok);
							inputTokens.clear();
							while(tok)
							{
								inputTokens.push_back(tok);
								tok = strtok_s(NULL, " ", &next_tok);				
							}
						}
						// add external object to course
						cout << "Adding object to course." << endl;
						course->add_external(current_object);
						current_object->build_bbox_polys();
						all_objects.push_back(current_object);						
					}

					j++; // move to next line					
					strInputLine = inputLines.at(j);
					strcpy_s(str, strInputLine.c_str());					
					tok = strtok_s(str, " ", &next_tok);
					inputTokens.clear();
					while(tok)
					{
						inputTokens.push_back(tok);
						tok = strtok_s(NULL, " ", &next_tok);				
					}
				} // end while: finished getting hole components	



				// Add hole to course
				// One of these 2 statements are redundant
				// but necessary (for now)
				course->add_hole(new_hole);					
				holes[hole_count] = new_hole;
				hole_count++;				
			}
		}
	}	
	//-------------------
	// AND FINALLY...
	// game course has been built,
	// set course
	//-------------------
	game->set_course(course);	
	

	//delete inputTokens;
	//delete inputLines;
}

/*
===========================
load_players
===========================
*/
void load_players()
{
	player_name1 = txt_player_name1->get_text();
	player_name2 = txt_player_name2->get_text();
	player_name3 = txt_player_name3->get_text();
	player_name4 = txt_player_name4->get_text();

	if( player_name1.length() > 0)
	{
		player1 = new Player(player_name1, NULL);
		players[0] = player1;
		current_player = players[current_player_index];
		current_player_name = current_player->get_name();
		game->add_player(player1);
	}
	if(player_name2.length() > 0)
	{
		player2 = new Player(player_name2, NULL);
		players[1] = player2;
		game->add_player(player2);
	}
	if(player_name3.length() > 0)
	{
		player3 = new Player(player_name3, NULL);
		players[2] = player3;
		game->add_player(player3);
	}
	if(player_name4.length() > 0)
	{
		player4 = new Player(player_name4, NULL);
		players[3] = player4;
		game->add_player(player4);
	}
}

/*
===========================
load_balls

Precondition:
- load_players() is called
===========================
*/
void load_balls()
{	
	float red[] = {1,0,0};
	float green[] = {0,1,0};	
	float blue[] = {0,0,1};
	float purple[] = {1,0,1};

	current_hole = holes[0];

	if(player_name1.length() > 0)
	{	
		ball1 = new Ball(tee_vert.x+0.05, tee_vert.y, tee_vert.z+0.05);
		ball1->set_color(red);
		ball1->set_hole(current_hole);
		ball1->set_status(Ball::WAITING_TO_HIT);
		balls[0] = ball1;
		player1->set_ball(ball1);
	}

	if(player_name2.length() > 0)
	{	
		ball2 = new Ball(tee_vert.x+0.05, tee_vert.y, tee_vert.z+0.05);
		ball2->set_color(green);
		ball2->set_hole(current_hole);
		ball2->set_status(Ball::WAITING_TO_HIT);
		balls[1] = ball2;
		player2->set_ball(ball2);
	}

	if(player_name3.length() > 0)
	{	
		ball3 = new Ball(tee_vert.x+0.05, tee_vert.y, tee_vert.z+0.05);
		ball3->set_color(blue);
		ball3->set_hole(current_hole);
		ball3->set_status(Ball::WAITING_TO_HIT);
		balls[2] = ball3;
		player3->set_ball(ball3);
	}

	if(player_name4.length() > 0)
	{	
		ball4 = new Ball(tee_vert.x+0.05, tee_vert.y, tee_vert.z+0.05);
		ball4->set_color(purple);
		ball4->set_hole(current_hole);
		ball4->set_status(Ball::WAITING_TO_HIT);
		balls[3] = ball4;
		player4->set_ball(ball4);	
	}

	// start w/ first ball
	current_ball = ball1;
}

/*
===========================
set_current_ball
===========================
*/
void set_current_ball(Ball *_ball)
{
	// redundant to set_current_player
}

/*
===========================
display
===========================
*/
void display()
{	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	glMatrixMode( GL_MODELVIEW );		// Setup model transformations	
	glPushMatrix();
	glTranslatef( translate[ 0 ], translate[ 1 ], translate[ 2 ] );	
	//glScalef(0,translate[ 1 ],0);
	glMultMatrixf( rot_mat );
	int game_status = -1;

	game_status = game->get_status();
	if(GolfGame::PREGAME == game_status)
	{
		draw_splash();
	}
	else if(GolfGame::LOADING == game_status)
	{
		draw_loading();
		glutPostRedisplay();
	}
	else if(GolfGame::IN_PROGRESS == game_status)
	{
		//-------------------
		// check if current player is
		// finished w/ current hole
		//-------------------
		if(game->finished_hole(current_player_index, current_hole_index))
		{
			//-------------------
			// check if all players
			// are finished w/ current hole
			//-------------------
			current_ball->clear_status();
			game->set_score(current_player_strokes, current_player_index, current_hole_index);


			if(game->finished_hole_all(current_hole_index, player_count))
			{
				//cout << "Hole is finished." << endl;
				//if(!game->finished())
				current_hole_index++;

				if(current_hole_index == hole_count)
				{
					cout << "Completed last hole" << endl;
					current_hole_index = 0;					
					game->set_status(GolfGame::FINISHED);
					display_scoreboard();
				}
				else
				{
					// load next hole
					current_hole = holes[current_hole_index];
					current_ball->set_x(tee_vert.x+0.05);
					current_ball->set_y(0);
					current_ball->set_z(tee_vert.z+0.05);
					// reset hole description glue					
					display_hole_descr();
					//hole_descr_glui = NULL;
				}
			}
		}

		if(current_player->finished_hole()) // DEPRECATED
		{
			game->set_score(current_player_strokes, current_player_index, current_hole_index);
			//---------------------
			// go to next hole
			//---------------------
			current_hole_index++;
			if(hole_count == current_hole_index)
			{
				game->set_status(GolfGame::FINISHED);
			}

			game->set_score(current_player->get_strokes() - current_hole->get_par(), current_player_index, current_hole_index);
			current_player->reset_strokes();


			scoreboard_glui->sync_live(); // reset player name, strokes			
		}

		if(current_ball->finished_with_stroke())
		{
			current_player_index++;
			if(player_count == current_player_index)
			{
				current_player_index = 0; // reset to first player
				//current_hole->set_finished();
			}
			//else
			//{
			// Get next player & ball
			current_player = players[current_player_index];
			current_player_name = current_player->get_name();	
			current_player_strokes = current_player->get_strokes();
			current_ball->clear_status();
			current_ball = balls[current_player_index];				
			//}
			//scoreboard_glui->sync_live(); // reset player name, strokes	
		}
		scoreboard_glui->sync_live(); // reset player name, strokes	
		//}
		
	}
	else if(GolfGame::FINISHED == game_status)
	{
		// 1. show score
		// 2. declare winner
		// 3. give option to restart
	}
	

	if(inputFileName.length() > 0)
	{
		// render polys
		draw_scene();
		//lighting
		light_scene();
	}
	
	// SERVER STUFF
	// if server is created
	if(server) 
	{		
		//listen_mouse_state();

		int client_ball_status;
		//bool client_mouse_up;
		int client_mouse_status;
		float *client_mouse_coord;
		float *client_ball_vel;
		
		client_ball_status = server->listen_ball_status();
		if(client_ball_status & Ball::WAITING_TO_HIT)
		{
			
			client_mouse_coord = server->listen_mouse_coord();
			draw_shot_arrow(client_mouse_coord[0], client_mouse_coord[1], client_mouse_coord[2]);
		}
		else if(client_ball_status & Ball::HIT)
		{
			client_ball_vel = server->listen_ball_vel();
			cout << "Hitting client's ball" << endl;
			current_ball->hit(client_ball_vel[0], client_ball_vel[1], client_ball_vel[2]);
			
		}
		else if(client_ball_status & Ball::IN_MOTION)
		{
		}
		else if(client_ball_status & Ball::STOPPED & Ball::HIT)
		{
			current_ball->stop();
			// send ball coordinates
			// sync ball coordinates
		}
	}
	// CLIENT STUFF
	if(client)
	{
		//send_ball_state();
		
		client->send_ball_status(current_ball->get_status());
		if(current_ball->get_status() & Ball::WAITING_TO_HIT)
		{
			client->send_mouse_coord(glCoord_x, 0, glCoord_z);
		}
		else if(current_ball->get_status() & Ball::HIT)
		{
			client->send_ball_vel(current_ball->get_vel()[0], current_ball->get_vel()[1], current_ball->get_vel()[2]);
		}
		else if(current_ball->get_status() & Ball::IN_MOTION)
		{
		}
		else if(current_ball->get_status() & Ball::STOPPED & Ball::HIT)
		{
			// send ball coordinates
			// sync ball coordinates
		}
	}

	glPopMatrix();
	glFlush();				// Flush OpenGL queue
	glutSwapBuffers();			// Display back buffer

}

/*
===========================
draw_scene
===========================
*/
void draw_scene()
{				
	Tile *current_ball_tile;
	std::vector<minigolf::Polygon *> current_ball_tile_borders;
	
	current_hole->draw();			
	current_object->draw();
	draw_border_polys();	
	//----------------------
	// Draw Ball
	//----------------------		
	if(current_ball)
	{
		current_ball_tile = current_ball->get_tile();
		current_ball_tile_borders = current_ball_tile->get_tile_borders();

		current_ball->apply_surface_resistance(0.0001);		
		current_ball->move();	
		//-----------------------
		// pass in set of polygons representing borders of
		// ball's current tile
		//-----------------------
		current_ball->check_border_collision(current_ball_tile_borders);	
		current_ball->check_object_collision(all_objects);		

		// check for ball in cup
		if(current_ball->in_cup())
		{
			current_ball->stop();
			current_ball->set_finished_with_stroke();
			game->finish_hole(current_player_index, current_hole_index);
			// hack to get it out of hole
			current_ball->move_away_from_hole();
		}
		else
		{
			current_ball->draw();
		}		
	}
	else
	{
		//cout << "Ball is null" << endl;
	}

	if(cur_menu_mode == PLAY)
		get_mouse_input();
	

}

/*
===========================
draw_border

	Just a line for now
===========================
*/
void draw_border(float x1, float y1, float z1, float x2, float y2, float z2, float thickness)
{
	glPushMatrix();
	glBegin(GL_LINES);
	glColor3f(1,1,1);
	glLineWidth(thickness);
	glVertex3f(x1, y1, z1);
	glVertex3f(x2, y2, z2);
	glEnd();
	glPopMatrix();
}

void draw_border_polys()
{
	//tileBorders
	float norm[3];
	vert_3f v1, v2, v3, v4;
			
	for(int i=0; i<(int)tileBorders.size(); i++)
	{
		v1 = tileBorders.at(i)->get_vertices().at(0);
		v2 = tileBorders.at(i)->get_vertices().at(1);
		v3 = tileBorders.at(i)->get_vertices().at(2);
		v4 = tileBorders.at(i)->get_vertices().at(3);
		// draw polygon border
		glBegin(GL_QUADS);
		//glColor3f(1,1,1);
		glColor3f(0.63, 0.32, 0.18);
		glNormal3fv(norm);
		glVertex3f(v1.x, v1.y, v1.z);
		glVertex3f(v2.x, v2.y, v2.z);
		glVertex3f(v3.x, v3.y, v3.z);
		glVertex3f(v4.x, v4.y, v4.z);
		glEnd();
	}	
}

/*
===========================
draw_splash
===========================
*/
void draw_splash()
{
	std::string name = "Minigolf";
	std::string author = "Ian Coleman";	
	int year = 2011;
	float versionNum = 1.0;	
	std::string version = "";

	version.append("Version ");
	version.append(float_to_str(versionNum));

	draw_string(-0.5, 0, -0.15, name);
	draw_string(-0.5, 0, 0, author);
	draw_string(-0.5, 0, 0.15, version);
	draw_string(-0.5, 0, 0.30, "Copyright 2011");
}

/*
===========================
draw_loading
===========================
*/
void draw_loading()
{
	std::string load_text = "Loading...";
	cout << "Drawing loading text" << endl;
	draw_string(-0.5, 0, 0, load_text);

}


/*
===========================
light_scene
===========================
*/
void light_scene()
{
	glEnable(GL_NORMALIZE);

	if(LIGHT_ON == cur_menu_mode)
	{
		glClearColor(0.0, 0.0, 0.0, 0.0);
		//GLfloat mat_specular[] = {0.0, 0.4, 0.3, 0.0};
		GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
		GLfloat mat_diffuse[] = {1.0, 1.0, 1.0, 1.0};
 		//GLfloat mat_shininess[] = {50.0};
		GLfloat light_position[] = {0.0, 1.0, -1.0, 1.0};
		//GLfloat white_light[] = {1.0, 1.0, 1.0, 1.0};
		//GLfloat green_light[] = {0.0, 0.45, 0.0, 0.0};
		GLfloat light[] = {0.2, 0.2, 0.2, 1.0};
		GLfloat lmodel_ambient[] = {0.0, 0.3, 0.2, 0.0};
		/*glClearColor(0.0, 0.0, 0.0, 0.0);*/
		glShadeModel(GL_SMOOTH);

		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		//glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, mat_diffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, light);
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_DEPTH_TEST);
	}
	else if(LIGHT_OFF == cur_menu_mode)
	{
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);
		glDisable(GL_DEPTH_TEST);
	}
}

/*
===========================
draw_tile
===========================
*/
void draw_tile(std::vector<vert_3f> vertices, int tile_id)
{
	Tile *t = new Tile(vertices);	
	t->set_id(tile_id);	
	/*if(ball->on_tile(t) )
	{
		cout << "Ball is on Tile " << t->get_id() << endl;
	}*/
	t->calc_normal_vector();
	t->draw();
	//course->add_tile(*t);
}

/*
===========================
draw_tee
===========================
*/
void draw_tee(float x, float y, float z)
{
	// get normal for tee
	float tee_norm[3];	
	vert_3f v1 = {x,y,z};
	vert_3f v2 = {x,y,z+0.1};
	vert_3f v3 = {x+0.1,y,z};
	calculate_normal(v1, v2, v3, tee_norm);


	glPushMatrix();
	glBegin(GL_QUADS);
	glColor3f(0,0,0.5);	
	glNormal3fv(tee_norm);
	glVertex3f(x,y,z);
	glVertex3f(x,y,z+0.1);
	glVertex3f(x+0.1,y,z+0.1);
	glVertex3f(x+0.1,y,z);		
	glEnd();
	glPopMatrix();
}

/*
===========================
draw_cup
===========================
*/
void draw_cup(float x, float y, float z)
{	
	Tile *holeTile = new Tile();
	vert_3f v1 = {x,y,z};
	vert_3f v2 = {x,y,z+0.1};
	vert_3f v3 = {x+0.1,y,z+0.1};
	vert_3f v4 = {x+0.1,y,z};
			
	float radius = 0.05;
	double da = M_PI/16;
	glPushMatrix();
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(0,0,0);
	glVertex2d(x, y);	 
	for(double a = 0.0; a <= 2 * M_PI; a += da) 
	{
		glVertex2d(x + cos(a) * radius, y + sin(a) * radius);
	}
	 
	glVertex2d( x + radius, y );
	glEnd();
	glPopMatrix();
	
	holeTile->add_vert(v1);
	holeTile->add_vert(v2);
	holeTile->add_vert(v3);
	holeTile->add_vert(v4);
}

/*
===========================
get_mouse_input
===========================
*/
void get_mouse_input()
{
	//float glCoord_x;
	//float glCoord_z; // viewplane coordinates of mouse
	float hit_v[3]; // initial velocity vector after hit

	glCoord_x = (((mouse_x - (float)(WIDTH/2))) / (float)(WIDTH/2)) * 3.0; // multiply by inverse of scale factor
	glCoord_z = (((float)(HEIGHT/2) - mouse_y) / (float)(HEIGHT/2)) * 3.0; // multiply by inverse of scale factor

	

	if(btn_state[0] == 1) // left mouse down
	{		
		// send mouse coordinates
		if(client)
		{
			//cout << "Sending mouse coordinates (" << glCoord_x << ", " << 0 << ", " << glCoord_z << ")" << endl;
			//client->send_ball_waiting_flag(false);
			//client->send_mouse_up_flag(false);
			//client->send_mouse_coord(glCoord_x, 0, glCoord_z);
		}
		//----------------------
		// draw line from ball 
		// to mouse pointer
		//----------------------
		glPushMatrix();
		glBegin(GL_LINES);
		glLineWidth(0.1);
		glColor3f(0,0,1);
		glVertex3f(current_ball->get_x(), current_ball->get_y(), current_ball->get_z());
		glVertex3f(glCoord_x, 0, glCoord_z);
		glEnd();
		glPopMatrix();
	}
	if(btn_state[0] == 0) // left mouse up
	{		
		// TODO: client should send a flag that the player is finishing making
		// hit selection 		
		hit_v[0] = current_ball->get_x() - glCoord_x;
		hit_v[1] = 0.0;
		hit_v[2] = current_ball->get_z() - glCoord_z;
		vector_mult(hit_v, 0.05);

		btn_state[0] = -1; // reset mouse down/up

		if(current_ball->stationary())
		{			
			current_ball->hitv(hit_v);
			current_player->add_stroke();
			current_player_strokes = current_player->get_strokes();
			//current_ball->set_status(Ball::HIT | Ball::IN_MOTION);
			current_ball->add_status(Ball::HIT);
			//current_ball->add_status(Ball::IN_MOTION);
			scoreboard_glui->sync_live();
		}		
	}
}


/*
===========================
play
===========================
*/
void play()
{
	cout << "Playing game." << endl;
	inputFileName = file_browser->get_file();		
	//inputFileName = "test_course.02.db";
	//cout << "File Name: " << inputFileName << endl;
	if(inputFileName.length() > 0)
	{
		game->set_status(GolfGame::LOADING);				
		load_course(inputFileName);	
		//load_timers();
		load_players();
		load_balls();
		current_hole = holes[current_hole_index];
		//set_current_player(player1);
		game->set_status(GolfGame::IN_PROGRESS);
		// COMMENTED TO PREVENT CLUTTER
		display_scoreboard();
		display_hole_descr();
	}
	else
	{
		cout << "Invalid file name" << endl;
	}
}
/*
===========================
get_player_count
===========================
*/
void get_player_count(int control)
{
	//cout << "Num Players: " << txt_player_count->get_int_val() << endl;
	//cout << "Num Players: " << player_count << endl;
}

void get_radio_selection(int control)
{
	//cout << "Getting radio selection..." << endl;
	//cout << "Option: " << obj << endl;
}


/*
===========================
control_callback.

Handles GLUI window callbacks

@params
control -- id of activated control
===========================
*/
void control_callback(int control)
{	
	//cout << "Control ID: " << control << endl;
	switch(control)
	{
	case 1: // Player Count
		cout << "Player Count: " <<  player_count << endl;
		break;
	case 6: // File Browser
		inputFileName = file_browser->get_file();				
		break;
	case 7: // Play Button						
		play();
		break;
	case 8:
		exit(0);
		break;
	case 9: // High Scores
		get_high_scores("high_scores.00.db", &high_score_lines);
		display_high_scores();
		break;
	case 10:
		//check_client_server();
		setup_server();
		break;
	case 11:
		setup_client();
		break;
	case 12:
		//close_window(hole_descr_glui);
		break;
	case 13:
		//close_window(scoreboard_glui);
		break;
	default:
		cout << "No action associated with control" << endl;
		break;
	}
}

void get_high_scores(std::string hs_file_name, vector<std::string> *hs_text)
{
	string strInputFileLine = "";

	//-------------------------
	// Golf Course High Score I/O
	//-------------------------
	if(hs_file_name.length() == 0)
		hs_file_name = "high_scores.00.db"; // TESTING PURPOSES ONLY
	
	inputFile.open(hs_file_name.c_str(), ios::in);
	buffSize = sizeof(inputFileLine)/sizeof(char);

	//TODO: maybe check for EOF
	while(inputFile.good())
	{
		inputFile.getline(inputFileLine, buffSize);		
		strInputFileLine = (std::string)inputFileLine;

		hs_text->push_back(strInputFileLine);
	}

	inputFile.close();
}

void display_high_scores()
{	
	GLUI_String high_scores = "";
	//GLUI_String newline = "\n";
	high_score_glui = GLUI_Master.create_glui("High Scores");
	GLUI_Panel *high_score_panel = new GLUI_Panel(high_score_glui, "", true);
	GLUI_TextBox *txt_high_scores;// = new GLUI_StaticText(high_score_panel, "HIGH SCORES");	
	GLUI_Button *btn_high_scores_close;
	std::string text;	
	
	if(high_score_lines.size() > 0)
	{
		for(int i=0; i<high_score_lines.size(); i++)
		{
			text.append(high_score_lines.at(i));
			text.append("\n");
		}
	}
	else
	{
		text = "No High Scores";
	}

	txt_high_scores = new GLUI_TextBox(high_score_panel, "");
	txt_high_scores->set_text(text.c_str());
	txt_high_scores->set_w(400);
	txt_high_scores->set_h(400);
	txt_high_scores->disable();
	btn_high_scores_close = new GLUI_Button(high_score_panel, "Close", -1, close_high_scores);
	//btn_high_scores_close = new GLUI_Button(high_score_panel, "Close", 14, control_callback);
	
	high_score_glui->set_main_gfx_window(window_id);
}

/*
===========================
display_scoreboard
===========================
*/
void display_scoreboard()
{
	//cout << "Displaying scoreboard" << endl;
	////int scores[][];

	////scores = game->get_scores();
	std::string player_score = "";
	std::string player_name = "";
	
	for(int i=0; i<player_count; i++)
	{		
		player_score = game->get_score(i);
		player_name.append("Player ");
		player_name.append(int_to_str(i+1));
		cout << "Current Score: " << player_score << endl;
		player_scores_str.append(player_score);
		player_scores_str.append(", ");

	}
	

	if(!scoreboard_glui)
	{
		scoreboard_glui = GLUI_Master.create_glui("Scoreboard");
		GLUI_Panel *scoreboard_panel = new GLUI_Panel(scoreboard_glui, "", true);
		GLUI_EditText *txt_score_line;// = new GLUI_EditText(scoreboard_panel, "", GLUI_EDITTEXT_STRING);

		GLUI_EditText *txt_score_header = scoreboard_glui->add_edittext_to_panel(scoreboard_panel, "", GLUI_EDITTEXT_STRING, &player_scores_str);
		GLUI_Button *btn_close_scoreboard = new GLUI_Button(scoreboard_panel, "Close", 13, control_callback);
		txt_score_header->set_text("H1    H2    H3    H4    H5    H6    H7    H8    H9    H10    H11    H12    H13    H14    H15    H16    H17    H18    ");
		txt_score_header->disable();
		txt_score_header->set_w(600);
		
		new GLUI_Column(scoreboard_panel, false);	
		scoreboard_glui->set_main_gfx_window(window_id);		
	}
	else
	{
		scoreboard_glui->sync_live();
	}
	
}

void display_hole_descr()
{
	//cout << "Displaying hole descr" << endl;
	std::string hole_title = "";
	int hole_par = -1;

	if(!hole_descr_glui)
	{
		hole_descr_glui = GLUI_Master.create_glui("Hole Description", 0, 0, 300);
		GLUI_Panel *hole_descr_panel = new GLUI_Panel(hole_descr_glui, "", true);
		//GLUI_Button *btn_close_hole_descr = new GLUI_Button(hole_descr_panel, "Close", 12, control_callback); 
		//GLUI_EditText *txt_player_name = new GLUI_EditText(scoreboard_panel, "Player", GLUI_EDITTEXT_INT);	
		GLUI_EditText *txt_hole_title = hole_descr_glui->add_edittext_to_panel(hole_descr_panel, "Title", GLUI_EDITTEXT_STRING, &current_hole_title);
		txt_hole_title->set_w(400);
		txt_hole_title->disable();
		GLUI_EditText *txt_hole_par = hole_descr_glui->add_edittext_to_panel(hole_descr_panel, "Par", GLUI_EDITTEXT_INT, &current_hole_par);
		txt_hole_par->set_w(400);
		txt_hole_par->disable();
		GLUI_Button *btn_close_hole_descr = new GLUI_Button(hole_descr_panel, "Close", 12, control_callback);
		
		//hole_descr_panel->set_w(500);
		//hole_descr_panel->set_h(200);
		new GLUI_Column(hole_descr_panel, false);	
		hole_descr_glui->set_main_gfx_window(window_id);
	}

	if(current_hole)
	{
		current_hole_title = current_hole->get_title();
		current_hole_par = current_hole->get_par();
	}
	hole_descr_glui->sync_live();
	
}

/*
===========================
close_high_scores
===========================
*/
void close_high_scores(int control)
{
	close_window(high_score_glui);
}

/*
===========================
close_high_scores

Closes window attached to 
@window parameter
===========================
*/
void close_window(GLUI *window)
{
	if(window)
		window->close();
	else
		cout << "Invalid window" << endl;
}

/*
===========================
handle_keyboard
===========================
*/
void handle_keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
	case 27: // ESCAPE
		exit(0); 
	case 's':
	case 'S': // stop ball
		cout << "Key 'S' pressed" << endl;
		current_ball->stop();
		break;
	case 'd':
	case 'D': // turn on dynamic transformations
		for(int i=0; i<all_objects.size(); i++)
		{
			all_objects.at(i)->toggle_dynamic();
			all_objects.at(i)->toggle_identity();
		}
		break;
	case 'p':
	case 'P':
		cur_menu_mode = PLAY;
		break;
	default:
		break;
	}
}

void init_gui()
{
	glui = GLUI_Master.create_glui("Minigolf");			   
	// Setup Window    
	panel = new GLUI_Panel(glui,"",true);		
	// TODO: remove "First chance" exception caused by GLUI code
	txt_player_count = new GLUI_EditText(panel, "How many players?", &player_count, 1, control_callback);
	txt_player_name1 = new GLUI_EditText(panel, "Player 1:", -1, 2, control_callback );
	//new GLUI_EditText(panel, "Player 1:", true, 
	txt_player_name2 = new GLUI_EditText(panel, "Player 2:", -1, 3, control_callback );
	txt_player_name3 = new GLUI_EditText(panel, "Player 3:", -1, 4, control_callback );
	txt_player_name4 = new GLUI_EditText(panel, "Player 4:", -1, 5, control_callback );
	file_browser = new GLUI_FileBrowser(panel, "Golf Hole Browser", false, 6, control_callback);
	btn_start = new GLUI_Button(panel, "Play!", 7, control_callback);
	btn_start_host = new GLUI_Button(panel, "Start Server", 10, control_callback);
	btn_start_client = new GLUI_Button(panel, "Start Client", 11, control_callback);
	btn_quit = new GLUI_Button(panel, "Quit", 8, control_callback);		
	btn_high_scores = new GLUI_Button(panel, "High Scores", 9, control_callback);
	new GLUI_Column(panel, false);	
	glui->set_main_gfx_window(window_id);
}

/*
===========================
load_timers

	Setup initial timers
===========================
*/
void load_timers()
{
	Transformation *tr;
	//ExternalObject *eo;
	//ofstream file;
	//file.open("output.txt", ios::app);
	
	for(int i=0; i<all_trans.size(); i++)
	{
		/*eo = all_objects.at(i);

		for(int i=0; i < eo->get_trans().size(); i++)
		{*/	
			tr = all_trans.at(i);
			// only setup timers for dynamic transformations
			if(tr->get_change() == Transformation::DYNAMIC)
			{				
				glutTimerFunc(tr->get_begin(), handle_timer, i);
			}
		//}
	}
}



/*
===========================
handle_timer
===========================
*/
void handle_timer(int data)
{
	ExternalObject *eo;

	// iterates over structure that contains
	// all objects
	for(int i=0; i<all_objects.size(); i++)
	{
	// increment ticks for all objects
		eo = all_objects.at(i);
		if(eo->get_ticks() >= eo->get_trans_cycle_end())
			eo->reset_ticks();
		else
			eo->incr_ticks();
	}
	// refresh
	glutPostRedisplay();
	// reset tick timer
	glutTimerFunc(50, handle_timer, 50);
}

/*
===========================
handle_timer_old
===========================
*/
void handle_timer_old(int data)
{	
	//ExternalObject *eo;
	Transformation *tr;	
	ofstream file;
	file.open("output.txt", ios::app);

	//cout << "HANDLE_TIMER" << endl;
	//glutTimerFunc(50, handle_timer, 50);

	/*for(int i=0; i<all_objects.size(); i++)
	{
		eo = all_objects.at(i);

		for(int i=0; i < eo->get_trans().size(); i++)
		{*/	
			tr = all_trans.at(data);
			// only setup timers for dynamic transformations
			if(tr->get_change() == Transformation::DYNAMIC)
			{	
				if(tr->get_type() == Transformation::TRANSLATE)
				{								
					((Translation *)tr)->add_total_trans(((Translation *)tr)->get_xpt(), ((Translation *)tr)->get_ypt(), ((Translation *)tr)->get_zpt());
					//glutTimerFunc(tr->get_begin(), handle_timer, i);
					
				}
				else if(tr->get_type() == Transformation::ROTATE)
				{
					((Rotation *)tr)->add_angle_rot(((Rotation *)tr)->get_angle_per_tick());
				}
				//file << tr->get_begin() << " " << tr->get_complete() << endl;
				glutPostRedisplay();
				glutTimerFunc(tr->get_complete() - tr->get_begin(), handle_timer, data);
			}
		/*}
		
	}*/
	
	// reset timer
	

	//file << curr_trans->get_begin() << " " << curr_trans->get_complete() << endl;
	file.close();
}

void setup_client()
{
	char host_type[DEFAULT_BUFLEN];
	char ip[DEFAULT_BUFLEN];
	char port[DEFAULT_BUFLEN];

	cout << "Enter game type: (1) default client (2) custom client" << endl;	
	fgets(host_type, DEFAULT_BUFLEN, stdin);
	cout << "Starting Game ";	
	// get client/server credentials
	if(atoi(host_type) == CUSTOM)
	{
		cout << "Enter IP: ";
		fgets(ip, DEFAULT_BUFLEN, stdin);
		cout << "Enter port: " ;
		fgets(port, DEFAULT_BUFLEN, stdin);
	}	
	else if(atoi(host_type) == DEFAULT)
	{
		strcpy_s(ip, DEFAULT_HOST);
		strcpy_s(port, DEFAULT_PORT);
	}

	// create client/server objects
	ConnectSocket = INVALID_SOCKET; //initialize as invalid		
	cout << "Creating client" << endl;
	trim_str(ip);	
	trim_str(port);				
	client = new Client(ip, port, 512, &ConnectSocket);
	// connect
	cout << "Connecting client to " << client->get_ip() << ":" << client->get_port() << endl;
	client->connect_client();	
}

void setup_server()
{
	char host_type[DEFAULT_BUFLEN];	
	char port[DEFAULT_BUFLEN];

	cout << "Enter game type: (1) default server (2) custom server " << endl;	
	fgets(host_type, DEFAULT_BUFLEN, stdin);
	cout << "Starting Game Server" << endl;
	//cout << (atoi(host_type) == CLIENT || atoi(host_type) == CLIENT_DEF ? "Client" : "Server") << endl;	
	// get client/server credentials	
	if(atoi(host_type) == CUSTOM)
	{
		cout << "Enter port: " << endl;
		fgets(port, DEFAULT_BUFLEN, stdin);
	}		
	else if(atoi(host_type) == DEFAULT)
	{
		strcpy_s(port, DEFAULT_PORT);
	}

	ListenSocket = INVALID_SOCKET;
	ClientSocket = INVALID_SOCKET;

	cout << "Creating server" << endl;
	server = new Server(port, DEFAULT_BUFLEN, &ListenSocket, &ClientSocket);
	// open socket & listen
	server->open_socket();
	server->listen_server();
	// receive messages until shutdown
	cout << "Ready to receive..." << endl;
	//server->receive();
}

// send appropriate data across the wire to server
void send_ball_state()
{
	client->send_ball_waiting_flag(current_ball->get_status() & Ball::WAITING_TO_HIT);
	client->send_left_mouse_status(btn_state[0]);
	client->send_ball_vel(current_ball->get_vel()[0], current_ball->get_vel()[1], current_ball->get_vel()[2]) ;
	client->send_ball_moving_flag(current_ball->get_status() & Ball::IN_MOTION);	
	client->send_ball_stop_flag(current_ball->get_status() & Ball::STOPPED);
	//client->send_ball_finished_moving_flag(WAITING_TO_HIT & Ball::STOPPED );	
}

void draw_shot_arrow(float x, float y, float z)
{
	glBegin(GL_LINES);
	glLineWidth(0.1);
	glColor3f(0,0,1);
	glVertex3f(current_ball->get_x(), current_ball->get_y(), current_ball->get_z());
	glVertex3f(x, y, z);
	glEnd();
}

/*
===========================
main
===========================
*/
int main(int argc, char **argv)
{					
	//SETUP NETWORK SOCKETS
	//check_client_server();	

	// SETUP WINDOW
	glutInit(&argc, argv);
	init("Ian Coleman | CSC 562 | Homework 1", WIDTH, HEIGHT);	

	// SETUP GUI
	init_gui();

	// SETUP GLUT HANDLES
	glutKeyboardFunc(handle_keyboard); 
	glutMotionFunc(handle_transform);
	glutMouseFunc(handle_mouse);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);    
	glutTimerFunc(50, handle_timer, 50);
	GLUI_Master.set_glutIdleFunc(idle);	

	// SETUP MENU
	glutCreateMenu( handle_menu );	// Setup GLUT popup menu
	//glutAddMenuEntry( "New Game...", NEW_GAME );
	//glutAddMenuEntry( "New Game...", NEW_GAME );
	//glutAddMenuEntry( "Play", PLAY );
	glutAddMenuEntry( "Translate", TRANSLATE );
	glutAddMenuEntry( "Zoom", ZOOM );
	glutAddMenuEntry( "Rotate X", ROTATE_X );
	glutAddMenuEntry( "Rotate Y", ROTATE_Y );
	glutAddMenuEntry( "Turn On Lights", LIGHT_ON );
	glutAddMenuEntry( "Turn Off Lights", LIGHT_OFF );	
	glutAddMenuEntry( "Quit", QUIT );
	glutAttachMenu( GLUT_RIGHT_BUTTON );
    
	// --previous perspective code--
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	//gluLookAt( 0, 1, 0, 0, 0, 0, 0, 0, -1 );	
	//gluPerspective(50, 1, 10, 10);
	////glScalef(0.333, 0.333, 0.333);	

	// --new perspective code--
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity(); 
	gluPerspective(50.0, 1.0, 3.0, 7.0); 
	glMatrixMode(GL_MODELVIEW); 
	glLoadIdentity(); 
	gluLookAt(0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0);

	// RUN OPENGL
    glutMainLoop();
    
	return 0;
}
