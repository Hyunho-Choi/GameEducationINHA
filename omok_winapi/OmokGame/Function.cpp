#include "Function.h"
#include "stdafx.h"


//반환값을 벡터로저장
void ClientInput(WPARAM wParam, SOCKET nowPlayer, std::vector<GamePacket>& positon, SOCKET* clientSocket)
{
	char	buffer[100];
	int		msgLen;
	if (wParam == nowPlayer)
	{
		msgLen = recv(wParam, buffer, 100, 0);
		buffer[msgLen] = NULL;

		if (buffer[0] == INGAME_PACKET) // 게임패킷
		{
			for (int i = 0; i < 4; i++)
			{
				if(clientSocket[i] != NULL)
					send(clientSocket[i], buffer, msgLen, 0);
			}
			
			memmove(buffer, buffer + 1, msgLen - 1);
			buffer[msgLen - 1] = NULL;

			GamePacket packet;
			packet.player = wParam;
			memcpy(&packet, buffer, sizeof(packet));

			positon.push_back(packet);
		}
		else if (buffer[0] == CHAT_PACKET) // 채팅패킷
		{
			memmove(buffer, buffer + 1, msgLen - 1);
			buffer[msgLen - 1] = NULL;
		}
		return;
	}
}

void ReturnSocketNum(SOCKET sc, int sun) //
{
	int temp[2] = { sc, sun };
	char buffer[20];
	memcpy(buffer, &temp, sizeof(temp));
	memmove(buffer + 1, buffer, sizeof(temp));
	buffer[0] = SOCKET_NUM_PACKET;
	buffer[sizeof(temp) + 1] = NULL;
	send(sc, buffer, sizeof(temp) + 1, 0);
}

void SendNowGameToObserver(SOCKET sc, std::vector<GamePacket>& positon) // 패킷번호 8 
{
	for (int i = 0; i < positon.size(); i++)
	{
		char buffer[16];
		memcpy(buffer, &positon[i], sizeof(GamePacket));
		memmove(buffer + 1, buffer, sizeof(GamePacket));
		buffer[0] = NOWGAME_PACKET;
		buffer[sizeof(GamePacket) + 1] = NULL;
		send(sc, buffer, sizeof(buffer) + 1, 0);
	}
}

void SendResetPacket(SOCKET* sc) // 패킷번호 9
{
	char buffer[4];
	buffer[0] = CLEAR_GAME_PACKET;
	buffer[3] = NULL;
	for (int i = 0; i < 4; i++)
	{
		if(sc[i] != NULL)
			send(sc[i], buffer, sizeof(buffer), 0);
	}
}

/*
bool IsWin(std::vector<GamePacket> pos, SOCKET sc)
{
	for (int i = 0; i < pos.size(); i++)
	{
		if (pos[i].player == sc)
		{
			for (int j = 0; j < pos.size(); j++)
			{
				if (pos[i].index == pos[j].index)
					break;
				else if(pos[j].player == pos[i].player)
				{




				}
			}
		}
	}
}
*/
