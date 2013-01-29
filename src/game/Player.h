/* Player.h */
#ifndef PLAYER_H
#define PLAYER_H

#include "Ball.h"

#include <string>

class Player
{
private:
	std::string name;
	//int score;	
	int strokes;
	static int player_count;
	Ball *ball;
	bool finished_with_stroke;
	bool finished_with_hole;
public:	
	Player();
	Player(std::string, Ball *);
	std::string get_name();
	void set_name(std::string);
	//int get_score();
	//void set_score(int);
	int get_strokes();
	void add_stroke();
	void reset_strokes();
	Ball* get_ball();
	void set_ball(Ball *);
	bool finished_stroke();
	void set_finished_stroke();
	bool finished_hole();
	void set_finished_hole();

};

#endif
