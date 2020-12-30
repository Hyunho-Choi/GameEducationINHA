#pragma once
#include "Struct.h"

enum PacketNum
{
	INGAME_PACKET,
	CHAT_PACKET,
	SOCKET_NUM_PACKET = 3,
	NOWGAME_PACKET = 8,
	CLEAR_GAME_PACKET = 9
};

//반환값을 벡터로저장
void ClientInput(WPARAM wParam, SOCKET nowPlayer, std::vector<GamePacket>& positon, SOCKET* clientSocket);
void ReturnSocketNum(SOCKET sc, int sun);
//bool IsWin(std::vector<GamePacket> pos, SOCKET sc);
//int IsConnected(int last, int cnt);
void SendResetPacket(SOCKET* sc);
void SendNowGameToObserver(SOCKET sc, std::vector<GamePacket>& positon)