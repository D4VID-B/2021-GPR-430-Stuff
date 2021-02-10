/*
   Copyright 2021 Daniel S. Buckstein

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

	   http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*/

/*
	GPRO Net SDK: Networking framework.
	By Daniel S. Buckstein

	main-client.c/.cpp
	Main source for console client application.
*/

#include "gpro-net/gpro-net.h"
#include "gpro-net/Message.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "RakNet/BitStream.h"
#include "RakNet/RakNetTypes.h"
#include "RakNet/MessageIdentifiers.h"
#include "RakNet/RakPeerInterface.h"
#include "RakNet/GetTime.h"


int main(int const argc, char const* const argv[])
{

	//	**********	Initial Setup **********	//
	const char SERVER_IP[] = "172.16.2.194";
	const unsigned short SERVER_PORT = 7777;

	RakNet::RakPeerInterface* peer = RakNet::RakPeerInterface::GetInstance();
	RakNet::Packet* packet;

	RakNet::SocketDescriptor sd;
	peer->Startup(1, &sd, 1);

	peer->Connect(SERVER_IP, SERVER_PORT, 0, 0);
	printf("Starting the client.\n");

	//	**********	Initial Setup **********	//
	
	while (1)
	{
		for (packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive())
		{
			switch (packet->data[0])
			{
			case ID_REMOTE_DISCONNECTION_NOTIFICATION:
				printf("Another client has disconnected.\n");
				break;
			case ID_REMOTE_CONNECTION_LOST:
				printf("Another client has lost the connection.\n");
				break;
			case ID_REMOTE_NEW_INCOMING_CONNECTION:
				printf("Another client has connected.\n");
				break;
			case ID_CONNECTION_REQUEST_ACCEPTED:
			{
				printf("Our connection request has been accepted.\n");

				//	**********	User Input Section **********	//
				//Just get the username and store it in a string
				printf("\n Input User Name");
				printf("\n No spaces!");
				printf("\n User Name ==> ");
				std::string temp;
				std::cin >> temp;

				RakNet::RakString userName = RakNet::RakString(temp.c_str());
				
				//	**********	User Input Section **********	//



				//	**********	Sending User Data **********	//

				RakNet::BitStream bsOut;
				bsOut.Write((RakNet::MessageID)ID_USER_INFO);
				bsOut.Write(userName);
				peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);

				//	**********	Sending User Data **********	//

				
			}
			break;
			case ID_NEW_INCOMING_CONNECTION:
				printf("A connection is incoming.\n");
				break;
			case ID_NO_FREE_INCOMING_CONNECTIONS:
				printf("The server is full.\n");
				break;
			case ID_DISCONNECTION_NOTIFICATION:
					printf("We have been disconnected.\n");
				break;
			case ID_CONNECTION_LOST:
					printf("Connection lost.\n");
				break;
			case ID_SEND_CHAT_MESSAGE:



				break;
			case ID_GET_CHAT_MESSAGE:
			{
				//RakNet::RakString rs;
				//RakNet::BitStream bsIn(packet->data, packet->length, false);
				//bsIn.IgnoreBytes(sizeof(RakNet::MessageID));

				////bsIn.IgnoreBytes(sizeof(RakNet::MessageID));

				////bsIn.Read()
				////bsIn.Read()
				////bsIn.Read()

				//bsIn.Read(rs);
				//printf("%s\n", rs.C_String());
			}
			break;

			default:
				printf("Message with identifier %i has arrived.\n", packet->data[0]);
				break;
			}
		}
	}


	RakNet::RakPeerInterface::DestroyInstance(peer);

	//return 0;

	system("pause");
}

