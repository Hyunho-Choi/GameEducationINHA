#include "stdafx.h"
#include "SkinnedMesh.h"

#include <iostream>

#include "cAllocateHierarchy.h"
#include "SkinnedMeshManager.h"

SkinnedMesh::SkinnedMesh()
	: m_pRoot(NULL)
	, m_pAnimController(NULL)
	, m_fBlendTime(0.3f)
	, m_fPassedBlendTime(0.0f)
	, m_isAnimBlend(false)
	, m_vMin(0, 0, 0)
	, m_vMax(0, 0, 0)
{
	D3DXMatrixIdentity(&m_matWorldTM);
}


SkinnedMesh::~SkinnedMesh()
{
	cAllocateHierarchy ah;
	D3DXFrameDestroy(m_pRoot, &ah);
	SAFE_RELEASE(m_pAnimController);
}

SkinnedMesh::SkinnedMesh(char* szFolder, char* szFilename)
	: m_pRoot(NULL)
	, m_pAnimController(NULL)
	, m_fBlendTime(0.3f)
	, m_fPassedBlendTime(0.0f)
	, m_isAnimBlend(false)
	, m_vMin(0, 0, 0)
	, m_vMax(0, 0, 0)
{
	D3DXMatrixIdentity(&m_matWorldTM);
	SkinnedMesh* pSkinnedMesh = gSkinnedMeshManger->GetSkinnedMesh(szFolder, szFilename);

	m_pRoot = pSkinnedMesh->m_pRoot;
	
	m_vMin = pSkinnedMesh->m_vMin;
	m_vMax = pSkinnedMesh->m_vMax;

	pSkinnedMesh->m_pAnimController->CloneAnimationController(
		pSkinnedMesh->m_pAnimController->GetMaxNumAnimationOutputs(),
		pSkinnedMesh->m_pAnimController->GetMaxNumAnimationSets(),
		pSkinnedMesh->m_pAnimController->GetMaxNumTracks(),
		pSkinnedMesh->m_pAnimController->GetMaxNumEvents(),
		&m_pAnimController);
}

void SkinnedMesh::Load(char* szFolder, char* szFilename)
{
	cAllocateHierarchy ah;
	ah.SetFolder(szFolder);

	string sFullPath(szFolder);
	sFullPath += (string("/")) + string(szFilename);
	D3DXLoadMeshHierarchyFromXA(sFullPath.c_str(), D3DXMESH_MANAGED, gD3Device, &ah, NULL, (LPD3DXFRAME*)&m_pRoot, &m_pAnimController);
	m_vMax = ah.GetMax();
	m_vMin = ah.GetMin();

	SetupBoneMatrixPtrs(m_pRoot);
}

void SkinnedMesh::Destroy()
{
	cAllocateHierarchy ah;
	D3DXFrameDestroy((LPD3DXFRAME)m_pRoot, &ah);
}

void SkinnedMesh::UpdateAndRender()
{
	if(m_pAnimController)
	{
		cout << gTimeManager->GetDeltaTime() << endl;
		m_pAnimController->AdvanceTime(gTimeManager->GetDeltaTime(), NULL);
	}
	if(m_pRoot)
	{
		Update((ST_BONE*)m_pRoot, &m_matWorldTM);
		Render(m_pRoot);
	}
}

void SkinnedMesh::Update(ST_BONE* pCurrent, D3DXMATRIXA16* pMatWorld)
{
	if(pCurrent == NULL)
	{
		pCurrent = (ST_BONE*)m_pRoot;
	}
	pCurrent->CombinedTransformationMatrix = pCurrent->TransformationMatrix;

	if(pMatWorld != nullptr)
	{
		pCurrent->CombinedTransformationMatrix = pCurrent->CombinedTransformationMatrix *(*pMatWorld);
	}

	if (pCurrent->pFrameSibling)
		Update((ST_BONE*)pCurrent->pFrameSibling, pMatWorld);
	if (pCurrent->pFrameFirstChild)
		Update((ST_BONE*)pCurrent->pFrameFirstChild, &(pCurrent->CombinedTransformationMatrix));
}

void SkinnedMesh::SetRandomTrackPosition()
{
	m_pAnimController->SetTrackPosition(0, (rand() % 100 / 10.f));
}

void SkinnedMesh::SetTransform(D3DXMATRIXA16* pMat)
{
	m_matWorldTM = *pMat;
}


void SkinnedMesh::Setup(char* szFolder, char* szFile)
{
	string sFullPath(szFolder);
	sFullPath = sFullPath + string("/") + string(szFile);


	cAllocateHierarchy ah;

	ah.SetFolder(szFolder);

	D3DXLoadMeshHierarchyFromXA(sFullPath.c_str(), D3DXMESH_MANAGED, gD3Device, &ah, NULL, &m_pRoot, &m_pAnimController);

	//SetupBoneMatrixPtrs(m_pRoot);
}

void SkinnedMesh::Update()
{
	if(m_isAnimBlend)
	{
		m_fPassedBlendTime += gTimeManager->GetDeltaTime();
		if(m_fPassedBlendTime >= m_fBlendTime)
		{
			m_isAnimBlend = false;
			m_pAnimController->SetTrackWeight(0, 1.0f);
			m_pAnimController->SetTrackEnable(1, false);
		}
		else
		{
			float fWeight = m_fPassedBlendTime / m_fBlendTime;
			m_pAnimController->SetTrackWeight(0, fWeight);
			m_pAnimController->SetTrackWeight(1, 1.0f - fWeight);
		}
	}
	m_pAnimController->AdvanceTime(gTimeManager->GetDeltaTime(), NULL);
	
	Update((ST_BONE*)m_pRoot, nullptr);
	UpdateSkinnedMesh(m_pRoot);
}

void SkinnedMesh::Update(LPD3DXFRAME pFrame, LPD3DXFRAME pParent)
{
	if (pFrame == NULL)
		pFrame = m_pRoot;
	ST_BONE* pBone = (ST_BONE*)pFrame;
	pBone->CombinedTransformationMatrix = pBone->TransformationMatrix;

	if(pParent != nullptr)
	{
		pBone->CombinedTransformationMatrix = pBone->CombinedTransformationMatrix *(((ST_BONE*)pParent)->CombinedTransformationMatrix);
	}

	if (pFrame->pFrameSibling)
	{
		Update(pFrame->pFrameSibling, pParent);
	}
	if(pFrame->pFrameFirstChild)
	{
		Update(pFrame->pFrameFirstChild, pFrame);
	}
}

void SkinnedMesh::Render(LPD3DXFRAME pFrame)
{
	if (pFrame == NULL)
		pFrame = m_pRoot;
	ST_BONE * pBone = (ST_BONE*)pFrame;
	if(pBone->pMeshContainer)
	{
		ST_BONE_MESH * pBoneMesh = (ST_BONE_MESH*)pBone->pMeshContainer;
		if(pBoneMesh->MeshData.pMesh)
		{
			for (size_t i = 0; i < pBoneMesh->vecMtl.size(); ++i)
			{
				gD3Device->SetTexture(0, pBoneMesh->vecTex[i]);
				gD3Device->SetMaterial(&pBoneMesh->vecMtl[i]);
				pBoneMesh->MeshData.pMesh->DrawSubset(i);
			}
		}
	}

	if (pFrame->pFrameFirstChild)
		Render(pFrame->pFrameFirstChild);
	if (pFrame->pFrameSibling)
		Render(pFrame->pFrameSibling);                
}

void SkinnedMesh::SetupBoneMatrixPtrs(LPD3DXFRAME pFrame)
{
	if(pFrame && pFrame->pMeshContainer)
	{
		ST_BONE_MESH* pBoneMesh = (ST_BONE_MESH*)pFrame->pMeshContainer;

		if(pBoneMesh->pSkinInfo)
		{
			LPD3DXSKININFO pSkinInfo = pBoneMesh->pSkinInfo;
			DWORD dwNumBones = pSkinInfo->GetNumBones();
			for (DWORD i = 0; i < dwNumBones; ++i)
			{
				ST_BONE* pBone = (ST_BONE*)D3DXFrameFind(m_pRoot, pSkinInfo->GetBoneName(i));
				pBoneMesh->ppBoneMatrixPtrs[i] = &(pBone->CombinedTransformationMatrix);
			}
		}
	}

	if(pFrame->pFrameFirstChild)
	{
		SetupBoneMatrixPtrs(pFrame->pFrameFirstChild);
	}
	if(pFrame->pFrameSibling)
	{
		SetupBoneMatrixPtrs(pFrame->pFrameSibling);
	}
}

void SkinnedMesh::UpdateSkinnedMesh(LPD3DXFRAME pFrame)
{
	if(pFrame && pFrame->pMeshContainer)
	{
		ST_BONE_MESH* pBoneMesh = (ST_BONE_MESH*)pFrame->pMeshContainer;
		if(pBoneMesh->pSkinInfo)
		{
			LPD3DXSKININFO pSkinInfo = pBoneMesh->pSkinInfo;
			DWORD dwNumBones = pSkinInfo->GetNumBones();
			for (DWORD i = 0; i < dwNumBones; ++i)
			{
				pBoneMesh->pCurrentBoneMatrices[i] = pBoneMesh->pBoneOffsetMatrices[i] * *(pBoneMesh->ppBoneMatrixPtrs[i]);
			}
		}

		BYTE * src = NULL;
		BYTE * dest = NULL;

		if (pBoneMesh->pOrigMesh)
			pBoneMesh->pOrigMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&src);
		pBoneMesh->MeshData.pMesh->LockVertexBuffer(0, (void**)&dest);

		if(pBoneMesh->pSkinInfo)
			pBoneMesh->pSkinInfo->UpdateSkinnedMesh(pBoneMesh->pCurrentBoneMatrices, NULL, src, dest);
		
		pBoneMesh->MeshData.pMesh->UnlockVertexBuffer();
		if (pBoneMesh->pOrigMesh)
			pBoneMesh->pOrigMesh->UnlockVertexBuffer();
	}
	if (pFrame->pFrameFirstChild)
		UpdateSkinnedMesh(pFrame->pFrameFirstChild);
	if (pFrame->pFrameSibling)
		UpdateSkinnedMesh(pFrame->pFrameSibling);
}

void SkinnedMesh::SetAnimationIndex(int index)
{
	int num = m_pAnimController->GetNumAnimationSets();
	if (index > num) index = index % num;

	LPD3DXANIMATIONSET pAnimSet = NULL;
	//m_pAnimController->ResetTime();
	m_pAnimController->GetAnimationSet(index, &pAnimSet);
	m_pAnimController->SetTrackAnimationSet(0, pAnimSet);
	m_pAnimController->GetPriorityBlend();
}

void SkinnedMesh::SetAnimationIndexBlend(int nIndex)
{
	m_isAnimBlend = true;
	m_fPassedBlendTime = 0.0f;
	int num = m_pAnimController->GetNumAnimationSets();

	// 확인용 코드
	/*
	LPD3DXANIMATIONSET testset;
	LPCSTR outchar[128];
	m_pAnimController->GetAnimationSetByName("m_pAnimController", &testset);
	m_pAnimController->GetAnimationSet(0, &testset);
	cout << "애니메이션 숫자 : " << testset->GetNumAnimations() << endl;
	*/
	// 확인용 코드 


	if (nIndex > num) nIndex = nIndex % num;

	LPD3DXANIMATIONSET pPrevAnimSet = NULL;
	LPD3DXANIMATIONSET pNextAnimSet = NULL;

	D3DXTRACK_DESC stTrackDesc;
	m_pAnimController->GetTrackDesc(0, &stTrackDesc);
	m_pAnimController->GetTrackAnimationSet(0, &pPrevAnimSet);
	m_pAnimController->SetTrackAnimationSet(1, pPrevAnimSet);
	m_pAnimController->SetTrackDesc(1, &stTrackDesc);

	m_pAnimController->GetAnimationSet(nIndex, &pNextAnimSet);
	m_pAnimController->SetTrackAnimationSet(0, pNextAnimSet);
	m_pAnimController->SetTrackPosition(0, 0.0f);

	m_pAnimController->SetTrackWeight(0, 0.0f);
	m_pAnimController->SetTrackWeight(1, 1.0f);

	m_pAnimController->SetTrackSpeed(0, 1.f);

	SAFE_RELEASE(pPrevAnimSet);
	SAFE_RELEASE(pNextAnimSet);

	m_animationStartTime = GetTickCount();
}

void SkinnedMesh::SetCurrentAnimationSpeed(float time)
{
	m_pAnimController->SetTrackSpeed(0, time);
}

void SkinnedMesh::PrintAnimationList()
{
	int GetAnimationNum = m_pAnimController->GetNumAnimationSets();
	LPD3DXANIMATIONSET animSet;
	for (int i = 0; i < GetAnimationNum; ++i)
	{
		m_pAnimController->GetAnimationSet(i, &animSet);
		cout << "AnimName : " << animSet->GetName() << " Index : " << i << endl;
	}
}
