#pragma once

#include "UIAssemblyLine.h"
#include "GameObjectAssemblyLine.h"

enum class eSceneName;
class Scene;
class Static3DObject;
class Room;

class SceneFactory
{
	UIAssemblyLine mUILine;
	GameObjectAssemblyLine mGameObjectLine;
public:
	SceneFactory();
	~SceneFactory();

	Scene* CreateScene(eSceneName eSceneName);
};

