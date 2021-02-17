/*
	Created by David Bakaleinik with the Apache 2.0 licence

	Written by Dyson Little and David Bakaleinik
*/

#pragma once

#include "RakNet/BitStream.h"
#include "RakNet/RakString.h"
#include "RakNet/RakNetTypes.h"
#include "RakNet/MessageIdentifiers.h"
#include "RakNet/GetTime.h"
#include "RakNet/BitStream.h"
#include <stdlib.h>
#include <string>
#include "common/gpro-net-gamestate.h"


enum GameMessages
{
	ID_USER_INFO = ID_USER_PACKET_ENUM + 1,
	ID_CONFIRM_MESSAGE_RECIEVED,
	ID_REQUEST_PLAYER_MOVE,
	ID_RETURN_PLAYER_MOVE,
	ID_SEND_GAME_RESULTS
};

class MancalaGame
{
protected:
	unsigned int score;

	unsigned int currentStones;



public:

	unsigned int getScore() { return score; };

	bool pickHole(unsigned int holeNum)
	{
		bool valid = true; //All holes are fullied at the start

		//perform turn

		return valid;
	}

	void doTurn() //does all the turn stuff
	{
		

	}

	bool determineTurn()
	{
		bool sameTurn = false; //True if the last player got their last stone into their pit, false otherwise

		return sameTurn;
	}

	bool isGameOver()
	{
		bool status = false; //Returns true if all of one player's cups are empty

		return status;
	}

	int calculateScore()
	{

	}

};