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

/*
	Edits by David Bakaleinik and Dyson Little
	Base code from RakNet website
*/

#include "gpro-net/gpro-net.h"
#include "gpro-net/Message.h"
#include "gpro-net/MancalaGame.h"
#include "gpro-net/common/CustomMessageIDs.h"


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
//	**********	Initial Setup Start **********	//
	const char SERVER_IP[] = "172.16.2.51";
	const unsigned short SERVER_PORT = 7777;

	RakNet::RakPeerInterface* peer = RakNet::RakPeerInterface::GetInstance();
	RakNet::Packet* packet;

	RakNet::SocketDescriptor sd;
	peer->Startup(1, &sd, 1);

	peer->Connect(SERVER_IP, SERVER_PORT, 0, 0);
	printf("Starting the client.\n");
//	**********	Initial Setup End **********	//



//	**********	Coomon Variables Start **********	//
	RakNet::RakString userName = "";
	bool connected = false;

	ReturnPlayerMoveMessage playerMove_ret = ReturnPlayerMoveMessage();
	RequestPlayerMoveMessage playerMove_req = RequestPlayerMoveMessage();

	int player1Score = 0;
	int player2Score = 0;
//	**********	Coomon Variables End **********	//
	

//	********* Testing Space Start*********	//

	

//	********* Testing Space End *********	//

	while (1)
	{
		for (packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive())
		{
			switch (packet->data[0])
			{
			case ID_TIMESTAMP:
			{

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
				printf("Our connection request has been accepted.\n");

//	**********	User Input Section Start **********	//
				//Just get the username and store it in a string
				/*printf("\n Input User Name");
				printf("\n No spaces please!");
				printf("\n User Name ==> ");
				std::string temp;
				std::cin >> temp;

				userName = RakNet::RakString(temp.c_str());*/
//	**********	User Input Section End **********	//


//	**********	Sending User Data Start **********	//
				//RakNet::BitStream bsOut;
				//RakNet::Time stamp = RakNet::GetTime();
				////bsOut.Write((RakNet::MessageID)ID_TIMESTAMP);
				////bsOut.Write(stamp);
				//bsOut.Write((RakNet::MessageID)ID_USER_INFO);
				//bsOut.Write(userName);
				//peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
//	**********	Sending User Data End**********	//


				connected = true;
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
					connected = false;
				break;
			case ID_CONNECTION_LOST:
					printf("Connection lost.\n");
				break;
			case ID_SEND_CHAT_MESSAGE:
			{
				RakNet::BitStream bsIn;
				bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
				ChatMessage inChatMessage;
				bsIn.Read(inChatMessage);
				std::cout << inChatMessage.chatMessage;

			}
			break;
			case ID_REQUEST_PLAYER_MOVE:
			{
				unsigned int num = 0;
				//gpro_mancala updBoard;
				RakNet::BitStream bsIn(packet->data, packet->length, false);
				//bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
				//bsIn.Read(updBoard);
				playerMove_req.read(bsIn);
				
				

				//Print out the state of the board for the player to see
				std::cout << "Board state:: \n";
				std::cout << "_____________________________\n\n";
				std::cout << "| " << playerMove_req.board[0][gpro_mancala_score] << " | " << playerMove_req.board[0][gpro_mancala_cup1] << " | " << playerMove_req.board[0][gpro_mancala_cup2] << " | " << playerMove_req.board[0][gpro_mancala_cup3] << " | " << playerMove_req.board[0][gpro_mancala_cup4] << " | " << playerMove_req.board[0][gpro_mancala_cup5] << " | " << playerMove_req.board[0][gpro_mancala_cup6] << " |\n"; //Row 1
				std::cout << "-----------------------------\n";
				std::cout << "| " << playerMove_req.board[1][gpro_mancala_score] << " | " << playerMove_req.board[1][gpro_mancala_cup1] << " | " << playerMove_req.board[1][gpro_mancala_cup2] << " | " << playerMove_req.board[1][gpro_mancala_cup3] << " | " << playerMove_req.board[1][gpro_mancala_cup4] << " | " << playerMove_req.board[1][gpro_mancala_cup5] << " | " << playerMove_req.board[1][gpro_mancala_cup6] << " |\n"; //Row 2
				std::cout << "_____________________________\n\n";


				//Ask the player for their move
				std::cout << "Please enter a number for the cup you would like to draw from.\n";
				std::cout << "Enter only a single number between 1 and 6 please:::>> ";
				std::cin >> num;

				//Input validation:
				while (num == 0 || num < 1 || num > 6)
				{
					std::cout << "Enter only a single number between 1 and 6 please:::>> ";
					std::cin >> num;
				}

				//Send back the player's responce using ID_RETURN_PLAYER_MOVE
				RakNet::BitStream bsOut;
				playerMove_ret.move = num;
				playerMove_ret.write(bsOut);
				peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);

				
			}
			break;
			case ID_RETURN_PLAYER_MOVE:
			{
				//Shouldn't actually get called, as it's a client->server only ID
			}
			break;
			case ID_SEND_GAME_RESULTS:
			{
				//Recive bitstream w/ results
				RakNet::BitStream bsIn;
				bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
				bsIn.Read(player1Score);
				bsIn.Read(player2Score);

				//Determine who won and print that out
				//Print out the players' scores
				if (player1Score == player2Score)
				{
					std::cout << "\nIt's a Tie!\n";
					std::cout << "Both players got: " << player1Score << " !\n";
				}
				else if (player1Score > player2Score)
				{
					std::cout << "\n Player 1 has won with a score of: " << player1Score << " !\n";

				}
				else
				{
					std::cout << "\n Player 2 has won with a score of: " << player2Score << " !\n";

				}
				
				int status;
				//Ask them if they want to go again or they want to leave
				std::cout << "\n Would you like to play again? \n";
				std::cout << "Enter 1 for YES, enter 2 for NO ::==>> ";
				std::cin >> status;

				while (status != 1 && status != 2)
				{
					std::cout << "Enter 1 for YES, enter 2 for NO ::==>> ";
					std::cin >> status;
				}

				//If they both want to go again, reset the game, otherwise kick both players to the lobby
				RakNet::BitStream bsOut;
				bsOut.Write((RakNet::MessageID)ID_REQUEST_GAME_STATUS);
				bsOut.Write(status);

				peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);


			}
			break;

			default:
				printf("Message with identifier %i has arrived.\n", packet->data[0]);
				break;
			}
		}
		//if (connected)
		//{
		//	// Scan and send outbound messages
		//	printf("> Give recipient? (input _ if you don't want to): ");
		//	char rec[101];
		//	scanf("%100s", &rec);

		//	printf("> Private? 0 for no, 1 for yes: ");
		//	unsigned int isPr = 0;
		//	bool isPrivate;
		//	scanf("%u", &isPr);
		//	if (isPr == 1)
		//	{
		//		isPrivate = true;
		//	}
		//	else
		//	{
		//		isPrivate = false;
		//	}

		//	printf("> ");
		//	char input[101];
		//	std::cin.ignore();
		//	std::cin.getline(input, sizeof input);
		//	ChatMessage outboundMessage = ChatMessage(userName.C_String(), rec, isPr, input);
		//	RakNet::BitStream bsOut;
		//	bsOut.Write((RakNet::MessageID)ID_SEND_CHAT_MESSAGE);
		//	bsOut.Write(outboundMessage);
		//	peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
		//}
	}


	RakNet::RakPeerInterface::DestroyInstance(peer);

	//return 0;

	system("pause");
}



