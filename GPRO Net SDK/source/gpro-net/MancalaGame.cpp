/*
	Created by David Bakaleinik with the Apache 2.0 licence

	Written by Dyson Little and David Bakaleinik
*/

#include "gpro-net/MancalaGame.h"

const void RequestPlayerMoveMessage::write(RakNet::BitStream& bs)
{
	bs.Write((RakNet::MessageID)ID_REQUEST_PLAYER_MOVE);
	timeStamp = RakNet::GetTime();
	bs.Write(timeStamp);
	bs.Write(board);

	/*for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			bs.Write(board[i][j]);
		}
	}*/
	//bs.Write(board[0][2]);
}

void RequestPlayerMoveMessage::read(RakNet::BitStream& bs)
{
	//unsigned int i;
	bs.IgnoreBytes(sizeof(RakNet::MessageID));

	bs.Read(timeStamp);
	bs.Read(board);

	/*for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			bs.Read(board[i][j]);
		}
	}*/
	//bs.Read(i);
}

void RequestPlayerMoveMessage::setBoard(gpro_mancala theBoard)
{
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			board[i][j] = theBoard[i][j];
			std::cout << " " << board[i][j] << " ";
		}
		std::cout << "\n";

	}
}

void RequestPlayerMoveMessage::getBoard(gpro_mancala &theBoard)
{
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			theBoard[i][j] = board[i][j];
		}
	}
}

const void ReturnPlayerMoveMessage::write(RakNet::BitStream& bs)
{
	bs.Write((RakNet::MessageID)ID_RETURN_PLAYER_MOVE);
	timeStamp = RakNet::GetTime();
	bs.Write(timeStamp);
	bs.Write(move);
}

void ReturnPlayerMoveMessage::read(RakNet::BitStream& bs)
{
	bs.IgnoreBytes(sizeof(RakNet::MessageID));
	bs.Read(timeStamp);
	bs.Read(move);
}