
#include <iostream>
#include <math.h>

#include "Ball.h"
#include "vector.h"
#include "glut.h"

using namespace std;

Ball::Ball()
{
	x = 0; 
	y = 0; 
	z = 0;
	radius = 0.03;
	surface_tension = 0.001;
	vel[0] = 0;
	vel[1] = 0;
	vel[2] = 0;
	status = 0;
}

Ball::Ball(float _x, float _y, float _z)
{	
	//Ball(_x,_y,_z,10);
	x = _x;
	y = _y;
	z = _z;
	radius = 0.03;
	currentTile = new Tile();
	surface_tension = 0.001;
	vel[0] = 0;
	vel[1] = 0;
	vel[2] = 0;
	status = 0;
}

Ball::Ball(float _x, float _y, float _z, float _radius)
{
	x = _x;
	y = _y;
	z = _z;
	radius = _radius;
	currentTile = new Tile();
	surface_tension = 0.001;
	vel[0] = 0;
	vel[1] = 0;
	vel[2] = 0;
	status = 0;
	//RGBA = _rgba;
	//*RGBA = *_rgba;
}

Ball::Ball(float _x, float _y, float _z, float _radius, Tile *newTile)
{
	x = _x;
	y = _y;
	z = _z;
	radius = _radius;
	currentTile = newTile;
	surface_tension = 0.001;
	vel[0] = 0;
	vel[1] = 0;
	vel[2] = 0;
	status = 0;
	//RGBA = _rgba;
	//*RGBA = *_rgba;
}

float Ball::get_x()
{
	return x;
}

float Ball::get_y()
{
	return y;
}

float Ball::get_z()
{
	return z;
}

void Ball::set_x(float _x)
{
	x = _x;
}

void Ball::set_y(float _y)
{
	y = _y;
}

void Ball::set_z(float _z)
{
	z = _z;
}

float* Ball::get_vel()
{
	return vel;
}

int Ball::get_status()
{
	return status;
}
void Ball::set_status(int s)
{
	status = s;
}

void Ball::add_status(int s)
{
	status |= s;
}

void Ball::clear_status()
{
	status = 0;
}

float* Ball::get_color()
{
	return RGBA;
}

void Ball::set_color(float *_rgba)
{
	RGBA[0] = _rgba[0];
	RGBA[1] = _rgba[1];
	RGBA[2] = _rgba[2];
	RGBA[3] = _rgba[3];
}

float Ball::get_surface_tension()
{
	return surface_tension;
}

float* Ball::get_coordinates()
{
	float c[3];

	c[0] = x;
	c[1] = y;
	c[2] = z;

	return c;
}

GolfHole* Ball::get_hole()
{
	return hole;
}

void Ball::set_hole(GolfHole *_hole)
{
	cout << "Setting hole..." << endl;
	hole = _hole;
}

Tile* Ball::get_tile()
{
	return currentTile;
}

void Ball::set_tile(Tile *newTile)
{
	currentTile = newTile;
}

int Ball::get_tile_id()
{
	if(currentTile)
	{
		return currentTile->get_id();
	}
	else
	{
		return -1;
	}
}

void Ball::draw()
{			
	int num_tiles = 0;

	glPushMatrix();
	//glLoadIdentity();
	glColor3f(RGBA[0],RGBA[1],RGBA[2]);
	glTranslatef(x, y, z);	
	glutSolidSphere(radius, 30, 30);	
	glPopMatrix();

	//vertex_trace();
	
	num_tiles = hole->get_tiles()->size();
	Tile *tile;

	for(int i=0; i<num_tiles; i++)
	{
		tile = &hole->get_tiles()->at(i);
		if(on_tile(tile))
		{
			currentTile = tile;
			break;
		}
	}

	if(on_cup_tile())
	{
		//cout << "On cup tile!!" << endl;
		if(in_cup())
		{
			//cout << "In hole!!" << endl;
		}
	}

	//check_border_collision();
}

void Ball::hitv(float *v) 
{
	
	vel[0] = v[0];
	vel[1] = v[1];
	vel[2] = v[2];
	this->add_status(HIT);
}

void Ball::hit(float _x, float _y, float _z)
{
	float vel[] = {_x,_y,_z};
	hitv(vel);
}

void Ball::move()
{
	// set velocity to 0 when near 0
	if(vector_magnitude(vel) < 0.00001)
	{
		vel[0] = 0;
		vel[1] = 0;
		vel[2] = 0;
	}

	// movement per update
	x += vel[0];
	y += vel[1];
	z += vel[2];		
}

void Ball::apply_grav()
{
	//float dir_grav[] = {0,1,0};

	float grav[] = {0, 0.01, 0};

	//vel[0] = vel[0] + grav[0];
	//vel[1] = vel[1] + grav[1];
	//vel[2] = vel[2] + grav[2];
}

void Ball::apply_surface_resistance(float accel)
{
	float accel_v[] = {accel, 0, accel};

	//HACK -- if velocity gets reeeeeaallly close to 0, then stop applying surface resistance
	if(vector_magnitude(vel) > 0.00001)
	{
		if(abs(vel[0]) > 0.00001)
		{
			if(vel[0] > 0.0)
				accel_v[0] = -accel;
			if(vel[1] > 0.0)
				accel_v[1] = -accel;
			if(vel[2] > 0.0)
				accel_v[2] = -accel;


			vel[0] = vel[0] + accel_v[0];
			//vel[1] = vel[1] + accel;
			vel[2] = vel[2] + accel_v[2];
		}

		//cout << "x velocity: " << vel[0] << endl;

		/*if(abs(vel[2]) > 0.00001)
			vel[2] = vel[2] + accel;*/
	}
	else // if close to 0, set vel = {0,0,0}
	{
		vel[0] = 0;
		vel[1] = 0;
		vel[2] = 0;
	}
}

bool Ball::on_cup_tile()
{	
	if(currentTile)
	{
		if(hole)
		{
			if(currentTile->get_id() == hole->get_cup_tile_id())
				return true;
		}
	}
	else
	{
		cout << "Unknown tile for ball" << endl;
	}
	return false;	
}

float* Ball::get_dir_from_mouse(int mouse_x, int mouse_y, float *dir)
{
	//float dir[3];

	dir[0] = abs(mouse_x - this->x); // x distance
	dir[1] = 0; // y distance
	dir[2] = abs(mouse_x - this->y); // z distance

	return dir;
}

bool Ball::is_stopped()
{	
	if(vector_magnitude(vel) == 0.0)
		return true;
	return false;	
}

bool Ball::on_tile(Tile *tile)
{
	const double E = 0.0000001;
	int i;
	double m1, m2;
	double angle_sum = 0;
	double cos_theta;
	vert_3f v1,v2;
	std::vector<vert_3f> verts = tile->get_vertices();
	int n = 0; 
	
	if(tile)
	{
		/*if(tile->get_vertices())
		{*/
			// numbert of vertices for tile
			n = tile->get_vertices().size();
		//}
	}

	for (i=0;i<n;i++) {

		v1.x = verts.at(i).x - this->get_x();
		v1.y = verts.at(i).y - this->get_y();
		v1.z = verts.at(i).z - this->get_z();
		v2.x = verts.at((i+1)%n).x - this->get_x();
		v2.y = verts.at((i+1)%n).y - this->get_y();
		v2.z = verts.at((i+1)%n).z - this->get_z();

		m1 = vector_magnitude(v1);
		m2 = vector_magnitude(v2);
		if (m1*m2 <= E)
		{
			currentTile = tile;
			//cout << "Current Tile: " << tile->get_id() << endl;
			return true;			

			//return(2*M_PI); // consider this inside 
		}
		else 
		{
			cos_theta = (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z) / (m1*m2);
		}

		angle_sum += acos(cos_theta);
	}

	if(angle_sum < 2*M_PI)
		return false;

	currentTile = tile;
	//cout << "Current Tile: " << tile->get_id() << endl;
	return true;

	//return(angle_sum);
}

/*
===========================
check_border_collision

	Check if ball collides w/
	border; update direction
	accordingly
===========================
*/
//void Ball::check_border_collision(std::vector<Tile> *tileBorders)
//{
//	Tile *tileBorder = new Tile();
//	float bounce_dir[3];
//	float *dir;
//	vert_3f ball_center = {this->get_x(), this->get_y(), this->get_z()};	
//	float dist_to_border;
//
//	// check collision w/ all border polys
//	for(int i=0; i<(int)tileBorders->size(); i++)
//	{
//		tileBorder = &tileBorders->at(i);
//
//		//cout << "Distance from ball to border: " << dist_point_to_plane(ball_center, tileBorder) << endl;
//
//		if(tileBorder)
//		{
//			dist_to_border = dist_point_to_plane(ball_center, tileBorder);
//			if(dist_to_border < 0.003 && dist_to_border > -0.003)
//			{
//				//cout << "BORDER COLLISION" << endl;
//				tileBorder->calc_normal_vector();
//				dir = get_bounce_dir(tileBorder->get_normal());
//				// Warning: pointer arithmetic ahead
//				bounce_dir[0] = -*dir;				
//				vel[0] = bounce_dir[0];
//				dir++;
//				bounce_dir[1] = *dir;				
//				dir++;
//				bounce_dir[2] = -*dir;
//				vel[2] = bounce_dir[2];				
//				break;
//			}
//		}
//	}
//}



void Ball::check_border_collision(std::vector<minigolf::Polygon *> polys)
{
	//Tile *tileBorder = new Tile();
	minigolf::Polygon *poly;
	float bounce_dir[3];
	float *dir;
	vert_3f ball_center = {this->get_x(), this->get_y(), this->get_z()};	
	float dist_to_border;

	// check collision w/ all border polys
	for(int i=0; i<(int)polys.size(); i++)
	{
		//tileBorder = &tileBorders->at(i); 
		poly = polys.at(i);
		//cout << "Checking " << polys.size() << " borders for collision" << endl;

		//cout << "Distance from ball to border: " << dist_point_to_plane(ball_center, tileBorder) << endl;

		if(poly)
		{
			dist_to_border = dist_point_to_plane(ball_center, poly);
			if(dist_to_border < 0.005 && dist_to_border > -0.005)
			{
				cout << "BORDER COLLISION" << endl;
				poly->calc_normal_vector();
				dir = get_bounce_dir(poly->get_normal());
				// Warning: pointer arithmetic ahead
				bounce_dir[0] = -*dir;				
				vel[0] = bounce_dir[0];
				dir++;
				bounce_dir[1] = *dir;				
				dir++;
				bounce_dir[2] = -*dir;
				vel[2] = bounce_dir[2];		
				//cout << "poly normal: " << poly->get_normal()[0] << ", " << poly->get_normal()[1] << ", " << poly->get_normal()[2] << endl;
				for(int j=0; j<poly->get_vertices().size(); j++)
				{
					cout << "Border Poly Verts: " << poly->get_vertices().at(j).x;
					cout << ", " << poly->get_vertices().at(j).y ;
					cout << ", " << poly->get_vertices().at(j).z << endl;
				}
				break;
			}
		}
	}
}

void Ball::check_object_collision(std::vector<ExternalObject*> objs)
{	
	for(int i=0; i<objs.size(); i++)
	{
		for(int j=0; j<objs.at(i)->get_bbox_tile_ids().size(); j++)
		{
			if(currentTile)
			{
				if(objs.at(i)->get_bbox_tile_ids().at(j) == currentTile->get_id())
				{
					//cout << "Checking for object collision" << endl;
					check_border_collision(objs.at(i)->get_bbox_polys());
					//cout << "bbox poly vert x: " << objs.at(i)->get_bbox_polys().at(0)->get_vertices().at(0).x << endl;
					//cout << "bbox poly vert y: " << objs.at(i)->get_bbox_polys().at(0)->get_vertices().at(0).y << endl;
					//cout << "bbox poly vert z: " << objs.at(i)->get_bbox_polys().at(0)->get_vertices().at(0).z << endl;
				}
			}
		}
	}
}

void Ball::check_poly_collision(minigolf::Polygon *poly)
{
	//Tile *tileBorder = new Tile();
	float bounce_dir[3];
	float *dir;
	vert_3f ball_center = {this->get_x(), this->get_y(), this->get_z()};	

	// check collision w/ all border polys
	/*for(int i=0; i<tileBorders->size(); i++)
	{*/
		//tileBorder = &tileBorders->at(i);

		//cout << "Distance from ball to border: " << dist_point_to_plane(ball_center, tileBorder) << endl;

	if(poly)
	{
		if(dist_point_to_plane(ball_center, poly) < 0.01 && dist_point_to_plane(ball_center, poly) > -0.01 )
		{
			cout << "POLY COLLISION" << endl;
			poly->calc_normal_vector();
			dir = get_bounce_dir(poly->get_normal());
			// Warning: pointer arithmetic ahead
			bounce_dir[0] = -*dir * 0.001;				
			vel[0] = bounce_dir[0];
			dir++;
			bounce_dir[1] = *dir;				
			dir++;
			bounce_dir[2] = -*dir * 0.001;
			vel[2] = bounce_dir[2];				
			//break;
		}
	}
	//}
}

float* Ball::get_bounce_dir(float *border_norm)
{
	  //D = W + H = W + ( W + D_in )
		
	float N_vec[3]; // border normal vector
	float *W_vec; // vector in direction of N
	float H_vec[3]; // incoming angle as vector
	float D_vec[3]; // reflection vector
	float Din_vec[] = {vel[0], vel[1], vel[2]}; // collision vector = velocity @ collision
	float vectorSum[3]; // stores sums of vectors
	float ang_betw;
	float W_vec_mag;
	float *D_in_neg;

	//W_vec =  D_in projected onto N_vec
	N_vec[0] = border_norm[0];
	N_vec[1] = border_norm[1];
	N_vec[2] = border_norm[2];

	cout << "Din_vec: (" << Din_vec[0] << ", " << Din_vec[1] << ", " << Din_vec[2] << ")" << endl;
	D_in_neg = vector_mult(Din_vec, -1);
	cout << "Din_vec [neg]: (" << Din_vec[0] << ", " << Din_vec[1] << ", " << Din_vec[2] << ")" << endl;
	cout << "N_vec: (" << N_vec[0] << ", " << N_vec[1] << ", " << N_vec[2] << ")" << endl;
	ang_betw = ang_between(D_in_neg, N_vec);
	cout << "Angle Between: " << ang_betw << endl;
	
	// get magnitude of D_in projected onto border normal
	W_vec_mag = vector_magnitude(D_in_neg) * cos(ang_betw);
	W_vec = vector_mult(N_vec, W_vec_mag);
	cout << "W_vec: (" << W_vec[0] << ", " << W_vec[1] << ", " << W_vec[2] << ")" << endl;
	
	vector_mult(Din_vec, -1); // reorient D_in vector
	vector_add(W_vec, Din_vec, vectorSum); // W + D_in = H
	vector_add(vectorSum, W_vec, D_vec); // D = H + W


	cout << "Reflection vector: (" << D_vec[0] << ", " << D_vec[1] << ", " << D_vec[2] << ")" << endl;
	return D_vec;  // reflection vector
}

bool Ball::in_cup()
{	
	//return on_tile(holeTile);
	vert_3f cup_loc = hole->get_cup();
	vert_3f ball_loc = { get_x(), get_y(), get_z() };
	float dist_to_cup[3];

	dist_to_cup[0] = cup_loc.x - ball_loc.x;
	dist_to_cup[1] = cup_loc.y - ball_loc.y;
	dist_to_cup[2] = cup_loc.z - ball_loc.z;

	if(vector_magnitude(dist_to_cup) < 0.05 )
	{
		return true;
		//cout << "BALL IN HOLE" << endl;
	}
	return false;
}

void Ball::vertex_trace()
{
	//cout << "cool." << endl;
	if(currentTile)
	{
		//cout << "still cool." << endl;
		//cout << "Current tile id: " << currentTile->get_id() << endl;
		//cout << "still still cool." << endl;
		std::vector<vert_3f> curr_tile_verts = currentTile->get_vertices();
		int num_verts = (int)curr_tile_verts.size();
		vert_3f curr_vert;

		glPushMatrix();
		
		for(int i=0; i<num_verts; i++)
		{
			curr_vert = curr_tile_verts.at(i);
			//cout << "current vertex: (" << curr_vert.x << ", " << curr_vert.y << ", " << curr_vert.z << ")" << endl;

			glBegin(GL_LINES);
			glColor3f(0.8,0.8,0.8);
			glVertex3f(get_x(), get_y(), get_z());
			glVertex3f(curr_vert.x, curr_vert.y, curr_vert.z);
			glEnd();
		}
		glPopMatrix();
	}
}

bool Ball::stationary()
{
	return vector_magnitude(vel) <= 0.0001;
}

void Ball::stop()
{
	vel[0] = 0;
	vel[1] = 0;
	vel[2] = 0;

	status |= STOPPED;
	//status 
}

bool Ball::finished_with_stroke()
{
	return (status & Ball::HIT) && (status & Ball::STOPPED);
}

void Ball::set_finished_with_stroke()
{
	status |= (Ball::HIT | Ball::STOPPED);
}

void Ball::move_away_from_hole()
{
	x += 1;
	//z += 1;
}

