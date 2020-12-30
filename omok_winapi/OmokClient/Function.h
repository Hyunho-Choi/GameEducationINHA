#pragma once
#include "Struct.h"

void ServerInput(WPARAM wParam, std::vector<GamePacket>& positon, int& mySocket, bool& myturn , bool& black, bool& observer);
void SentGame(SOCKET s, LPARAM lParam, int& mySocket, std::vector<GamePacket>& positon);
void DrawCircle(HDC hdc, POINT pt, int r);
void DrawOmokBoard(HDC hdc);