#pragma once

#include "RakNet/BitStream.h"
#include "RakNet/RakString.h"
#include "RakNet/RakNetTypes.h"
#include "RakNet/MessageIdentifiers.h"
#include "RakNet/GetTime.h"
#include <stdlib.h>
#include <string>

enum GameMessages
{
	ID_GAME_MESSAGE_1 = ID_USER_PACKET_ENUM + 1,
	ID_CHAT_MESSAGE									// used for sending normal messages
};

#pragma pack(push, 1)
struct ChatMessage
{
	unsigned char useTimeStamp; // Assign ID_TIMESTAMP to this
	RakNet::Time timeStamp; // Put the system time in here returned by RakNet::GetTime() or some other method that returns a similar value
	unsigned char typeId; // Your type here

	RakNet::RakString sender;
	RakNet::RakString reciever; // If blank, send to everyone
	bool isPrivate;
	RakNet::RakString chatMessage;

	ChatMessage(RakNet::RakString sender, RakNet::RakString reciever, bool isPrivate, RakNet::RakString chatMessage)
	{
		this->sender = sender;
		this->reciever = reciever;
		this->isPrivate = isPrivate;
		this->chatMessage = chatMessage;
		this->timeStamp = RakNet::GetTime();
		this->useTimeStamp = ID_TIMESTAMP;
		this->typeId = ID_CHAT_MESSAGE;
	}

	ChatMessage(std::string sender, std::string reciever, bool isPrivate, std::string chatMessage)
	{
		this->sender = RakNet::RakString(sender.c_str());
		this->reciever = RakNet::RakString(reciever.c_str());
		this->isPrivate = isPrivate;
		this->chatMessage = RakNet::RakString(chatMessage.c_str());
		this->timeStamp = RakNet::GetTime();
		this->useTimeStamp = ID_TIMESTAMP;
		this->typeId = ID_CHAT_MESSAGE;
	}
};
#pragma pack(pop)