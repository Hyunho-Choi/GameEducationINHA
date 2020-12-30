#include "stdafx.h"
#include "CameraManager.h"
#include "Camera.h"
#include "KeyboardInputManager.h"
#include "MouseInputManager.h"

void CameraManager::SetCamera(Camera* newCamera)
{
	if(newCamera)
		mCurrentCamera = newCamera;
}

Camera* CameraManager::GetCurrentCamera()
{
	return mCurrentCamera;
}

void CameraManager::SetArcCamera(float period, D3DXVECTOR3* tragetPos, D3DXVECTOR3 startingPos, float speed, bool isClockWise)
{
	if (mEventCamera == nullptr)
	{
		gKeyboardInputManager->Disabled();
		gMouseInputManager->Disabled();

		mKeepCamera = gCurrentCamera;
	
		mEventCamera = new Camera;
		mEventCamera->SetEventCamera(true);
		mEventCamera->SetLifeTime(period);
		mEventCamera->SetTarget(tragetPos);
		mEventCamera->SetMoveSpeed(speed);
		mEventCamera->SetCameraDistance(D3DXVec3Length(&(*tragetPos - startingPos)));
		mEventCamera->SetStartPos(startingPos);

		mEventCamera->Setup();

		gCameraManager->SetCamera(mEventCamera);
	}
}

void CameraManager::EndEventCamera()
{
	mKeepCamera->Setup();
	gCameraManager->SetCamera(mKeepCamera);

	gKeyboardInputManager->Abled();
	gMouseInputManager->Abled();
	
	SAFE_DELETE(mEventCamera);
}
