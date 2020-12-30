#include "stdafx.h"
#include "SceneFactory.h"
#include "SceneCenter.h"
#include "Scene.h"
#include "QuarterMap.h"
#include "Player.h"
#include "Static3DObject.h"
#include "RoomCenter.h"
#include "Room.h"
#include "CollideHandle.h"
#include "Portal.h"
#include "Interactable3DObject.h"
#include "Inventory.h"
#include "UICellGrid.h"
#include "Door.h"
#include "FlashLight.h"

SceneFactory::SceneFactory()
{
}


SceneFactory::~SceneFactory()
{
}

Scene* SceneFactory::CreateScene(eSceneName eSceneName)
{
	Scene * newScene = new Scene(eSceneName);
	mUILine.MakeSceneUI(newScene);
	mGameObjectLine.MakeGameObject(newScene);
	return newScene;
}

