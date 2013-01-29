#include "Player.h"
#include "convert.h"

int Player::player_count = 1;

Player::Player()
{	
	name.append("Player ");
	name.append(int_to_str(player_count));
	//score = 0;
	strokes = 0;
	player_count++;
	ball = new Ball();
	finished_with_hole = false;
}

Player::Player(std::string n, Ball *myBall)
{	
	name = n;
	//score = 0;
	strokes = 0;
	player_count++;
	ball = myBall;
	finished_with_hole = false;
}

std::string Player::get_name()
{
	return name;
}
	

void Player::set_name(std::string n)
{
	name = n;
}

//int Player::get_score()
//{
//	return score;
//}
//	
//
//void Player::set_score(int s)
//{
//	score = s;
//}

int Player::get_strokes()
{
	return strokes;
}
	
void Player::add_stroke()
{
	strokes++;
}

void Player::reset_strokes()
{
	strokes = 0;
}

Ball* Player::get_ball()
{
	return ball;
}

void Player::set_ball(Ball *b)
{
	ball = b;
}

bool Player::finished_stroke()
{
	return finished_with_stroke;
}

void Player::set_finished_stroke()
{
	finished_with_stroke = true;
}

bool Player::finished_hole()
{
	return finished_with_hole;
}

void Player::set_finished_hole()
{
	finished_with_hole = true;
}