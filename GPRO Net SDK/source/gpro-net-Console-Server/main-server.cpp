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

	main-server.c/.cpp
	Main source for console server application.
*/

/*
	Edits by David Bakaleinik and Dyson Little
	Base code from RakNet website
*/

#include "gpro-net/gpro-net.h"
#include "gpro-net/MancalaGame.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <map>
#include <fstream>
#include "RakNet/BitStream.h"
#include "RakNet/RakNetTypes.h"
#include "RakNet/MessageIdentifiers.h"
#include "RakNet/RakPeerInterface.h"

int main(int const argc, char const* const argv[])
{
	const unsigned short SERVER_PORT = 7777;
	const unsigned short MAX_CLIENTS = 10;

	RakNet::RakPeerInterface* peer = RakNet::RakPeerInterface::GetInstance();
	RakNet::Packet* packet;

	RakNet::SocketDescriptor sd(SERVER_PORT, 0);
	peer->Startup(MAX_CLIENTS, &sd, 1);

	peer->SetMaximumIncomingConnections(MAX_CLIENTS);
	printf("Starting the server.\n");

	std::map<std::string, std::string> users;

	RakNet::SystemAddress player0Address, player1Address;

	//const char* path = "/Desktop/Chat_Log.txt";
	std::ofstream log;
	//log.open("Chat_Log.txt");

//****		Manacala Stuff		**//
	//Used to figure out if one or both player want to end the game
	int endgameTotal = 0;

	MancalaGame gameInstance = MancalaGame();
	
//****		Manacala Stuff		**//

	log << "Server Online ...\n";

	while (1)
	{
		for (packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive())
		{
			switch (packet->data[0])
			{
			case ID_TIMESTAMP:
			{
				printf("Received packet with timestamp!");
			}
			break;
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
				printf("Welcome to the server\n");

			}
			break;
			case ID_NEW_INCOMING_CONNECTION:
				printf("A connection is incoming.\n");
				break;
			case ID_NO_FREE_INCOMING_CONNECTIONS:
				printf("The server is full.\n");
				break;
			case ID_DISCONNECTION_NOTIFICATION:
					printf("A client has disconnected.\n");
				break;
			case ID_CONNECTION_LOST:
					printf("A client lost the connection.\n");
				break;

			case ID_SEND_CHAT_MESSAGE:
			{
				/* Handling incoming messages:
				When a message arrives, check if there are any recipients - if not, print the message ignoring the isPrivate setting
				If there is 1 or more, and it's not private, print the names of recipients first, then the message
				If there is 1 or more and it is private, send it directly to the recipient

				*/

				printf("\nNew Message Incoming... \n");
				RakNet::BitStream bsIn(packet->data, packet->length, false);
				bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
				ChatMessage inChatMessage;
				bsIn.Read(inChatMessage);
				

				bool isPublicMessage = true;
				if (inChatMessage.isPrivate)
				{
					if (inChatMessage.reciever != "_")
					{
						isPublicMessage = true;
					}
					else
					{
						isPublicMessage = false; //This is the first special case, where the message is private and has specified recipients

						std::string messageTitle = users.at(packet->systemAddress.ToString()) + " whispers something to " + inChatMessage.reciever + ": \n";
						std::cout << messageTitle;
						
						RakNet::BitStream bsOut;
						bsOut.Write((RakNet::MessageID)ID_SEND_CHAT_MESSAGE);
						bsOut.Write(messageTitle);
						bsOut.Write(inChatMessage.chatMessage);
						peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);


						log << "\n";
						log << messageTitle;
						log << inChatMessage.chatMessage;
						log << "\n";
					}
				}
				else
				{
					if (inChatMessage.reciever == "_")
					{
						isPublicMessage = true;

					}
					else
					{
						isPublicMessage = false; //This is the second special case as the message is public, but it is still adressed to specific users

						std::string messageTitle = users.at(packet->systemAddress.ToString()) + " says to " + inChatMessage.reciever + ": \n";

						std::cout << messageTitle;
						std::cout << inChatMessage.chatMessage;
						std::cout << "\n";

						RakNet::BitStream bsOut;
						bsOut.Write((RakNet::MessageID)ID_SEND_CHAT_MESSAGE);
						bsOut.Write(messageTitle);
						bsOut.Write(inChatMessage.chatMessage);
						peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);

						log << "\n";
						log << messageTitle;
						log << inChatMessage.chatMessage;
						log << "\n";
					}
				}
				
				
				if (isPublicMessage) //If this is a public message, send it to everyone
				{

					std::string messageTitle = users.at(packet->systemAddress.ToString()) + " says: \n";
					std::cout << messageTitle;
					std::cout << inChatMessage.chatMessage;
					std::cout << "\n";

					RakNet::BitStream bsOut;
					//RakNet::Time stamp = RakNet::GetTime();
					//bsOut.Write((RakNet::MessageID)ID_TIMESTAMP);
					//bsOut.Write(stamp);
					bsOut.Write((RakNet::MessageID)ID_SEND_CHAT_MESSAGE);
					bsOut.Write(messageTitle);
					bsOut.Write(inChatMessage.chatMessage);
					peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);

					//Now we log the message for safekeeping

					log << "\n";
					log << messageTitle;
					log << inChatMessage.chatMessage;
					log << "\n";
				}
				
			}
			break;
			case ID_GET_CHAT_MESSAGE:
			{
				/*RakNet::RakString rs;
				RakNet::BitStream bsIn(packet->data, packet->length, false);
				bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
				bsIn.Read(rs);
				printf("%s\n", rs.C_String());*/
			}
			break;
			case ID_USER_INFO:
			{
				RakNet::RakString userName;
				//RakNet::Time stamp;

				RakNet::BitStream bsIn(packet->data, packet->length, false);
				//bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
				//bsIn.Read(stamp);
				bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
				bsIn.Read(userName);
				
				users.insert({ packet->systemAddress.ToString(), userName.C_String() });

				std::cout << "Welcome " + users.at(packet->systemAddress.ToString()) + " !";

				log << "\nWelcome " + users.at(packet->systemAddress.ToString()) + " !\n";

			}
				break;
			case ID_RETURN_PLAYER_MOVE:
			{
				// We recieve the player move from the client
				// Check if this was the correct player?
				// Update the game state
				// Determine if the game is over, and if not which player goes next
				// Send that player an ID_REQUEST_PLAYER_MOVE message

				// Check if the correct player is sending us the packet

				if ((packet->systemAddress != player0Address && gameInstance.getCurrentPlayer() == 0) ||
					(packet->systemAddress != player1Address && gameInstance.getCurrentPlayer() == 1))
				{
					// got a message from the wrong player
				}
				else
				{

					ReturnPlayerMoveMessage retPlayerMoveMessage;

					RakNet::BitStream bsIn(packet->data, packet->length, false);

					retPlayerMoveMessage.read(bsIn);

					gameInstance.doTurn(retPlayerMoveMessage.move);

					if (gameInstance.isGameOver())
					{
						// send players game over info
						printf("Game Over!");
					}
					else
					{
						RequestPlayerMoveMessage reqPlayerMoveMessage;
						RakNet::BitStream bsOut;
						reqPlayerMoveMessage.setBoard(gameInstance.getGameBoard());
						reqPlayerMoveMessage.write(bsOut);

						if (gameInstance.getCurrentPlayer() == 0)
						{
							peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, player0Address, false);
						}
						else if (gameInstance.getCurrentPlayer() == 1)
						{
							peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, player1Address, false);
						}
					}
				}
			}
			break;
			case ID_REQUEST_GAME_STATUS:
			{
				int gameStatus;

				RakNet::BitStream bsIn;
				bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
				bsIn.Read(gameStatus);

				endgameTotal += gameStatus;

				if (endgameTotal >= 3) //If it's 3, then one player wants to leave and the other wants to stay. If it's 4 then both want to leave.
				{
					//Close the game and the lobby/server

				}
				else
				{
					//Reset the game and start again
					RequestPlayerMoveMessage message = RequestPlayerMoveMessage();
					gameInstance = MancalaGame();

					RakNet::BitStream bsOut;
					message.setBoard(gameInstance.getGameBoard());
					message.write(bsOut);

					peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);

				}

			}
			break;
			default:
				printf("Message with identifier %i has arrived.\n", packet->data[0]);
				break;
			}
		}
	}


	RakNet::RakPeerInterface::DestroyInstance(peer);
	log.close();

	//return 0;

system("pause");

}
