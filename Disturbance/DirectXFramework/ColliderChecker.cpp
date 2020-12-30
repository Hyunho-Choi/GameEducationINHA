#include "stdafx.h"
#include "ColliderChecker.h"
#include "ColliderCube.h"
#include "ColliderSphere.h"
#include "Player.h"

ColliderChecker::ColliderChecker()
{
}

ColliderChecker::~ColliderChecker()
{
}

void ColliderChecker::CheckCollider(Player * player, map<string, Base3DObject*>& objects)
{
	if (objects.empty() == true)
	{
		return;
	}

	queue<CollisionEvent> collisionEventQueue;

	bool sphereCollideOccurred = false;
	
	for(map<string, Base3DObject*>::iterator iter = objects.begin(); iter != objects.end(); ++iter)
	{
		if (CheckSphere((*iter).second, (Base3DObject*)player))
		{
			string obj1Tag;
			string obj2Tag;
			
			if (iter->second->GetObjectTag() == eObjTag::INTERACTABLE_OBJECT)
			{
				player->HandleInteractableObjectSphereCollideEvent(iter->second);
				iter->second->SetShaderPath(eShaderPath::ALL_PATH);
				sphereCollideOccurred = true;
			}

			if (CheckCube((*iter).second, (Base3DObject*)player, obj1Tag, obj2Tag))
			{
				CollisionEvent collisionEvent;
				collisionEvent.obj1 = iter->second;
				collisionEvent.obj1ColliderTag = obj1Tag;
				collisionEvent.obj2 = (Base3DObject*)player;
				collisionEvent.obj2ColliderTag = obj2Tag;
				collisionEventQueue.push(collisionEvent);
			}
			else
			{

			}
		}
	}

	if (sphereCollideOccurred == false && player->GetInteractingObject() != nullptr)
	{
		player->GetInteractingObject()->SetShaderPath(eShaderPath::PATH1);
		player->SetInteractingObject(nullptr);
	}
	
	for (map<string, Base3DObject*>::iterator iter = objects.begin(); iter != prev(objects.end(), 1); ++iter)
	{
		for (map<string, Base3DObject*>::iterator iter2 = next(iter, 1); iter2 != objects.end(); ++iter2)
		{
			if (CheckSphere((*iter).second, (*iter2).second))
			{
				string obj1Tag;
				string obj2Tag;

				if (CheckCube((*iter).second, (*iter2).second, obj1Tag, obj2Tag))
				{
					CollisionEvent collisionEvent;
					collisionEvent.obj1 = iter->second;
					collisionEvent.obj1ColliderTag = obj1Tag;
					collisionEvent.obj2 = iter2->second;
					collisionEvent.obj2ColliderTag = obj2Tag;
					collisionEventQueue.push(collisionEvent);
				}
				else
				{

				}
			}
		}
	}
	

	while(collisionEventQueue.empty() == false)
	{
		CollisionEvent collisionEvent = collisionEventQueue.front();
		collisionEventQueue.pop();
		collisionEvent.obj1->CollideHandle(collisionEvent.obj1, collisionEvent.obj1ColliderTag, collisionEvent.obj2, collisionEvent.obj2ColliderTag);
		collisionEvent.obj2->CollideHandle(collisionEvent.obj2, collisionEvent.obj2ColliderTag, collisionEvent.obj1, collisionEvent.obj1ColliderTag);
	}
}

bool ColliderChecker::CheckCube(IN Base3DObject * object1, IN Base3DObject * object2, OUT string& obj1Tag, OUT string& obj2Tag)
{
	for (map<string, ColliderCube*>::iterator iter = object1->GetColliderCube().begin(); iter != object1->GetColliderCube().end(); ++iter)
	{
		for (map<string, ColliderCube*>::iterator iter2 = object2->GetColliderCube().begin(); iter2 != object2->GetColliderCube().end(); ++iter2)
		{
			if(ColliderCube::IsCollision((*iter).second,(*iter2).second))
			{
				obj1Tag = iter->first;
				obj2Tag = iter2->first;
				return true;
			}
		}
	}
	return false;
}

bool ColliderChecker::CheckSphere(Base3DObject* object1, Base3DObject* object2)
{
	float distance;
	D3DXVECTOR3 diff;
	diff = object1->GetPos() - object2->GetPos();
	
	distance = D3DXVec3Length(&diff);

	if (distance <= object1->GetColliderSphere()->GetRadius() + object2->GetColliderSphere()->GetRadius())
	{
		return true;
	}

	return false;
}