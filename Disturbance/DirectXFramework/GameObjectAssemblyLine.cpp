#include "stdafx.h"
#include "GameObjectAssemblyLine.h"
#include "Scene.h"
#include "Room.h"
#include "Door.h"
#include "Interactable3DObject.h"
#include "RoomCenter.h"
#include "Player.h"
#include "Portal.h"
#include "ColliderCube.h"
#include "ColliderSphere.h"
#include "CollideHandle.h"
#include "CCTV.h"
#include "Chaser.h"
#include "FlashLight.h"
#include "UIConversation.h"

Static3DObject* GameObjectAssemblyLine::CreateStatic3DObject(string objectName, string sourceFileName, D3DXVECTOR3 position,
                                                     D3DXVECTOR3 scale, D3DXVECTOR3 colliderScale, D3DXQUATERNION rotation, string colliderName)
{
	Static3DObject* newStaticObject = new Static3DObject;
	newStaticObject->SetObjectName(objectName);
	newStaticObject->Setup("Resources/XFile/", sourceFileName);
	newStaticObject->SetRot(rotation);
	newStaticObject->SetPos(position);
	newStaticObject->SetScale(scale);

	if ((colliderScale.x <= 0) || (colliderScale.y <= 0) || (colliderScale.z <= 0))
	{
	}
	else
	{
		newStaticObject->AddColliderCube(colliderName);
		newStaticObject->GetColliderCube()["basicColliderCube"]->SetCubeCollider(
			colliderScale.x,
			colliderScale.y,
			colliderScale.z);

		float big = max(colliderScale.x, colliderScale.y);
		newStaticObject->SetCullingSize(max(big, colliderScale.z));

		newStaticObject->GetColliderSphere()->SetRadius(D3DXVec3Length(&(D3DXVECTOR3(colliderScale.x / 2, colliderScale.y / 2, colliderScale.z / 2) - D3DXVECTOR3(colliderScale.x, colliderScale.y, colliderScale.z))));
	}

	return newStaticObject;
}

void GameObjectAssemblyLine::LoadObjectFromJson(string objectName, Room* targetRoom)
{
	D3DXVECTOR3 offset;
	Value& JsonOffset = gJSON->mDocument["offset"];
	offset.x = JsonOffset["x"].GetFloat();
	offset.y = JsonOffset["y"].GetFloat();
	offset.z = JsonOffset["z"].GetFloat();

	static int wallCnt = 0;

	Value& object = gJSON->mDocument[objectName.c_str()];
	for (SizeType i = 0; i < object.Size(); ++i)
	{
		D3DXVECTOR3 scale(1, 1, 1);
		D3DXQUATERNION rotation(0, 0, 0, 0);
		Value::ConstMemberIterator itrot = object[i].FindMember("rotation");
		if (itrot != object[i].MemberEnd())
		{
			D3DXVECTOR3 rot(
				object[i]["rotation"]["x"].GetFloat() * D3DX_PI / 180.f,
				object[i]["rotation"]["y"].GetFloat() * D3DX_PI / 180.f,
				object[i]["rotation"]["z"].GetFloat() * D3DX_PI / 180.f);

			D3DXQUATERNION quater;

			D3DXQuaternionRotationAxis(&rotation, &D3DXVECTOR3(0, 1, 0), rot.y);
		}

		Value::ConstMemberIterator itscale = object[i].FindMember("scale");
		if (itscale != object[i].MemberEnd())
		{
			scale = D3DXVECTOR3(
				object[i]["scale"]["x"].GetFloat(),
				object[i]["scale"]["y"].GetFloat(),
				object[i]["scale"]["z"].GetFloat());
		}

		D3DXVECTOR3 ColliderScale(1.f, 1.f, 1.f);
		Value::ConstMemberIterator itcol = object[i].FindMember("colliderScale");
		if (itcol != object[i].MemberEnd())
		{
			ColliderScale.x = object[i]["colliderScale"]["height"].GetFloat();
			ColliderScale.y = (object[i]["colliderScale"]["width"].GetFloat() * scale.x) - 1.f;
			ColliderScale.z = object[i]["colliderScale"]["depth"].GetFloat();
		}

		D3DXVECTOR3 Position(0.f, 0.f, 0.f);
		Value::ConstMemberIterator itpos = object[i].FindMember("position");
		if (itpos != object[i].MemberEnd())
		{
			Position.x = (object[i]["position"]["x"].GetFloat() + offset.x);
			Position.y = (object[i]["position"]["y"].GetFloat() + offset.y);
			Position.z = (object[i]["position"]["z"].GetFloat() + offset.z);
		}

		Static3DObject* static3Dobject = CreateStatic3DObject(
			objectName + to_string(wallCnt++),
			object[i]["sourceFileName"].GetString(),
			Position,
			scale,
			ColliderScale,
			rotation
		);
		if (objectName == "wall" || objectName == "door")
		{
			static3Dobject->SetTypeTag(eTypeTag::WALL);
		}
		else
		{
			static3Dobject->SetTypeTag(eTypeTag::ITEM);
			static3Dobject->SetShaderPath(eShaderPath::PATH1);
		}

		targetRoom->InsertObject(static3Dobject);
	}
}

void GameObjectAssemblyLine::LoadAreaFromJson(Room* targetRoom)
{
	Value& jsonAreaArr = gJSON->mDocument["area"];
	RoomArea roomArea;
	D3DXVECTOR3 cubeMin;
	D3DXVECTOR3 cubeMax;
	for (int i = 0; i < jsonAreaArr.Size(); ++i)
	{
		cubeMin = { 0, 0, 0 };
		cubeMax = { 0, 0, 0 };
		if(jsonAreaArr[i].FindMember("x_start") != jsonAreaArr[i].MemberEnd())
		{
			cubeMin.x = jsonAreaArr[i]["x_start"].GetFloat();
		}
		if(jsonAreaArr[i].FindMember("y_start") != jsonAreaArr[i].MemberEnd())
		{
			cubeMin.y = jsonAreaArr[i]["y_start"].GetFloat();
		}
		if (jsonAreaArr[i].FindMember("z_start") != jsonAreaArr[i].MemberEnd())
		{
			cubeMin.z = jsonAreaArr[i]["z_start"].GetFloat();
		}
		if (jsonAreaArr[i].HasMember("x_end"))
		{
			cubeMax.x = jsonAreaArr[i]["x_end"].GetFloat();
		}
		if (jsonAreaArr[i].HasMember("y_end"))
		{
			cubeMax.y = jsonAreaArr[i]["y_end"].GetFloat();
		}
		if (jsonAreaArr[i].HasMember("z_end"))
		{
			cubeMax.z = jsonAreaArr[i]["z_end"].GetFloat();
		}
		roomArea.insertCube(cubeMin, cubeMax);
	}
	targetRoom->SetArea(roomArea);
}

void GameObjectAssemblyLine::LoadFromJson(string fileName, Room* targetRoom)
{
	gJSON->LoadJSON(fileName);
	if(gJSON->mDocument.HasMember("wall"))
	{
		LoadObjectFromJson("wall", targetRoom);
	}
	if (gJSON->mDocument.HasMember("door"))
	{
		LoadObjectFromJson("door", targetRoom);
	}
	if (gJSON->mDocument.HasMember("area"))
	{
		LoadAreaFromJson(targetRoom);
	}
	if (gJSON->mDocument.HasMember("2A0304_SM_Prop_Chair_05"))
	{
		LoadObjectFromJson("2A0304_SM_Prop_Chair_05", targetRoom);
	}
	if (gJSON->mDocument.HasMember("2A0304_SM_Prop_Paper_Pile_02"))
	{
		LoadObjectFromJson("2A0304_SM_Prop_Paper_Pile_02", targetRoom);
	}
	if (gJSON->mDocument.HasMember("2A0304_SM_Prop_Printer_01"))
	{
		LoadObjectFromJson("2A0304_SM_Prop_Printer_01", targetRoom);
	}
	if (gJSON->mDocument.HasMember("2A0304_SM_Prop_Cord_Wall_01"))
	{
		LoadObjectFromJson("2A0304_SM_Prop_Cord_Wall_01", targetRoom);
	}
	if (gJSON->mDocument.HasMember("newWall"))
	{
		LoadObjectFromJson("newWall", targetRoom);
	}
	if (gJSON->mDocument.HasMember("newDoor"))
	{
		LoadObjectFromJson("newDoor", targetRoom);
	}
	if (gJSON->mDocument.HasMember("2A0304_SM_Prop_Bin_01"))
	{
		LoadObjectFromJson("2A0304_SM_Prop_Bin_01", targetRoom);
	}
	if (gJSON->mDocument.HasMember("2A0304_SM_Prop_Bin_Rubbish_01"))
	{
		LoadObjectFromJson("2A0304_SM_Prop_Bin_Rubbish_01", targetRoom);
	}
	if (gJSON->mDocument.HasMember("2A0304_SM_Prop_Chair_05"))
	{
		LoadObjectFromJson("2A0304_SM_Prop_Chair_05", targetRoom);
	}
	if (gJSON->mDocument.HasMember("2A0304_SM_Prop_Cord_Wall_01"))
	{
		LoadObjectFromJson("2A0304_SM_Prop_Cord_Wall_01", targetRoom);
	}
	if (gJSON->mDocument.HasMember("2A0304_SM_Prop_CubicalWalHalfl_01"))
	{
		LoadObjectFromJson("2A0304_SM_Prop_CubicalWalHalfl_01", targetRoom);
	}
	if (gJSON->mDocument.HasMember("2A0304_SM_Prop_CubicalWall_01"))
	{
		LoadObjectFromJson("2A0304_SM_Prop_CubicalWall_01", targetRoom);
	}
	if (gJSON->mDocument.HasMember("2A0304_SM_Prop_Plant_Flowers_02"))
	{
		LoadObjectFromJson("2A0304_SM_Prop_Plant_Flowers_02", targetRoom);
	}
	if (gJSON->mDocument.HasMember("2A0304_SM_Prop_Plant_08"))
	{
		LoadObjectFromJson("2A0304_SM_Prop_Plant_08", targetRoom);
	}
	if (gJSON->mDocument.HasMember("2A0304_SM_Prop_Table_01"))
	{
		LoadObjectFromJson("2A0304_SM_Prop_Table_01", targetRoom);
	}
	if (gJSON->mDocument.HasMember("2A0304_SM_Prop_FaxMachine_01"))
	{
		LoadObjectFromJson("2A0304_SM_Prop_FaxMachine_01", targetRoom);
	}
	if (gJSON->mDocument.HasMember("2A0304_SM_Prop_Paper_Pile_02"))
	{
		LoadObjectFromJson("2A0304_SM_Prop_Paper_Pile_02", targetRoom);
	}
	if (gJSON->mDocument.HasMember("2A0304_SM_Prop_Photocopier_01"))
	{
		LoadObjectFromJson("2A0304_SM_Prop_Photocopier_01", targetRoom);
	}
	if (gJSON->mDocument.HasMember("2A0304_SM_Prop_Photocopier_02"))
	{
		LoadObjectFromJson("2A0304_SM_Prop_Photocopier_02", targetRoom);
	}
	if (gJSON->mDocument.HasMember("2A0304_SM_Prop_Plant_08"))
	{
		LoadObjectFromJson("2A0304_SM_Prop_Plant_08", targetRoom);
	}
	if (gJSON->mDocument.HasMember("2A0304_SM_Prop_Plant_14"))
	{
		LoadObjectFromJson("2A0304_SM_Prop_Plant_14", targetRoom);
	}
	if (gJSON->mDocument.HasMember("2A0304_SM_Prop_Photocopier_01"))
	{
		LoadObjectFromJson("2A0304_SM_Prop_Photocopier_01", targetRoom);
	}
	if (gJSON->mDocument.HasMember("2A0304_SM_Prop_Photocopier_02"))
	{
		LoadObjectFromJson("2A0304_SM_Prop_Photocopier_02", targetRoom);
	}
	if (gJSON->mDocument.HasMember("2A0304_SM_Prop_Plant_Flowers_02"))
	{
		LoadObjectFromJson("2A0304_SM_Prop_Plant_Flowers_02", targetRoom);
	}
	if (gJSON->mDocument.HasMember("2A0304_SM_Prop_Printer_01"))
	{
		LoadObjectFromJson("2A0304_SM_Prop_Printer_01", targetRoom);
	}
	if (gJSON->mDocument.HasMember("2A0304_SM_Prop_Table_01"))
	{
		LoadObjectFromJson("2A0304_SM_Prop_Table_01", targetRoom);
	}
	if (gJSON->mDocument.HasMember("2A0304_SM_Prop_Computer_Setup_02"))
	{
		LoadObjectFromJson("2A0304_SM_Prop_Computer_Setup_02", targetRoom);
	}
	//2A0302
	if (gJSON->mDocument.HasMember("2A02_SM_Prop_ArcadeMachine_01"))
	{
		LoadObjectFromJson("2A02_SM_Prop_ArcadeMachine_01", targetRoom);
	}
	if (gJSON->mDocument.HasMember("2A02_SM_Prop_Bin_02"))
	{
		LoadObjectFromJson("2A02_SM_Prop_Bin_02", targetRoom);
	}
	if (gJSON->mDocument.HasMember("2A02_SM_Prop_Chair_09"))
	{
		LoadObjectFromJson("2A02_SM_Prop_Chair_09", targetRoom);
	}
	if (gJSON->mDocument.HasMember("2A02_SM_Prop_CoffeeMachine_01"))
	{
		LoadObjectFromJson("2A02_SM_Prop_CoffeeMachine_01", targetRoom);
	}
	if (gJSON->mDocument.HasMember("2A02_SM_Prop_FruitBowl_01"))
	{
		LoadObjectFromJson("2A02_SM_Prop_FruitBowl_01", targetRoom);
	}
	if (gJSON->mDocument.HasMember("2A02_SM_Prop_Kitchen_Cabinet_01"))
	{
		LoadObjectFromJson("2A02_SM_Prop_Kitchen_Cabinet_01", targetRoom);
	}
	if (gJSON->mDocument.HasMember("2A02_SM_Prop_Kitchen_Counter_01"))
	{
		LoadObjectFromJson("2A02_SM_Prop_Kitchen_Counter_01", targetRoom);
	}
	if (gJSON->mDocument.HasMember("2A02_SM_Prop_Kitchen_CounterSink_01"))
	{
		LoadObjectFromJson("2A02_SM_Prop_Kitchen_CounterSink_01", targetRoom);
	}
	if (gJSON->mDocument.HasMember("2A02_SM_Prop_Plant_04"))
	{
		LoadObjectFromJson("2A02_SM_Prop_Plant_04", targetRoom);
	}
	if (gJSON->mDocument.HasMember("2A02_SM_Prop_Plant_14"))
	{
		LoadObjectFromJson("2A02_SM_Prop_Plant_14", targetRoom);
	}
	if (gJSON->mDocument.HasMember("2A02_SM_Prop_Table_Round_01"))
	{
		LoadObjectFromJson("2A02_SM_Prop_Table_Round_01", targetRoom);
	}
}

void GameObjectAssemblyLine::MakeRoomConnector(Room* firstRoom, eRoomName eFirst, Room* secondRoom,
	eRoomName eSecond, D3DXVECTOR3 doorPos, RoomCenter* roomCenter, eDir dirFirst2Second)
{
	/*D3DXVECTOR3 fisrtPos, secondPos;
	float colGap = 0.5;
	fisrtPos = secondPos = doorPos;

	switch (dirFirst2Second)
	{
		case eDir::UP:
			fisrtPos.x -= colGap;
			secondPos.x += colGap;
			break;
		case eDir::DOWN:
			fisrtPos.x += colGap;
			secondPos.x -= colGap;
			break;
		case eDir::RIGHT:
			fisrtPos.z += colGap;
			secondPos.z -= colGap;
			break;
		case eDir::LEFT:
			fisrtPos.z -= colGap;
			secondPos.z += colGap;
			break;
	}
	static int portalcnt = 0;

	Base3DObject* portalFirst = new Base3DObject();
	portalFirst->SetObjectName(string("portalFisrt") + to_string(portalcnt));
	portalFirst->AddColliderCube(string("portalFisrt") + to_string(portalcnt) + string("collider"));
	portalFirst->Setup();
	portalFirst->SetPos(fisrtPos);

	switch (dirFirst2Second)
	{
		case eDir::UP:
		case eDir::DOWN:
			portalFirst->GetColliderCube()[string("portalFisrt") + to_string(portalcnt) + string("collider")]->SetCubeCollider(2, 1, 2);
			break;
		case eDir::RIGHT:
		case eDir::LEFT:
			portalFirst->GetColliderCube()[string("portalFisrt") + to_string(portalcnt) + string("collider")]->SetCubeCollider(2, 2, 1);
			break;
	}

	portalFirst->CollideHandle = [=](Base3DObject* myObject, string& myColliderTag, Base3DObject* otherObject, string& otherColliderTag)->void
	{
		if (otherColliderTag == "playerCubeCollider")
		{
			roomCenter->SetCurRoom(eSecond);
			cout << "i'm in SecondRoom" << endl;
		}
	};

	firstRoom->InsertObject(portalFirst);

	Base3DObject* portalSecond = new Base3DObject();
	portalSecond->SetObjectName(string("portalSecond") + to_string(portalcnt));
	portalSecond->AddColliderCube(string("portalSecond") + to_string(portalcnt) + string("collider"));
	portalSecond->Setup();
	portalSecond->SetPos(secondPos);

	switch (dirFirst2Second)
	{
		case eDir::UP:
		case eDir::DOWN:
			portalSecond->GetColliderCube()[string("portalSecond") + to_string(portalcnt) + string("collider")]->SetCubeCollider(2, 1, 2);
			break;
		case eDir::RIGHT:
		case eDir::LEFT:
			portalSecond->GetColliderCube()[string("portalSecond") + to_string(portalcnt) + string("collider")]->SetCubeCollider(2, 2, 1);
			break;
	}
	portalSecond->CollideHandle = [=](Base3DObject* myObject, string& myColliderTag, Base3DObject* otherObject, string& otherColliderTag)->void
	{
		if (otherColliderTag == "playerCubeCollider")
		{
			roomCenter->SetCurRoom(eFirst);
			cout << "i'm in FirstRoom" << endl;
		}
	};

	secondRoom->InsertObject(portalSecond);

	portalcnt++;*/
}

void GameObjectAssemblyLine::CreateStartSceneGameObject(Scene* newScene)
{
}

void GameObjectAssemblyLine::CreateIngameSceneGameObject(Scene* newScene)
{
	RoomCenter* roomCenter = new RoomCenter;
	roomCenter->SetObjectName("RoomCenter");
	newScene->mGameObjects.insert(make_pair("RoomCenter", roomCenter));

	Player* player = new Player;
	player->AddColliderCube("playerCubeCollider");
	player->GetColliderCube()["playerCubeCollider"]->SetCubeCollider(8.f, 1.f, 1.f);
	player->Setup();
	player->SetObjectName("player");
	newScene->GetCamera()->SetTarget(player->GetPosRef());
	newScene->AddEventSubscriberList(eEventName::KEY_DOWN, 9, player);
	newScene->AddEventSubscriberList(eEventName::KEY_UP, 9, player);
	newScene->AddEventSubscriberList(eEventName::MOUSE_MOVE, 9, player);
	roomCenter->SetPlayer(player);
	newScene->mGameObjects.insert(make_pair("Aplayer", player));

	Chaser* chaser1 = new Chaser(D3DXVECTOR3(45, 0, 140), roomCenter);
	D3DXQUATERNION chaserRot;
	D3DXQuaternionRotationAxis(&chaserRot, &D3DXVECTOR3(0, 1, 0), -D3DX_PI / 2);
	chaser1->SetRot(chaserRot);
	newScene->mGameObjects.insert(make_pair("Chaser1",chaser1));
	
	Static3DObject* key = new Static3DObject;
	key->SetObjectName("key1");
	//key->Setup("Resources/Xfile/", "Key.X");
	//key->AddColliderCube("basicColliderCube");

	Room* room2A01 = new Room;
	roomCenter->InsertRoom(eRoomName::R2A01, room2A01);
	LoadFromJson("Resources/Json/wall3A01.json", room2A01);
	Room * room2A02 = new Room;
	roomCenter->InsertRoom(eRoomName::R2A02, room2A02);
	LoadFromJson("Resources/Json/wall3A02.json", room2A02);
	LoadFromJson("Resources/Json/OBJ2A02.json", room2A02);

	Room* room2A03 = new Room;
	roomCenter->InsertRoom(eRoomName::R2A03, room2A03);
	LoadFromJson("Resources/Json/wall3A03.json", room2A03);
	LoadFromJson("Resources/Json/OBJ2A03.json", room2A03);
	
	Room2A03EventObject(room2A03, newScene, player);

	

	Room* room2A04 = new Room;
	roomCenter->InsertRoom(eRoomName::R2A04, room2A04);
	LoadFromJson("Resources/Json/wall3A04.json", room2A04);
	Room* room2A05 = new Room;
	roomCenter->InsertRoom(eRoomName::R2A05, room2A05);
	LoadFromJson("Resources/Json/wall3A05.json", room2A05);
	Room* room2A06 = new Room;
	roomCenter->InsertRoom(eRoomName::R2A06, room2A06);
	LoadFromJson("Resources/Json/wall3A06.json", room2A06);
	Room* room2A07 = new Room;
	roomCenter->InsertRoom(eRoomName::R2A07, room2A07);
	LoadFromJson("Resources/Json/wall3A07.json", room2A07);
	Room* room2B01 = new Room;
	roomCenter->InsertRoom(eRoomName::R2B01, room2B01);
	LoadFromJson("Resources/Json/wall3B01.json", room2B01);
	Room* room2B02 = new Room;
	roomCenter->InsertRoom(eRoomName::R2B02, room2B02);
	LoadFromJson("Resources/Json/wall3B02.json", room2B02);
	Room* room2B03 = new Room;
	roomCenter->InsertRoom(eRoomName::R2B03, room2B03);
	LoadFromJson("Resources/Json/wall3B03.json", room2B03);
	Room* room2B04 = new Room;
	roomCenter->InsertRoom(eRoomName::R2B04, room2B04);
	LoadFromJson("Resources/Json/wall3B04.json", room2B04);
	Room* room2C01 = new Room;
	roomCenter->InsertRoom(eRoomName::R2C01, room2C01);
	LoadFromJson("Resources/Json/wall3C01.json", room2C01);
	Room* room2C02 = new Room;
	roomCenter->InsertRoom(eRoomName::R2C02, room2C02);
	LoadFromJson("Resources/Json/wall3C02.json", room2C02);
	Room* room2C03 = new Room;
	roomCenter->InsertRoom(eRoomName::R2C03, room2C03);
	LoadFromJson("Resources/Json/wall3C03.json", room2C03);
	Room* room2D01 = new Room;
	roomCenter->InsertRoom(eRoomName::R2D01, room2D01);
	LoadFromJson("Resources/Json/wall3D01.json", room2D01);
	
	//MakeRoomConnector(room2A01, eRoomName::R2A01, room2A07, eRoomName::R2A07, D3DXVECTOR3(25.0f, 0.f, 17.f), roomCenter, eDir::UP);
	//MakeRoomConnector(room2A01, eRoomName::R2A01, room2A04, eRoomName::R2A04, D3DXVECTOR3(45.0f, 0.f, 35.25f), roomCenter, eDir::UP);
	//MakeRoomConnector(room2A01, eRoomName::R2A01, room2A03, eRoomName::R2A03, D3DXVECTOR3(45.0f, 0.f, 54.5f), roomCenter, eDir::UP);
	//MakeRoomConnector(room2A01, eRoomName::R2A01, room2A03, eRoomName::R2A03, D3DXVECTOR3(45.0f, 0.f, 116.5f), roomCenter, eDir::UP);
	//MakeRoomConnector(room2A01, eRoomName::R2A01, room2A06, eRoomName::R2A06, D3DXVECTOR3(60.0f, 0.f, 4.f), roomCenter, eDir::DOWN);
	//MakeRoomConnector(room2A01, eRoomName::R2A01, room2A05, eRoomName::R2A05, D3DXVECTOR3(60.0f, 0.f, 30.f), roomCenter, eDir::DOWN);
	//MakeRoomConnector(room2A01, eRoomName::R2A01, room2C01, eRoomName::R2C01, D3DXVECTOR3(60.0f, 0.f, 118.5f), roomCenter, eDir::DOWN);
	//MakeRoomConnector(room2C01, eRoomName::R2C01, room2C03, eRoomName::R2C03, D3DXVECTOR3(66.25f, 0.f, 135.5f), roomCenter, eDir::RIGHT);
	//MakeRoomConnector(room2C01, eRoomName::R2C01, room2B01, eRoomName::R2B01, D3DXVECTOR3(72.5f, 0.f, 118.0f), roomCenter, eDir::DOWN);
	//MakeRoomConnector(room2B01, eRoomName::R2B01, room2B02, eRoomName::R2B02, D3DXVECTOR3(85.75f, 0.f, 120.5f), roomCenter, eDir::RIGHT);
	//MakeRoomConnector(room2B02, eRoomName::R2B02, room2B03, eRoomName::R2B03, D3DXVECTOR3(93.0f, 0.f, 133.5f), roomCenter, eDir::DOWN);
	//MakeRoomConnector(room2B01, eRoomName::R2B01, room2B04, eRoomName::R2B04, D3DXVECTOR3(87.5f, 0.f, 99.5f), roomCenter, eDir::DOWN);
	//MakeRoomConnector(room2B04, eRoomName::R2B04, room2B03, eRoomName::R2B03, D3DXVECTOR3(140.0f, 0.f, 108.0f), roomCenter, eDir::RIGHT);
	//MakeRoomConnector(room2C01, eRoomName::R2C01, room2C02, eRoomName::R2C02, D3DXVECTOR3(97.5f, 0.f, 45.0f), roomCenter, eDir::LEFT);
	//MakeRoomConnector(room2C02, eRoomName::R2C02, room2D01, eRoomName::R2D01, D3DXVECTOR3(142.0f, 0.f, 40.0f), roomCenter, eDir::RIGHT);
	
	Static3DObject* floor = CreateStatic3DObject("floor", "mapTile.x", D3DXVECTOR3(75, 0, 75), D3DXVECTOR3(1, 1, 1), D3DXVECTOR3(0, 0, 0));
	floor->SetTypeTag(eTypeTag::FLOOR);
	newScene->mGameObjects.insert(make_pair("AAfloor", floor));

	

	CCTV* cctv = new CCTV(floor->GetMesh(), D3DXVECTOR3(-1,-0.3f,-1), D3DXVECTOR3(59, 3, 134));
	cctv->SetObjectName("CCTV1");
	cctv->ConnectChaser(chaser1);
	room2A01->InsertObject(cctv);
	//newScene->mGameObjects.insert(make_pair("cctv", cctv));
	
	Interactable3DObject* box = new Interactable3DObject;
	box->SetObjectName("box");
	box->AddColliderCube("basicColliderCube");
	box->Setup("Resources/XFile/", "2A02_SM_Prop_Kitchen_Counter_01.X");
	box->SetScale(D3DXVECTOR3(0.04f, 0.04f, 0.04f));
	box->SetPos(D3DXVECTOR3(23.22, 0.69, 120.5 + 9.66));
	box->SetRot(D3DXQUATERNION(0, 0.5f, 0, 0.8f));
	box->SetIsInteractable(true);
	box->AddInteractionCondition(bind(&Interactable3DObject::GetTryInteractionCalled, box));
	box->AddInteractionBehavior([=]()->void
	{
		player->AddItem(eInventorySlot::Key, key);
	});
	box->AddInteractionBehavior(bind(&Interactable3DObject::ChangeToStaticObject, box));
	room2A02->InsertObject(box);

	Portal * portal1 = new Portal(D3DXVECTOR3(1, 0, 0));
	portal1->SetObjectName("portal1");
	portal1->CollideHandle = bind(&Portal::PortalColliderHandler, portal1, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4);
	portal1->SetPos(D3DXVECTOR3(25, 0, 123.5));
	portal1->SetExitPos(D3DXVECTOR3(25, 0, 142.5));
	room2A02->InsertObject(portal1);

	Portal * portal2 = new Portal(D3DXVECTOR3(1, 0, 0));
	portal2->SetObjectName("portal2");
	portal2->CollideHandle = bind(&Portal::PortalColliderHandler, portal2, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4);
	portal2->SetPos(D3DXVECTOR3(25, 0, 142.5));
	portal2->SetExitPos(D3DXVECTOR3(25, 0, 123.5));
	room2A02->InsertObject(portal2);

	Interactable3DObject* door = new Interactable3DObject;
	door->SetObjectName("door");
	door->AddColliderCube("basicColliderCube");
	door->Setup("Resources/XFile/", "newDoor.x", eTypeTag::ITEM);
	door->SetPos(D3DXVECTOR3(3, 0, 133.5));
	door->GetColliderCube()["basicColliderCube"]->SetCubeCollider(3, 1.5, 0.5);
	door->SetIsInteractable(true);
	door->AddInteractionCondition([=]()->bool
	{
		return player->HasItem(eInventorySlot::Key, "key1");
	});
	door->AddInteractionCondition(bind(&Interactable3DObject::GetTryInteractionCalled, door));
	door->AddInteractionBehavior([=]()->void
	{
		D3DXQUATERNION rotY;
		D3DXQuaternionRotationAxis(&rotY, &D3DXVECTOR3(0, 1, 0), D3DX_PI * 0.5f);
		door->SetRot(rotY);
		door->SetPos(door->GetPos() + D3DXVECTOR3(-1, 0, -1));
	});
	door->AddInteractionBehavior([=]()->void
	{
		player->UseItem(eInventorySlot::Key);
	});
	door->AddInteractionBehavior([=]()->void
	{
		portal1->CollideHandle = DefaultColliderHandler;
		portal2->CollideHandle = DefaultColliderHandler;
	});
	door->AddInteractionBehavior(bind(&Interactable3DObject::ChangeToStaticObject, door));
	room2A02->InsertObject(door);

	Door* tempDoor = new Door;
	tempDoor->SetObjectName("tempDoor");
	tempDoor->Setup("Resources/XFile/", "newDoor.x");
	tempDoor->SetRot(D3DXQUATERNION(0, 0, 0, 1));
	tempDoor->SetPos(D3DXVECTOR3(25.f, 1.5f, 18.5f));
	tempDoor->SetScale(D3DXVECTOR3(0.65f, 1.f, 0.5f));
	tempDoor->AddColliderCube("basicColliderCube");
	tempDoor->GetColliderCube()["basicColliderCube"]->SetCubeCollider(8.f, 3.f, 0.5f);
	tempDoor->GetColliderSphere()->SetRadius(D3DXVec3Length(&(D3DXVECTOR3(1, 1, 1) - D3DXVECTOR3(1, 1, 1))));
	room2A02->InsertObject(tempDoor);

	FlashLight* onlyFlashLight = new FlashLight;
	onlyFlashLight->Setup(player->GetPosRef(), player->GetRotPt());
	newScene->mGameObjects.insert(make_pair("ZFlashLight", onlyFlashLight));

}

void GameObjectAssemblyLine::Room2A03EventObject(Room* room, Scene * newScene, Player* player)
{
	Static3DObject* key2 = new Static3DObject;
	key2->SetObjectName("key2");

	Interactable3DObject* plant1 = new Interactable3DObject;
	plant1->SetObjectName("plant1");
	plant1->AddColliderCube("basicColliderCube");
	plant1->Setup("Resources/XFile/", "2A0304_SM_Prop_Plant_14.X");
	plant1->SetScale(D3DXVECTOR3(0.02f, 0.02f, 0.02f));
	plant1->SetPos(D3DXVECTOR3(3.434, 2.1, 50.5 + 3.56));
	plant1->SetRot(D3DXQUATERNION(0, 0, 0, 1));
	plant1->GetColliderSphere()->SetRadius(2.f);
	plant1->SetIsInteractable(true);
	plant1->AddInteractionCondition(bind(&Interactable3DObject::GetTryInteractionCalled, plant1));
	plant1->AddInteractionBehavior([=]()->void
	{
		dynamic_cast<UIConversation*>(newScene->mGameUIs["conversation"])->SetConversation("아무것도 없어...");
	});
	plant1->AddInteractionBehavior(bind(&Interactable3DObject::ChangeToStaticObject, plant1));
	room->InsertObject(plant1);

	plant1 = new Interactable3DObject;
	plant1->SetObjectName("plant2");
	plant1->AddColliderCube("basicColliderCube");
	plant1->Setup("Resources/XFile/", "2A0304_SM_Prop_Plant_14.X");
	plant1->SetScale(D3DXVECTOR3(0.02f, 0.02f, 0.02f));
	plant1->SetPos(D3DXVECTOR3(42.31, 2.1, 50.5 + 27.02));
	plant1->SetRot(D3DXQUATERNION(0, 0, 0, 1));
	plant1->GetColliderSphere()->SetRadius(2.f);
	plant1->SetIsInteractable(true);
	plant1->AddInteractionCondition(bind(&Interactable3DObject::GetTryInteractionCalled, plant1));
	plant1->AddInteractionBehavior([=]()->void
	{
		dynamic_cast<UIConversation*>(newScene->mGameUIs["conversation"])->SetConversation("아무것도 없어...");
	});
	plant1->AddInteractionBehavior(bind(&Interactable3DObject::ChangeToStaticObject, plant1));
	room->InsertObject(plant1);
	

	plant1 = new Interactable3DObject;
	plant1->SetObjectName("plant2");
	plant1->AddColliderCube("basicColliderCube");
	plant1->Setup("Resources/XFile/", "2A0304_SM_Prop_Plant_14.X");
	plant1->SetScale(D3DXVECTOR3(0.02f, 0.02f, 0.02f));
	plant1->SetPos(D3DXVECTOR3(38.434, 2.1, 50.5 + 3.56));
	plant1->SetRot(D3DXQUATERNION(0, 0, 0, 1));
	plant1->GetColliderSphere()->SetRadius(2.f);
	plant1->SetIsInteractable(true);
	plant1->AddInteractionCondition(bind(&Interactable3DObject::GetTryInteractionCalled, plant1));
	plant1->AddInteractionBehavior([=]()->void
	{
		dynamic_cast<UIConversation*>(newScene->mGameUIs["conversation"])->SetConversation("찾았다!");
		player->AddItem(eInventorySlot::Key, key2);
	});
	plant1->AddInteractionBehavior(bind(&Interactable3DObject::ChangeToStaticObject, plant1));
	room->InsertObject(plant1);


	D3DXQUATERNION rotY;
	D3DXQuaternionRotationAxis(&rotY, &D3DXVECTOR3(0, 1, 0), D3DX_PI * 0.5f);
	Interactable3DObject* table = new Interactable3DObject;
	table->SetObjectName("table");
	table->AddColliderCube("basicColliderCube");
	table->Setup("Resources/XFile/", "2A0304_SM_Prop_Table_01.X");
	table->SetScale(D3DXVECTOR3(0.03f, 0.03f, 0.03f));
	table->SetPos(D3DXVECTOR3(11.09, 1.0, 50.5 + 35.0));
	table->SetRot(rotY);
	table->GetColliderSphere()->SetRadius(3.f);
	table->SetIsInteractable(true);
	table->AddInteractionCondition(bind(&Interactable3DObject::GetTryInteractionCalled, table));
	table->AddInteractionBehavior([=]()->void
	{
		dynamic_cast<UIConversation*>(newScene->mGameUIs["conversation"])->SetConversation("\"퇴근시 카드키는 화분 속에 넣어둘것\"");
	});
	table->AddInteractionBehavior(bind(&Interactable3DObject::ChangeToStaticObject, table));
	room->InsertObject(table);
}

void GameObjectAssemblyLine::MakeGameObject(Scene* newScene)
{
	switch (newScene->GetSceneName())
	{
		case eSceneName::START_SCENE :
			{
			}
			break;
		case eSceneName::INGAME_SCENE :
			{
				CreateIngameSceneGameObject(newScene);
			}
			break;
		default :
			{}
			break;
	}
}
