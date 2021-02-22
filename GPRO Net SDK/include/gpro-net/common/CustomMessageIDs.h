#pragma once


#include "RakNet/MessageIdentifiers.h"

enum GameMessages
{
	ID_USER_INFO = ID_USER_PACKET_ENUM + 1,
	ID_SEND_CHAT_MESSAGE,									// used for sending normal messages
	ID_GET_CHAT_MESSAGE,
	ID_CONFIRM_MESSAGE_RECIEVED,
	ID_REQUEST_PLAYER_MOVE,
	ID_RETURN_PLAYER_MOVE,
	ID_SEND_GAME_RESULTS
};