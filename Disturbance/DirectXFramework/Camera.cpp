#include "stdafx.h"
#include "Camera.h"


Camera::Camera()
	: mCameraDistance(75.0f)
	, mLookAt(0, 0, 0)
	, mUp(0, 1, 0)
	, mRight(0, 0 ,0)
	, mTarget(nullptr)
	, mEye(0, 50, mCameraDistance)
	, mCamRotAngle(0, D3DX_PI * 0.25f, 0)
	, bEventCamera(false)
{
	mStartPos = D3DXVECTOR3(0, 50, -mCameraDistance);
}


Camera::~Camera()
{

}

void Camera::Setup()
{
	D3DVIEWPORT9 viewPort;
	gD3Device->GetViewport(&viewPort);
	D3DXMATRIXA16 matProj;
	//D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4.0F, viewPort.Width / (float)(viewPort.Height), 1.0f, 1000.0f); // 원근 투영
	D3DXMatrixOrthoLH(&matProj, ORTHO_WIDTH, ORTHO_WIDTH * viewPort.Height / viewPort.Width, 1.f, 1000.f); // 직교 투영
	gD3Device->SetTransform(D3DTS_PROJECTION, &matProj);

	gCullingManager->Setup();
}

void Camera::SetupPre()
{
	D3DVIEWPORT9 viewPort;
	gD3Device->GetViewport(&viewPort);
	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4.0F, viewPort.Width / (float)(viewPort.Height), 0.5f, 1000.0f); // 원근 투영
	//D3DXMatrixOrthoLH(&matProj, ORTHO_WIDTH/3, ORTHO_WIDTH / 3 * viewPort.Height / viewPort.Width, 1.f, 1000.f); // 직교 투영
	gD3Device->SetTransform(D3DTS_PROJECTION, &matProj);
}

void Camera::SetTarget(D3DXVECTOR3* target)
{
	mTarget = target;
}

void Camera::Update()
{
	if (bEventCamera)
	{
		// 라이프타임 체크
		if (mLifeTime <= 0)
		{
			gCameraManager->EndEventCamera();
			return;
		}
		mLifeTime -= gDeltaTime;
		mCamRotAngle.y += gDeltaTime * mMoveSpeed;
	}

	D3DXMATRIXA16 matR, matRX, matRY;
	D3DXMatrixRotationX(&matRX, mCamRotAngle.x);
	D3DXMatrixRotationY(&matRY, mCamRotAngle.y);
	matR = matRX * matRY;

	mEye = mStartPos;
	D3DXVec3TransformCoord(&mEye, &mEye, &matR);
	
	if (mTarget)
	{ 
		mLookAt = *mTarget;
		mEye += *mTarget;
	}

	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(&matView, &mEye, &mLookAt, &mUp);

	gD3Device->SetTransform(D3DTS_VIEW, &matView);
}

bool Camera::Update(eEventName eventName, void* parameter)
{
	switch (eventName)
	{
		case eEventName::MOUSE_L_DRAG:
			{
				POINT mouseDeltaPosition = *(POINT*)parameter;

				mCamRotAngle.y += (mouseDeltaPosition.x / 100.0f);
				mCamRotAngle.x += (mouseDeltaPosition.y / 100.0f);

				if (mCamRotAngle.x < -D3DX_PI / 2.0f + 0.0001f)
				{
					mCamRotAngle.x = -D3DX_PI / 2.0f + 0.0001f;
				}

				if (mCamRotAngle.x > D3DX_PI / 2.0f - 0.0001f)
				{
					mCamRotAngle.x = D3DX_PI / 2.0f - 0.0001f;
				}
			}
			break;
		case eEventName::MOUSE_WHEEL_SCROLL:
			{
				mCameraDistance -= *(float*)parameter;
				if (mCameraDistance < 75.0f)
				{
					mCameraDistance = 75.0f;
				}	
			}
			break;
		default:
			break;
	}
	return true;
}

D3DXVECTOR3 Camera::GetPickingPosition(POINT& mousePos)
{
	D3DXVECTOR3 ray = mLookAt - mEye;
	D3DXVec3Cross(&mRight, &mUp, &ray);
	D3DXVec3Normalize(&mRight, &mRight);
	D3DXVec3Cross(&mCameraUp,  &ray, &mRight );
	D3DXVec3Normalize(&mCameraUp, &mCameraUp);
	
	D3DVIEWPORT9 viewPort;
	gD3Device->GetViewport(&viewPort);

	D3DXVECTOR3 mouseWorldPosition = mEye;
	
	mouseWorldPosition += ((viewPort.Height * 0.5 - (float)mousePos.y) * (ORTHO_WIDTH / (float)viewPort.Width)) * mRight;
	mouseWorldPosition += (((float)mousePos.x - viewPort.Width * 0.5) * (ORTHO_WIDTH / (float)viewPort.Width)) * mCameraUp;

	D3DXVec3Normalize(&ray, &ray);
	mouseWorldPosition += abs(mouseWorldPosition.y / ray.y) * ray;
	
	return mouseWorldPosition;
}

void Camera::SetEventCamera(bool setEvent)
{
	bEventCamera = setEvent;
}

bool Camera::GetEventCamera()
{
	return bEventCamera;
}

void Camera::SetLifeTime(float lifeTime)
{
	mLifeTime = lifeTime;
}

float Camera::GetLifeTime()
{
	return mLifeTime;
}

void Camera::SetYCamRotAngle(float CamRotAngle)
{
	mCamRotAngle.y = CamRotAngle;
}

float Camera::GetYCamRotAngle()
{
	return mCamRotAngle.y;
}

void Camera::SetStartPos(D3DXVECTOR3 StartPos)
{
	mStartPos = StartPos;
}

D3DXVECTOR3 Camera::GetStartPos()
{
	return mStartPos;
}

void Camera::SetMoveSpeed(float MoveSpeed)
{
	mMoveSpeed = MoveSpeed;
}

float Camera::GetMoveSpeed()
{
	return mMoveSpeed;
}

void Camera::SetCameraDistance(float CameraDistance)
{
	mCameraDistance = CameraDistance;
}

float Camera::GetCameraDistance()
{
	return mCameraDistance;
}
