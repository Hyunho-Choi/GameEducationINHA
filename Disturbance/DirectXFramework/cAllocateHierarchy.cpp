#include "stdafx.h"
#include "cAllocateHierarchy.h"
#include <iostream>

cAllocateHierarchy::cAllocateHierarchy()
	: m_vMax(0,0,0)
	, m_vMin(0,0,0)
{
}

cAllocateHierarchy::~cAllocateHierarchy()
{
}

STDMETHODIMP cAllocateHierarchy::CreateFrame(THIS_ LPCSTR Name, LPD3DXFRAME* ppNewFrame)
{
	ST_BONE* pBone = new ST_BONE;
	ZeroMemory(pBone, sizeof(ST_BONE));
	if(Name)
	{
		pBone->Name = new char[lstrlenA(Name) + 1];
		UINT	length = lstrlenA(Name) + 1;
		strcpy_s(pBone->Name, length * sizeof(CHAR), Name);
	}
	D3DXMatrixIdentity(&pBone->TransformationMatrix);
	D3DXMatrixIdentity(&pBone->CombinedTransformationMatrix);

	*ppNewFrame = pBone;
	return S_OK;
}

STDMETHODIMP cAllocateHierarchy::CreateMeshContainer(THIS_
	LPCSTR Name,
	CONST D3DXMESHDATA *pMeshData,
	CONST D3DXMATERIAL *pMaterials,
	CONST D3DXEFFECTINSTANCE *pEffectInstances,
	DWORD NumMaterials,
	CONST DWORD *pAdjacency,
	LPD3DXSKININFO pSkinInfo,
	LPD3DXMESHCONTAINER *ppNewMeshContainer)
{
	ST_BONE_MESH* pBoneMesh = new ST_BONE_MESH;
	ZeroMemory(pBoneMesh, sizeof(ST_BONE_MESH));
	for (DWORD i = 0; i < NumMaterials; ++i)
	{
		pBoneMesh->vecMtl.push_back(pMaterials[i].MatD3D);
		string sFullPath = m_sFolder;
		if (pMaterials[i].pTextureFilename != nullptr)
		{
			sFullPath = sFullPath + string("/") + string(pMaterials[i].pTextureFilename);
			pBoneMesh->vecTex.push_back(gTextureManager->GetTexture(sFullPath));
		}
		else
		{
			pBoneMesh->vecTex.push_back(NULL);
		}
	}
	if(pSkinInfo)
		pSkinInfo->AddRef();
	pBoneMesh->pSkinInfo = pSkinInfo;
	pMeshData->pMesh->AddRef();
	pBoneMesh->MeshData.pMesh = pMeshData->pMesh;

	if(pMeshData && pMeshData->pMesh)
	{
		D3DXVECTOR3 vMin(0, 0, 0), vMax(0,0,0);
		LPVOID pV = NULL;
		pMeshData->pMesh->LockVertexBuffer(0, &pV);
		D3DXComputeBoundingBox((D3DXVECTOR3*)pV, pMeshData->pMesh->GetNumVertices(), D3DXGetFVFVertexSize(pMeshData->pMesh->GetFVF()), &vMin, &vMax);
		D3DXVec3Minimize(&m_vMin, &m_vMin, &vMin);
		D3DXVec3Maximize(&m_vMax, &m_vMax, &vMax);
		pMeshData->pMesh->UnlockVertexBuffer();
	}

	pMeshData->pMesh->CloneMeshFVF(
		pMeshData->pMesh->GetOptions(),
		pMeshData->pMesh->GetFVF(),
		gD3Device,
		&pBoneMesh->pOrigMesh
	);
	if (pSkinInfo != nullptr)
	{
		DWORD dwNumBones = pSkinInfo->GetNumBones();
		pBoneMesh->ppBoneMatrixPtrs = new D3DXMATRIX * [dwNumBones];
		pBoneMesh->pBoneOffsetMatrices = new D3DXMATRIX[dwNumBones];
		pBoneMesh->pCurrentBoneMatrices = new D3DXMATRIX[dwNumBones];

		for (DWORD i = 0; i < dwNumBones; ++i)
		{
			pBoneMesh->pBoneOffsetMatrices[i] = *(pSkinInfo->GetBoneOffsetMatrix(i));
		}
	}
	*ppNewMeshContainer = pBoneMesh;
	return S_OK;
}
STDMETHODIMP cAllocateHierarchy::DestroyFrame(THIS_ LPD3DXFRAME pFrameToFree)
{
	ST_BONE * pBone = (ST_BONE *)pFrameToFree;
	SAFE_DELETE_ARRAY(pBone->Name);
	SAFE_DELETE(pFrameToFree);
	return S_OK;
}
STDMETHODIMP cAllocateHierarchy::DestroyMeshContainer(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree)
{
	ST_BONE_MESH * pBoneMesh = (ST_BONE_MESH*)pMeshContainerToFree;

	SAFE_RELEASE(pBoneMesh->pSkinInfo);
	SAFE_RELEASE(pBoneMesh->MeshData.pMesh);
	SAFE_RELEASE(pBoneMesh->pOrigMesh);
	
	SAFE_DELETE_ARRAY(pBoneMesh->pCurrentBoneMatrices);
	SAFE_DELETE_ARRAY(pBoneMesh->ppBoneMatrixPtrs);
	SAFE_DELETE_ARRAY(pBoneMesh->pBoneOffsetMatrices);

	SAFE_DELETE(pBoneMesh);
	return S_OK;
}