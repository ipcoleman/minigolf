#include <iostream>
#include <fstream>

#include "GolfGame.h"
#include "convert.h"


using namespace std;

GolfGame::GolfGame()
{
	title = "Golf Game";
	course = new GolfCourse();

	// clear hole-player status array
	for (int i=0; i<MAX_PLAYER_COUNT; i++)
	{
		for(int j=0; j<MAX_HOLE_COUNT; j++)
		{
			finished_with_hole[i][j] = false;
		}
	}
	for (int i=0; i<MAX_PLAYER_COUNT; i++)
	{
		for(int j=0; j<MAX_HOLE_COUNT; j++)
		{
			player_scores[i][j] = 0;
		}
	}
	//player_scores = new int[MAX_PLAYER_COUNT][MAX_HOLE_COUNT];
	//finished_with_hole = new bool[MAX_PLAYER_COUNT][MAX_HOLE_COUNT];
}

GolfGame::GolfGame(std::string t, GolfCourse *c, int _status)
{
	title = t;
	course = c;
	status = _status;

	// clear hole-player status array
	for (int i=0; i<MAX_PLAYER_COUNT; i++)
	{
		for(int j=0; j<MAX_HOLE_COUNT; j++)
		{
			finished_with_hole[i][j] = false;
		}
	}

	for (int i=0; i<MAX_PLAYER_COUNT; i++)
	{
		for(int j=0; j<MAX_HOLE_COUNT; j++)
		{
			player_scores[i][j] = 0;
		}
	}
	//player_scores = new int[MAX_PLAYER_COUNT][MAX_HOLE_COUNT];
	//finished_with_hole = new bool[MAX_PLAYER_COUNT][MAX_HOLE_COUNT];
}


void GolfGame::set_course(GolfCourse *c)
{
	course = c;
}

GolfCourse* GolfGame::get_course()
{
	return course;
}

int GolfGame::get_status()
{
	return status;
}

void GolfGame::set_status(int _status)
{
	status = _status;
}

void GolfGame::play()
{

}

void GolfGame::add_player(Player *pl)
{
	players.push_back(pl);
}

Player* GolfGame::get_player(int index)
{
	return players.at(index);
}

void GolfGame::add_high_score(std::string name, int score, int position)
{
	std::string file_name;
	
		
	file_name = "high_scores.00.db";

	int buffSize = 0;
	char inputFileLine[255];
	string strInputFileLine = "";
	vector<std::string> input_buffer;
	ifstream inputFile;	
	ofstream outputFile;

	inputFile.open(file_name.c_str(), ios::in);
	buffSize = sizeof(inputFileLine)/sizeof(char);

	
	//int j=0;
	//TODO: maybe check for EOF
	while(!inputFile.eof())
	{
		//cout << "File's good" << endl;
		inputFile.getline(inputFileLine, buffSize);	
		//cout << "Printing stuff" << endl;
		cout << inputFileLine << endl;
		strInputFileLine = (std::string)inputFileLine;
		//cout << "Casting to string" << endl;

						

		//cout << input_buffer.at(i) << endl;
		cout << strInputFileLine << endl;

		input_buffer.push_back(strInputFileLine);
		//j++;
	}

	inputFile.close();	

	//cout << "Input buffer size: " << input_buffer.size() << endl;

	//-----------------
	// Open file to write
	// new high score
	//-----------------
	outputFile.open(file_name.c_str(), ios::out | ios::trunc );
	//outputFile << "This is some text.";
	for(int i=0; i<input_buffer.size(); i++)
	{		
		if(position-1 == i)
		{
			//cout << "At position where we're adding score" << endl;
			//cout << name << " " << score << " A Single Hole Course" << "\n";
			outputFile << name << " " << score << " A Single Hole Course" << "\n";
		}
		else
		{
			//cout << "Simply putting line back" << endl;
			//cout << input_buffer.at(i) << "\n";
			outputFile << input_buffer.at(i) << "\n";
		}
	}


	outputFile.close();
}

void GolfGame::set_score(int score, int player, int hole)
{
	player_scores[player][hole] = score;
}

//int* GolfGame::get_scores()
//{
//	return player_scores;	
//}

std::string GolfGame::get_score(int player_index)
{
	std::string score;

	for(int i=0; i<MAX_HOLE_COUNT; i++)
	{
		score.append(int_to_str(player_scores[player_index][i]));
		score.append("     ");
	}

	return score;
}

bool GolfGame::finished_hole_all(int hole_index, int player_count)
{
	bool done = true;

	// loop through active players
	for(int i=0; i<player_count; i++)
	{
		if(!finished_with_hole[i][hole_index])
		{
			done = false;
			break;
		}
	}

	return done;
}

bool GolfGame::finished_hole(int player_index, int hole_index)
{
	//cout << "Player " << player_index << " finished w/ hole " << hole_index << "? " << finished_with_hole[player_index][hole_index] << endl;
	return finished_with_hole[player_index][hole_index];
}

void GolfGame::finish_hole(int player_index, int hole_index)
{
	finished_with_hole[player_index][hole_index] = true;
}

bool GolfGame::finished()
{
	return false;
}