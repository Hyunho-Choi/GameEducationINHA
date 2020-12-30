#pragma once

struct GamePacket
{
	int player;
	int index;
};

struct ChatPacket
{
	int player;
	char msg[128];
};
