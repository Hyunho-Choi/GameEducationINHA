#include "stdafx.h"
#include "ServerMap.h"


void ServerMap::SetMap()
{
}

void ServerMap::SendMap()
{
}

int ServerMap::GetMapState(int x, int y)
{
	return this->Map[y][x].state;
}

RECT * ServerMap::GetMapRt(int x, int y)
{
	return &(this->Map[y][x].rt);
}

void ServerMap::ImportMap()
{
	std::string str;
	std::ifstream loadFile;
	loadFile.open("map_index.csv");

	if (loadFile.is_open())
	{
		while (!loadFile.eof())
		{
			std::string temp;
			std::getline(loadFile, temp);
			str += temp;
			str += "\n";
		}
	}

	loadFile.close();

	std::string::iterator it = str.begin();
	std::string toInt;
	for (int i = 2; i < 42;)
	{
		for (int j = 2; j < 22; )
		{
			if (it == str.end())
				return;
			switch ((*it))
			{
			case '\n':
				Map[i][j].state = std::stoi(toInt);
				toInt.clear();
				++j;
				++i;
				break;
			case ',':
				Map[i][j].state = std::stoi(toInt);
				toInt.clear();
				++j;
				break;
			default:
				toInt.push_back(*it);
				break;
			}
			++it;
		}
	}

}

ServerMap::ServerMap()
{
	for (int i = 0; i < MAPSIZEY; i++)
	{
		for (int j = 0; j < MAPSIZEX; j++)
		{
			Map[i][j].rt = { j*MAPTILE_SIZE, i*MAPTILE_SIZE , j*MAPTILE_SIZE + MAPTILE_SIZE, i*MAPTILE_SIZE + MAPTILE_SIZE };
		}
	}
	
	for (int j = 5; j < MAPSIZEX-5; j++)
	{
		Map[MAPSIZEY / 2][j].state = ROCK;
	}

	for (int j = 0; j < MAPSIZEX; j++)
	{
		Map[0][j].state = OUTOFGAME;
		Map[1][j].state = OUTOFGAME;
		Map[MAPSIZEY - 2][j].state = OUTOFGAME;
		Map[MAPSIZEY - 1][j].state = OUTOFGAME;
	}

	for (int j = 0; j < MAPSIZEY; j++)
	{
		Map[j][0].state = OUTOFGAME;
		Map[j][1].state = OUTOFGAME;
		Map[j][MAPSIZEX - 2].state = OUTOFGAME;
		Map[j][MAPSIZEX - 1].state = OUTOFGAME;
	}
	ImportMap();
}


ServerMap::~ServerMap()
{
}
