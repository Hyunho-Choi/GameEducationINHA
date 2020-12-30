#include "stdafx.h"
#include "RoomCenter.h"
#include "Room.h"
#include "Player.h"


RoomCenter::RoomCenter()
	: mPlayer(nullptr)
{
	mObjectName = string("RoomCenter");
}


RoomCenter::~RoomCenter()
{
	for (map<eRoomName, Room*>::value_type& roomMap : mRoomMap)
	{
		SAFE_DELETE(roomMap.second);
	}
}

void RoomCenter::Update()
{
	for (map<eRoomName, Room*>::value_type& room : mRoomMap)
	{
		if (room.second->isInRoom(mPlayer->GetPos()))
		{
			room.second->Update(mPlayer);
			break;
		}
	}
}

void RoomCenter::Render()
{
	for (map<eRoomName, Room*>::value_type& room : mRoomMap)
	{
		if (room.second->isInRoom(mPlayer->GetPos()))
		{
			room.second->Render();
		}
	}
}

Room* RoomCenter::FindRoomIncludePos(D3DXVECTOR3& pos)
{
	for (map<eRoomName, Room*>::value_type& roomMap : mRoomMap)
	{
		if(roomMap.second->isInRoom(pos))
		{
			return roomMap.second;
		}
	}
	return nullptr;
}
