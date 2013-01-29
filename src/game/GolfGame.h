#ifndef GOLFGAME_H
#define GOLFGAME_H

#include <string>
#include "GolfCourse.h"
#include "Player.h"


// game statuses

#define MAX_HOLE_COUNT 18
#define MAX_PLAYER_COUNT 4

class GolfGame
{
private:
	std::string title;
	GolfCourse *course;
	GolfHole *current_hole;
	std::vector<Player*> players;
	int player_scores[MAX_PLAYER_COUNT][MAX_HOLE_COUNT]; // TODO: make these 2 dynamically sized based on 
	bool finished_with_hole[MAX_PLAYER_COUNT][MAX_HOLE_COUNT]; // number of active players, holes
	
	int status;
public:
	enum GameStatus{PREGAME, LOADING, IN_PROGRESS, FINISHED};
	GolfGame();
	GolfGame(std::string, GolfCourse*, int);
	virtual void set_course(GolfCourse *);
	virtual GolfCourse* get_course();
	virtual int get_status();
	virtual void set_status(int);
	virtual void play();
	virtual void add_player(Player *);
	virtual Player* get_player(int);
	virtual void add_high_score(std::string, int, int);
	virtual void set_score(int, int, int);
	//virtual int* get_scores();
	virtual std::string get_score(int);
	virtual bool finished_hole(int, int);
	virtual bool finished_hole_all(int, int);
	virtual void finish_hole(int, int);	
	virtual bool finished();
};

#endif