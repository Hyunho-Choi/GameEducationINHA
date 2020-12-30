#include "Function.h"
#include "stdafx.h"

void ServerInput(WPARAM wParam, std::vector<GamePacket>& positon, int& mySocket, bool& myturn, bool& black, bool& observer)
{
	char	buffer[100];
	int		msgLen;

	msgLen = recv(wParam, buffer, 100, 0);
	buffer[msgLen] = NULL;

	if (buffer[0] == 0) // 게임패킷(서버발)
	{
		memmove(buffer, buffer + 1, msgLen - 1);
		buffer[msgLen - 1] = NULL;
		GamePacket packet;
		memcpy(&packet, buffer, sizeof(packet));
		positon.push_back(packet);
		if (packet.player == mySocket)
			myturn = false;
		else
			myturn = true;
	}
	else if (buffer[0] == 1) // 채팅패킷
	{
		memmove(buffer, buffer + 1, msgLen - 1);
		buffer[msgLen - 1] = NULL;
	}
	else if (buffer[0] == 3) // 소켓번호리턴패킷
	{
		int temp[2];
		memmove(buffer, buffer + 1, msgLen - 1);
		buffer[msgLen - 1] = NULL;
		memcpy(&temp, buffer, sizeof(temp));
		mySocket = temp[0];

		if ((temp[1] == 1) || (temp[1] == 0))
		{
			myturn = temp[1];
			black = temp[1];
		}
		else
		{
			observer = true;
		}	
	}
	else if (buffer[0] == 8)
	{
		memmove(buffer, buffer + 1, msgLen - 1);
		buffer[msgLen - 1] = NULL;
		GamePacket packet;
		memcpy(&packet, buffer, sizeof(packet));
		positon.push_back(packet);
	}
	else if (buffer[0] == 9)
	{
		positon.clear();
	}
	return;
}


void SentGame(SOCKET s, LPARAM lParam, int& mySocket, std::vector<GamePacket>& positon)
{
	if (((LOWORD(lParam) / CELL_PITCH) > (CELL_NUM - 1)) || ((HIWORD(lParam) / CELL_PITCH) > (CELL_NUM - 1)))
		return;

	GamePacket pos;
	pos.player = mySocket;
	pos.index = (LOWORD(lParam) / CELL_PITCH) + (CELL_NUM * (HIWORD(lParam) / CELL_PITCH));

	for (int i = 0; i < positon.size(); i++)
	{
		if (positon[i].index == pos.index)
			return;
	}

	char	buffer[100];
	int msgLen = sizeof(pos) + 1;
	memcpy(buffer, &pos, msgLen-1);
	memmove(buffer+1, buffer, msgLen - 1);
	buffer[0] = 0;
	buffer[msgLen] = NULL;
	send(s, buffer, msgLen + 1, 0);
	return;
}

void DrawCircle(HDC hdc, POINT pt, int r)
{
	Ellipse(hdc, pt.x - r, pt.y - r, pt.x + r, pt.y + r);
}

void DrawOmokBoard(HDC hdc)
{
	for (int i = 0; i < CELL_NUM; i++)
	{
		MoveToEx(hdc, HALF_CELL_PITCH + (i*CELL_PITCH), HALF_CELL_PITCH, NULL);
		LineTo(hdc, HALF_CELL_PITCH + (i*CELL_PITCH), HALF_CELL_PITCH + (CELL_PITCH * (CELL_NUM-1)));
	}

	for (int i = 0; i < CELL_NUM; i++)
	{
		MoveToEx(hdc, HALF_CELL_PITCH, HALF_CELL_PITCH + (i*CELL_PITCH),  NULL);
		LineTo(hdc, HALF_CELL_PITCH + (CELL_PITCH * (CELL_NUM - 1)), HALF_CELL_PITCH + (i*CELL_PITCH) );
	}
}
