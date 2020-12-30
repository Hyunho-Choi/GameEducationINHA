#include "stdafx.h"
#include "Static3DObject.h"
#include "ModelLoader.h"
#include "ColliderCube.h"

void render()
{
	
}

Static3DObject::Static3DObject()
	: mpMesh(nullptr)
	, mpMaterials(nullptr)
	, mpTextures(nullptr)
	, mNumMaterials(0)
{
	mObjectTag = eObjTag::STATIC_OBJECT;
}


Static3DObject::~Static3DObject()
{
	SAFE_RELEASE(mpMesh);
}

void Static3DObject::Setup(
	string folder, string fileName, eTypeTag typeTag)
{
	Base3DObject::Setup();
	gModelLoader->LoadXfile(folder, fileName, mpMesh, &mpMaterials, &mpTextures, mNumMaterials);
	
	mTypeTag = typeTag;

	if (mTypeTag == eTypeTag::DOOR)
	{
		mStateTag = eStateTag::DOOR_CLOSE;
	}
}

void Static3DObject::Update()
{
	/*if (mTypeTag == eTypeTag::DOOR)
	{
		static float angle = 0.f;
		if (mStateTag == eStateTag::DOOR_CLOSE)
		{
			if (angle > 0)
			{
				angle -= 0.05;
				if (angle < 0)
					angle = 0.f;
			}
			D3DXQUATERNION rotation;
			D3DXQuaternionRotationAxis(&rotation, &D3DXVECTOR3(0,1,0), angle * D3DX_PI);
			mRot = rotation;
		}
		else if (mStateTag == eStateTag::DOOR_OPEN)
		{
			if (angle < 0.5)
				angle += 0.05;
			D3DXQUATERNION rotation;
			D3DXQuaternionRotationAxis(&rotation, &D3DXVECTOR3(0, 1, 0), angle * D3DX_PI);
			mRot = rotation;
		}
	}*/
	Base3DObject::Update();
}

void Static3DObject::Render()
{
	Base3DObject::Render();

	switch (mTypeTag)
	{
		
		case eTypeTag::ITEM :
		{
			if (!gCullingManager->CheckFrustumCulling(this))
			{
				return;
			}
			gShader->GetInstance()->RenderWithItemShader(bind(&Static3DObject::RenderMesh, this), mShaderPath);
			//RenderMesh();
		}
		break;
		case eTypeTag::WALL:
		{
			if (!gCullingManager->CheckFrustumCulling(this))
			{
				return;
			}
			gShader->GetInstance()->RenderWithWallShader(bind(&Static3DObject::RenderMesh, this));
			//RenderMesh();
		}
		break;
		case eTypeTag::FLOOR:
		{
			gShader->GetInstance()->RenderWithFloorShader(bind(&Static3DObject::RenderMesh, this));
			//RenderMesh();
		}
		break;
		case eTypeTag::PORTAL:
		{
			gShader->GetInstance()->RenderWithPortalShader(bind(&Static3DObject::RenderMesh, this));
			//RenderMesh();
		}
		break;
		default:
		{
		}
		break;

	}
}

void Static3DObject::RenderMesh()
{
	for (DWORD i = 0; i < mNumMaterials; ++i)
	{
		gD3Device->SetMaterial(&mpMaterials[i]);
		//gD3Device->SetTexture(0, mpTextures[i]);
		mpMesh->DrawSubset(i);
	}
}


LPD3DXEFFECT Static3DObject::LoadShader(const char* filename)
{
	LPD3DXEFFECT ret = NULL;
	LPD3DXBUFFER pError = NULL;
	DWORD dwShaderFlags = 0;

#if _DEBUG
	dwShaderFlags |= D3DXSHADER_DEBUG;
#endif

	D3DXCreateEffectFromFileA(gD3Device, filename,
		NULL, NULL, dwShaderFlags, NULL, &ret, &pError);

	// 쉐이더 로딩에 실패한 경우 output창에 쉐이더
	// 컴파일 에러를 출력한다.
	if (!ret && pError)
	{
		int size = pError->GetBufferSize();
		void *ack = pError->GetBufferPointer();

		/*
		if (ack)
		{
		char* str = new char[size];
		sprintf(str, (const char*)ack, size);
		OutputDebugStringA(str);
		delete[] str;
		}
		*/
	}

	return ret;
}
