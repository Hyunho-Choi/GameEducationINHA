#pragma once
#include "Static3DObject.h"
#include "RoomCenter.h"

class Room;

class GameObjectAssemblyLine
{
private :
	Static3DObject* CreateStatic3DObject(
		string objectName,
		string sourceFileName,
		D3DXVECTOR3	position,
		D3DXVECTOR3 scale,
		D3DXVECTOR3 colliderScale,
		D3DXQUATERNION rotation = D3DXQUATERNION(0, 0, 0, 1),
		string colliderName = "basicColliderCube");

	//void LoadWallFromJson(string fileName, Room* targetRoom);
	void MakeRoomConnector(
		Room * firstRoom, eRoomName eFirst, Room * secondRoom, 
		eRoomName eSecond, D3DXVECTOR3 doorPos, RoomCenter* roomCenter, eDir dirFirst2Second);
	void LoadObjectFromJson(string objectName, Room * targetRoom);
	void LoadAreaFromJson(Room* targetRoom);
	void LoadFromJson(string fileName, Room* targetRoom);
	void CreateStartSceneGameObject(Scene* newScene);
	void CreateIngameSceneGameObject(Scene* newScene);

	void Room2A03EventObject(Room* room, Scene* newScene, Player* player);


public :
	void MakeGameObject(Scene* newScene);
};

