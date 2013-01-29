#include "ExternalObject.h"
#include "physics.h"
#include <GL/glut.h>

ExternalObject::ExternalObject()
{
	total_ticks = 0;
	newest_change_tick = -1;
	prev_change_tick = -1;
	dynamic = false;
	//curr_angle = 0;	
	/*polys.clear();
	trans.clear();
	bbox_verts.clear();	
	bbox_tile_ids.clear();
	bbox_polys.clear();*/
}

ExternalObject::ExternalObject(std::vector<minigolf::Polygon *> _polys)
{
	polys = _polys;
	total_ticks = 0;
	dynamic = false;
	/*trans.clear();
	bbox_verts.clear();	
	bbox_tile_ids.clear();
	bbox_polys.clear();	*/
}

std::vector<minigolf::Polygon *> ExternalObject::get_polys()
{
	return polys;
}

//std::vector<Transformation *> ExternalObject::get_trans()
//{
//	return trans;
//}

void ExternalObject::add_bbox_vert(vert_3f vert)
{
	bbox_verts.push_back(vert);
}

void ExternalObject::add_bbox_tile_id(int t_id)
{
	bbox_tile_ids.push_back(t_id);
}

std::vector<int> ExternalObject::get_bbox_tile_ids()
{
	return bbox_tile_ids;
}

//std::ostream& operator<<(std::ostream &stream, const ExternalObject &eo)
//{
//	stream << "Number of Polys: " << eo.polys.size() << endl;
//
//	return stream;
//}

void ExternalObject::add_poly(minigolf::Polygon *poly)
{
	polys.push_back(poly);
}

//void ExternalObject::add_trans(Transformation *transform)
//{
//	trans.push_back(transform);
//}

void ExternalObject::add_translation(Translation *transl)
{
	translations.push_back(transl);
}

void ExternalObject::add_rotation(Rotation *rot)
{
	rotations.push_back(rot);
}

void ExternalObject::add_bbox_poly(minigolf::Polygon *bb_poly)
{
	bbox_polys.push_back(bb_poly);
}

std::vector<minigolf::Polygon *> ExternalObject::get_bbox_polys()
{
	return bbox_polys;
}

void ExternalObject::build_bbox_polys()
{
	minigolf::Polygon *bb_poly;
	vert_3f v1, v2, v3, v4;

	for(int i=0; i<bbox_verts.size(); i++)
	{
		v1 = bbox_verts.at(i);
		if(i == bbox_verts.size()-1)
			v2 = bbox_verts.at(0);
		else
			v2 = bbox_verts.at(i+1);

		vert_3f v3 = {v2.x, v2.y+0.1, v2.z};
		vert_3f v4 = {v1.x, v1.y+0.1, v1.z};
		////---------
		//v3.x = v2.x;
		//v3.y = v2.y+0.1;
		//v3.z = v2.z;
		////---------
		//v4.x = v1.x;
		//v4.y = v1.y+0.1;
		//v4.z = v1.z;
		/*v3 = bbox_verts.at(i);
		if(i == bbox_verts.size()-1)
			v4 = bbox_verts.at(i+1);
		else
			v4 = bbox_verts.at(0);*/

		bb_poly = new minigolf::Polygon();
		bb_poly->add_vert(v1);
		bb_poly->add_vert(v2);
		bb_poly->add_vert(v3);
		bb_poly->add_vert(v4);

		//cout << "Vector Capacity: " << bbox_polys.capacity() << endl;
		bbox_polys.push_back(bb_poly);
	}

	//x1 = tempVerts.at(i).x;
	//y1 = tempVerts.at(i).y;
	//z1 = tempVerts.at(i).z;
	//// end of border
	//if(i == (int)borderTileIDs.size() - 1)
	//{
	//	x2 = tempVerts.at(0).x;
	//	y2 = tempVerts.at(0).y;
	//	z2 = tempVerts.at(0).z;
	//}
	//else
	//{
	//	x2 = tempVerts.at(i+1).x;
	//	y2 = tempVerts.at(i+1).y;
	//	z2 = tempVerts.at(i+1).z;
	//}	

	//vert_3f v1 = {x1, y1, z1};
	//vert_3f v2 = {x2, y2, z2};	
	//vert_3f v3 = {v2.x, v2.y+0.1, v2.z};	
	//vert_3f v4 = {v1.x, v1.y+0.1, v1.z};	
}

void ExternalObject::draw()
{			

	GLfloat mat[16];
	vert_3f test_vert = {1,2,3};
	/*float x_b,y_b,z_b,w_b;	
	float x_b2,y_b2,z_b2,w_b2;	
	float x_a,y_a,z_a,w_a;	
	float x_a2,y_a2,z_a2,w_a2;	
	float bbox_x, bbox_y, bbox_z, bbox_w;*/

	//float x1, y1, z1;
	//float x1_t, y1_t, z1_t; // stores transformed coordinates
	//float x2, y2, z2;
	//float x2_t, y2_t, z2_t; // stores transformed coordinates
	// Polygons

	//build_bbox_polys();

	for(int i=0; i<polys.size(); i++)
	{	
		glMatrixMode(GL_MODELVIEW);			
		glPushMatrix();		
		//if(!dynamic)
			//glLoadIdentity();		

		/*translate();
		rotate();*/
		translate_static();
		translate_dynamic();
		rotate_static();
		rotate_dynamic();
		polys.at(i)->draw();
		// draw bbox polys
		
		
		glGetFloatv(GL_MODELVIEW_MATRIX, mat);
		transform_bbox_polys(mat);
		for(int j=0; j<bbox_polys.size(); j++)
		{
			//glTranslatef(0.0f, 0.0f, -5.0f);
			bbox_polys.at(j)->draw();
			//bbox_polys.at(j)->get_vertices().at(0).x;
		}
		glPopMatrix();
	}

	
	// Bounding Boxes
	//for(int i=0; i<bbox_polys.size(); i++)
	//{
	//	//glPushMatrix();
	//	glPushMatrix();
	//	glMatrixMode(GL_MODELVIEW);			
	//	glLoadIdentity();	
	//	translate();
	//	bbox_polys.at(i)->draw();
	//	glPopMatrix();

	//	//glPopMatrix();
	//}
}

void ExternalObject::transform_bbox_polys(float *mat)
{
	vert_3f v1, v2, v3, v4;
	float x1, y1, z1, w1;
	float x1_t, y1_t, z1_t, w1_t;
	float x2, y2, z2, w2;
	float x2_t, y2_t, z2_t, w2_t;
	minigolf::Polygon *curr_poly;
	int vert_count;
	vert_3f *curr_vert;
	std::vector<vert_3f> trans_verts;
	//vert_3f curr_vert;

	//glMatrixMode(GL_MODELVIEW);			
	//glPushMatrix();		
	//glLoadIdentity();
	// apply transformations
	//translate();
	//rotate();
	// load model view matrix
	//glGetFloatv(GL_MODELVIEW_MATRIX, mat);

	// only refresh modelview matrix when a 
	// dynamic transformation has been performed
	// or at start (for static transformations)
	if(newest_change_tick > prev_change_tick || total_ticks == 0)
	{
		prev_change_tick = newest_change_tick; //?
		// transform bbox poly verts
		for(int i=0; i<bbox_polys.size(); i++)
		{
			curr_poly = bbox_polys.at(i);
			vert_count = curr_poly->get_vertices().size();
			// work from end of vector;
			// 1. get original, 2. transform, 3. put back
			for(int j=0; j<vert_count; j++)
			{
				//cout << "old bbox vert: " << curr_vert.x << ", " << curr_vert.y << ", " << curr_vert.z << endl;
				curr_vert = &curr_poly->get_verts()->at(j);
				transform_vert(curr_vert, mat);
				//trans_verts.push_back(curr_vert);
				//cout << "new bbox vert: " << curr_poly->get_vertices().at(j).x << ", " << curr_poly->get_vertices().at(j).y << ", " << curr_poly->get_vertices().at(j).z << endl;
				//cout << "curr_vert: " << curr_vert.x << ", " << curr_vert.y << ", " << curr_vert.z << endl;
			}
			// remove all verts
			//curr_poly->clear_verts();
			// add in transformed verts
			for(int k=0; k<trans_verts.size(); k++)
			{
				//curr_poly->add_vert(trans_verts.at(k));
			}
			//curr_poly->draw();
		}
	}
	//x1_t = x1*mat[0] + y1*mat[4] + z1*mat[8] + mat[12];// + w*mat[12];		
	//y1_t = x1*mat[1] + y1*mat[5] + z1*mat[9] + mat[13];// + w*mat[13];		
	//z1_t = x1*mat[2] + y1*mat[6] + z1*mat[10] + mat[14];// + w*mat[14];
	//w1_t = x1*mat[3] + y1*mat[7] + z1*mat[11] + mat[15];// + w*mat[14];

	//x_a /= w_a;
	//y_a /= w_a;
	//z_a /= w_a;

	//glPopMatrix();
}

/*
===========================
transform

	DEPRECATED by static &
	dynamic forms of same
	function
===========================
*/
//void ExternalObject::transform()
//{	
//	Transformation *curr_trans;
//
//	for(int j=0; j<trans.size(); j++)
//	{		
//		curr_trans = trans.at(j);
//		
//		//if(curr_trans->get_change() == Transformation::STATIC)
//		curr_trans->transform(total_ticks);
//	}
//}

void ExternalObject::translate_static()
{
	Transformation *curr_trans;

	for(int i=0; i<translations.size(); i++)
	{
		curr_trans = translations.at(i);		

		if(curr_trans->get_change() == Transformation::STATIC)
			curr_trans->transform();
		//else if(curr_trans->get_change() == Transformation::DYNAMIC)
		//{
		//	if(dynamic)
		//	{
		//		curr_trans->transform(total_ticks);
		//		// record time at which the last dynamic transform
		//		// occurred to determine when to update
		//		// transformation matrix
		//		newest_change_tick = total_ticks;
		//	}
		//}
	}
}

void ExternalObject::translate_dynamic()
{
	Transformation *curr_trans;

	for(int i=0; i<translations.size(); i++)
	{
		curr_trans = translations.at(i);		
		
		if(curr_trans->get_change() == Transformation::DYNAMIC)
		{
			if(dynamic)
			{
				curr_trans->transform(total_ticks);
				// record time at which the last dynamic transform
				// occurred to determine when to update
				// transformation matrix
				newest_change_tick = total_ticks;
			}
		}
	}
}

void ExternalObject::rotate_static()
{
	Transformation *curr_trans;

	for(int i=0; i<rotations.size(); i++)
	{
		curr_trans = rotations.at(i);		

		if(curr_trans->get_change() == Transformation::STATIC)
			curr_trans->transform();
		//else if(curr_trans->get_change() == Transformation::DYNAMIC)
		//{
		//	if(dynamic)
		//	{
		//		curr_trans->transform(total_ticks);
		//		// record time at which the last dynamic transform
		//		// occurred to determine when to update
		//		// transformation matrix
		//		newest_change_tick = total_ticks;
		//	}
		//}
	}
}

void ExternalObject::rotate_dynamic()
{
	Transformation *curr_trans;

	for(int i=0; i<rotations.size(); i++)
	{
		curr_trans = rotations.at(i);		

		if(curr_trans->get_change() == Transformation::DYNAMIC)
		{
			if(dynamic)
			{
				curr_trans->transform(total_ticks);
				// record time at which the last dynamic transform
				// occurred to determine when to update
				// transformation matrix
				newest_change_tick = total_ticks;
			}
		}
	}
}

void ExternalObject::transform_static()
{	
	Transformation *curr_trans;

	// do translations
	for(int j=0; j<translations.size(); j++)
	{		
		curr_trans = translations.at(j);		
		if(curr_trans->get_change() == Transformation::STATIC)
			curr_trans->transform();
	}
	// do rotations
	for(int i=0; i<rotations.size(); i++)
	{
		curr_trans = rotations.at(i);		
		if(curr_trans->get_change() == Transformation::STATIC)
			curr_trans->transform();
	}
}



void ExternalObject::transform_dynamic()
{	
	Transformation *curr_trans;

	// do translations
	for(int j=0; j<translations.size(); j++)
	{		
		curr_trans = translations.at(j);		
		if(curr_trans->get_change() == Transformation::DYNAMIC)
			curr_trans->transform(total_ticks);
	}
	// do rotations
	for(int i=0; i<rotations.size(); i++)
	{
		curr_trans = rotations.at(i);		
		if(curr_trans->get_change() == Transformation::DYNAMIC)
			curr_trans->transform(total_ticks);
	}
}

void ExternalObject::translate()
{
	Transformation *curr_trans;

	for(int i=0; i<translations.size(); i++)
	{
		curr_trans = translations.at(i);		

		if(curr_trans->get_change() == Transformation::STATIC)
			curr_trans->transform();
		else if(curr_trans->get_change() == Transformation::DYNAMIC)
		{
			if(dynamic)
			{
				curr_trans->transform(total_ticks);
				// record time at which the last dynamic transform
				// occurred to determine when to update
				// transformation matrix
				newest_change_tick = total_ticks;
			}
		}
	}
}

void ExternalObject::rotate()
{
	Transformation *curr_trans;

	for(int i=0; i<rotations.size(); i++)
	{
		curr_trans = rotations.at(i);		

		if(curr_trans->get_change() == Transformation::STATIC)
			curr_trans->transform();
		else if(curr_trans->get_change() == Transformation::DYNAMIC)
		{
			if(dynamic)
			{
				curr_trans->transform(total_ticks);
				// record time at which the last dynamic transform
				// occurred to determine when to update
				// transformation matrix
				newest_change_tick = total_ticks;
			}
		}
	}
}

/*
===========================
get_trans_cycle_end

	Gets the greatest end time for all
	transformations for object
===========================
*/
int ExternalObject::get_trans_cycle_end()
{
	Transformation *tr;
	int cycle_end_trans = 0;
	int cycle_end_rot = 0;
	int size_trans = 0;
	int size_rot = 0;

	size_trans = translations.size();
	size_rot = rotations.size();
	// find end time of last transformation	
	if(size_trans > 0)
	{		
		tr = translations.at(size_trans-1);
		cycle_end_trans = tr->get_complete();		
	}
	if(size_rot > 0)
	{
		tr = rotations.at(size_rot-1);
		cycle_end_rot = tr->get_complete();
	}

	if(cycle_end_trans > cycle_end_rot)
		return cycle_end_trans;
	return cycle_end_rot;
}

int ExternalObject::get_ticks()
{
	return total_ticks;
}

void ExternalObject::incr_ticks()
{
	total_ticks++;
}

void ExternalObject::reset_ticks()
{
	total_ticks = 0;
}

void ExternalObject::toggle_dynamic()
{
	if(dynamic)
		dynamic = false;
	else
		dynamic = true;
}

void ExternalObject::toggle_identity()
{
	if(identity)
		identity = false;
	else
		identity = true;
}
	

//void ExternalObject::transform(int change)
//{	
//	Transformation *curr_trans;
//
//	for(int j=0; j<trans.size(); j++)
//	{		
//		curr_trans = trans.at(j);
//
//		
//		glPushMatrix();		
//		// only do type of transformation
//		// same as param
//		//glLoadIdentity();
//		if(curr_trans->get_change() == change)
//		{
//			curr_trans->transform();
//		}
//		glPopMatrix();
//	}
//}

//void ExternalObject::handle_timer(int data)
//{
//	cout << "TIMER HANDLER INSIDE EXTERNALOBJECT" << endl;
//}

//int ExternalObject::get_curr_angle()
//{
//	return curr_angle;
//}

//int ExternalObject::add_curr_angle(int ang_incr)
//{
//	curr_angle += ang_incr;
//}





