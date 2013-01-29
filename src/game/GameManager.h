#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "GolfGame.h"
#include "Player.h"
#include "GolfHole.h"

class GameManager
{
private:
	GolfGame *game;
	GolfHole *currentHole;

public:
	GameManager();
	GameManager(std::vector<Player *>);
	virtual GolfGame* get_game();
	virtual void set_game(GolfGame *);

};

#endif