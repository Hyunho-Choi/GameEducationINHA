#pragma once
#include "Singleton.h"

#define gCameraManager CameraManager::GetInstance()
#define gCurrentCamera CameraManager::GetInstance()->GetCurrentCamera()

class Camera;

class CameraManager : public Singleton<CameraManager>
{
private:
	Camera* mCurrentCamera;
	Camera* mKeepCamera;
	Camera* mEventCamera;

public:
	void SetCamera(Camera* newCamera);
	Camera* GetCurrentCamera();
	
	void SetArcCamera(float period, D3DXVECTOR3* tragetPos, D3DXVECTOR3 startingPos, float speed, bool isClockWise);
	void EndEventCamera();
};

