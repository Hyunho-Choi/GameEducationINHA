#pragma once
class Base3DObject;
class BaseObject;
class ColliderCube;
class Player;




class ColliderChecker
{
public:
	
	ColliderChecker();
	~ColliderChecker();

	void CheckCollider(Player * player, map<string, Base3DObject*>& objects);

	bool CheckCubeByOBB(ColliderCube * obj1Cube, ColliderCube * obj2Cube);
	bool CheckCube(Base3DObject* object1, Base3DObject* object2, string& obj1Tag, string& obj2Tag);
	bool CheckSphere(Base3DObject * object1, Base3DObject * object2);
	
public:
	map<string, BaseObject*> mColliderMap;
};