#pragma once

#include "RakNet/BitStream.h"
#include "RakNet/RakString.h"

struct Message
{
	RakNet::RakString sender;
	RakNet::RakString reciver;
	bool isPrivate;
	RakNet::RakString message;
};