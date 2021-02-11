#pragma once

#include "RakNet/BitStream.h"
#include "RakNet/RakString.h"
#include "RakNet/RakNetTypes.h"
#include "RakNet/MessageIdentifiers.h"
#include "RakNet/GetTime.h"
#include "RakNet/BitStream.h"
#include <stdlib.h>
#include <string>

enum GameMessages
{
	ID_USER_INFO = ID_USER_PACKET_ENUM + 1,
	ID_SEND_CHAT_MESSAGE,									// used for sending normal messages
	ID_GET_CHAT_MESSAGE
};

#pragma pack(push, 1)
struct ChatMessage
{
	unsigned char useTimeStamp; // Assign ID_TIMESTAMP to this
	RakNet::Time timeStamp; // Put the system time in here returned by RakNet::GetTime() or some other method that returns a similar value
	unsigned char typeId; // Your type here

	char sender[101];
	char reciever[101]; // If blank, send to everyone
	bool isPrivate;
	char chatMessage[101];

	ChatMessage(const char sender[], const char reciever[], bool isPrivate, const char chatMessage[])
	{
		std::strcpy(this->sender, sender);
		std::strcpy(this->reciever, reciever);
		this->isPrivate = isPrivate;
		std::strcpy(this->chatMessage, chatMessage);
		this->timeStamp = RakNet::GetTime();
		this->useTimeStamp = ID_TIMESTAMP;
		this->typeId = ID_SEND_CHAT_MESSAGE;
	}

	/*ChatMessage(std::string sender, std::string reciever, bool isPrivate, std::string chatMessage)
	{
		this->sender = RakNet::RakString(sender.c_str());
		this->reciever = RakNet::RakString(reciever.c_str());
		this->isPrivate = isPrivate;
		this->chatMessage = RakNet::RakString(chatMessage.c_str());
		this->timeStamp = RakNet::GetTime();
		this->useTimeStamp = ID_TIMESTAMP;
		this->typeId = ID_SEND_CHAT_MESSAGE;
	}*/

	ChatMessage()
	{

	}
};
#pragma pack(pop)