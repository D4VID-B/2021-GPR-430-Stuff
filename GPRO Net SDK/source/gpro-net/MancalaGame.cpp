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
}

void RequestPlayerMoveMessage::read(RakNet::BitStream& bs)
{
	bs.IgnoreBytes(sizeof(RakNet::MessageID));
	bs.Read(timeStamp);
	bs.Read(board);
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