#pragma once
class Scene;

class UIAssemblyLine
{
private :
	void MakeStartSceneUI(Scene* newScene);
	void MakeIngameSceneUI(Scene* newScene);
	void MakeLoadingSceneUI(Scene* newScene);
public :
	void MakeSceneUI(Scene* newScene);
};

