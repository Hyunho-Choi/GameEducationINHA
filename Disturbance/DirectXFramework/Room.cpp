#include "stdafx.h"
#include "Room.h"
#include "QuarterMap.h"
#include "Base3DObject.h"
#include "Player.h"

ColliderChecker Room::mColliderChecker;
bool Room::mbCheckCollision = false;

Room::Room()
{
	gEventManager->AttachSubscriber(eEventName::KEY_UP, 10, this);
}

Room::~Room()
{
	for (map<string, Base3DObject*>::value_type objectInRoom : mObjectsInRoom)
	{
		SAFE_DELETE(objectInRoom.second);
	}
	mObjectsInRoom.clear();
	gEventManager->DetachSubscriber(eEventName::KEY_UP, this);
}

void Room::Update(Player* player)
{
	for (map<string, Base3DObject*>::value_type objectInRoom : mObjectsInRoom)
	{
		objectInRoom.second->Update();
	}
	if (mbCheckCollision == true)
	{
		mColliderChecker.CheckCollider(player, mObjectsInRoom);
		player->ProcessCollisionEventQueue();
	}
}

//void Room::Update()
//{
//	mPlayer->Update();
//	for (map<string, Base3DObject*>::value_type objectInRoom : mObjectsInRoom)
//	{
//		objectInRoom.second->Update();
//	}
//	mColliderChecker.CheckCollider(mPlayer, mObjectsInRoom);
//	mPlayer->ProcessCollisionEventQueue();
//}

void Room::Render()
{
	for (map<string, Base3DObject*>::value_type objectInRoom : mObjectsInRoom)
	{
		objectInRoom.second->Render();
	}
}

bool Room::Update(eEventName eventName, void* parameter)
{
	switch (eventName)
	{
		case eEventName::KEY_UP:
			{
				eKeyName key = *static_cast<eKeyName*>(parameter);
				if(key == eKeyName::KEY_ONOFFCOLLIDE)
				{
					mbCheckCollision = !mbCheckCollision;
				}
			}
			break;
		default:
			{

			}
			break;
	}
	return false;
}

void Room::InsertObject(Base3DObject* object)
{
	if(object->GetObjectName().empty() == true)
	{
		assert("Room Insert ERROR\nObject Name is Empty");
	}
	else if(mObjectsInRoom.find(object->GetObjectName()) != mObjectsInRoom.end())
	{
		assert("Room Insert ERROR\nAlready in Room Object");
	}
	mObjectsInRoom.insert(make_pair(object->GetObjectName(), object));
	object->DeleteInRoom = bind(&Room::DeleteObject, this, placeholders::_1);
}

void Room::DeleteObject(string& name)
{
	mObjectsInRoom.erase(mObjectsInRoom.find(name));
}

bool Room::isInRoom(D3DXVECTOR3& pos)
{
	for (AreaCube& cube : mArea.cubeArr)
	{
		if (pos.x >= cube.cubeMin.x && pos.x <= cube.cubeMax.x
			&& pos.y >= cube.cubeMin.y && pos.y <= cube.cubeMax.y
			&& pos.z >= cube.cubeMin.z && pos.z <= cube.cubeMax.z)
		{
			return true;
		}
	}
	return false;
}
