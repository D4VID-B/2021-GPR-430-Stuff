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
#include <stdio.h>
#include <iostream>
#include "common/gpro-net-gamestate.h"


enum GameMessages
{
	ID_USER_INFO = ID_USER_PACKET_ENUM + 1,
	ID_PLAYER_MOVE,									// used for sending normal messages
	ID_GAME_OVER
};

class MancalaGame
{
protected:
	unsigned int player1Score, player2Score;

	unsigned int currentStones;

	//0 if it's player 1; 1 if it's player 2 -> to determine which row of the array we start in
	unsigned int currentPlayer;

	gpro_mancala board;

public:
	
//			Constructor takes a board and makes a new one, resetting all the scores and stuff
	
	MancalaGame(gpro_mancala newBoard); 


//*********Getters & Setters::
	
	unsigned int getPlayer1Score() { return player1Score; };
	unsigned int getPlayer2Score() { return player2Score; };

	gpro_mancala* getGameBoard();

	void setCurrentPlayer(int num) { if (num == 0) { currentPlayer = 0; } else { currentPlayer = 1; } };

//*********Game logic::
	
	bool validateChoice()
	{
		bool valid = true; //Is the hole picked a valid one
		unsigned int num;

		std::cout << "Please enter select the pocket.\n";
		std::cout << "Only enter whole numbers between 1 and 6.\n";
		std::cout << "Your Number: ";
		std::cin >> num;
			
		if (num != 0 && num >= 1 && num <= 6)
		{
			valid = true;
		}
		else
		{
			valid = false;
		}

		return valid;
	}

	void doTurn(unsigned int holeNum) //does all the turn stuff
	{
		bool isGameDone = false;
		
		//*********Perform turn:

		//Remove all stones
		currentStones = board[currentPlayer][holeNum];
		board[currentPlayer][holeNum] = 0;

		//Go through the board and drop the stones into it
		unsigned int col = holeNum;
		unsigned int row = currentPlayer;


		while (currentStones >= 0)
		{

			if (col > 0)
			{
				col--;
				board[row][col] ++;
				currentStones--;
			}
			else if (col == 0) //Reached the score of the current player
			{
				if (row != currentPlayer && col == 0)
				{
					//We got to the scoring pit of the other player, so we skip it
				}
				else
				{
					board[row][col] ++;
					currentStones--;
				}
				

				//Change board rows
				if (currentPlayer == 0)
				{
					row = 1;
				}
				else
				{
					row = 0;
				}

				//Start from the last cup
				col = gpro_mancala_cup6;
			}
		}

		if (isGameOver())
		{
			return;
		}
		else
		{
			determineTurn(col);
		}

		
		
	}

	bool determineTurn(unsigned int pos)
	{
		bool sameTurn = false; //True if the last player got their last stone into their pit, false otherwise

		//Pos should epual to the last position where a stone was dropped off, so if that position is 0, then it was the score pit
		if (pos == 0)
		{
			sameTurn = true;
		}

		return sameTurn;
	}

	bool isGameOver()
	{
		bool status = false; //Returns true if all of one player's cups are empty

		//If the current player's pockets are all empty, we are done
		if (board[currentPlayer][gpro_mancala_cup1] == 0 && 
			board[currentPlayer][gpro_mancala_cup2] == 0 && 
			board[currentPlayer][gpro_mancala_cup3] == 0 && 
			board[currentPlayer][gpro_mancala_cup4] == 0 && 
			board[currentPlayer][gpro_mancala_cup5] == 0 &&
			board[currentPlayer][gpro_mancala_cup6] == 0 )
		{
			status = true;
			
		}

		return status;
	}

	int calculateOtherPlayerScore()
	{

		//Take all the stones of the other player and add them to their score, since the currentPlayer is the one who is done
		if (currentPlayer == 0)
		{
			board[1][gpro_mancala_score] += board[1][gpro_mancala_cup1] + 
			board[1][gpro_mancala_cup2] + 
			board[1][gpro_mancala_cup3] + 
			board[1][gpro_mancala_cup4] + 
			board[1][gpro_mancala_cup5] +
			board[1][gpro_mancala_cup6];
		}
		else
		{
			board[0][gpro_mancala_score] += board[0][gpro_mancala_cup1] +
				board[0][gpro_mancala_cup2] +
				board[0][gpro_mancala_cup3] +
				board[0][gpro_mancala_cup4] +
				board[0][gpro_mancala_cup5] +
				board[0][gpro_mancala_cup6];
		}

		//Finalise the scores
		player1Score = board[0][gpro_mancala_score];
		player2Score = board[1][gpro_mancala_score];


		return 0;
	}

};