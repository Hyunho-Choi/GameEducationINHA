#pragma once
#include "BaseObject.h"
#include "ColliderChecker.h"

class QuarterMap;
class Base3DObject;
class Player;

struct AreaCube
{
	D3DXVECTOR3 cubeMin;
	D3DXVECTOR3 cubeMax;

	AreaCube()
		:cubeMin(0, 0, 0)
		,cubeMax(0, 0,0)
	{
		
	}
	AreaCube(D3DXVECTOR3& min, D3DXVECTOR3& max)
	{
		cubeMin = min;
		cubeMax = max;
	}
};

struct RoomArea
{
	vector<AreaCube> cubeArr;

	void insertCube(D3DXVECTOR3& min, D3DXVECTOR3& max)
	{
		cubeArr.emplace_back(min, max);
	}
};

class Room :
	public BaseObject, public BaseObserver
{
public :
	Room();
	~Room();
	void Update(Player* player);
	void Render() override;

	bool Update(eEventName eventName, void* parameter) override;
	//void SetPlayer(Player* player) { mPlayer = player; }
	void InsertObject(Base3DObject * object);
	void DeleteObject(string& name);
	bool isInRoom(D3DXVECTOR3& pos);

	void SetArea(RoomArea& area) { mArea = area; }
	map<string, Base3DObject*>& GetObjectsInRoomRef() { return mObjectsInRoom; }
protected:
private:
protected:
private:
	static ColliderChecker mColliderChecker;
	static bool mbCheckCollision;
	RoomArea mArea;
	map<string, Base3DObject*> mObjectsInRoom;
};

