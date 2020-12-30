#pragma once
#include "BaseObject.h"

class Room;
class Player;

enum class eRoomName
{
	R2A01 = 0, R2A02, R2A03, R2A04, R2A05, R2A06, R2A07, R2B01, R2B02, R2B03, R2B04, R2C01, R2C02, R2C03, R2D01
};

class RoomCenter :
	public BaseObject
{
	map<eRoomName, Room*> mRoomMap;
	Player * mPlayer;
public:
	RoomCenter();
	~RoomCenter();
	
	void Update() override;
	void Render() override;
	
	void InsertRoom(eRoomName roomName, Room * room)
	{
		mRoomMap.insert(make_pair(roomName, room));
	}

	void SetPlayer(Player * player)
	{
		mPlayer = player;
	}
	Player* GetPlayer() const
	{
		return mPlayer;
	}

	Room* FindRoomIncludePos(D3DXVECTOR3& pos);
	
	/*void SetCurRoom(eRoomName roomName)
	{
		mCurRoom = mRoomMap[roomName];
	}
	Room* GetCurRoom() const
	{
		return mCurRoom;
	}*/
};

