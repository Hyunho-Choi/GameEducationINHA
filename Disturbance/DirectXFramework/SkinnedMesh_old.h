#pragma once

struct FrameEx;
struct KeyFrame;
class AllocateHierarchyEx;

class SkinnedMesh
{
	D3DXFRAME* rootFrame;
	void Render(D3DXFRAME* node, D3DXMATRIX& parentWorldMat);
public :
	SkinnedMesh();
	void Render();
};

struct FrameEx : public D3DXFRAME
{
	D3DXMATRIXA16 CombinedTransform;

	void CombineTransforms(FrameEx* frame, D3DXMATRIX& parentMat)
	{
		D3DXMATRIX& localMat = frame->TransformationMatrix;
		D3DXMATRIX& combinedMat = frame->CombinedTransform;

		combinedMat = localMat * parentMat;

		if (frame->pFrameSibling)
		{
			CombineTransforms((FrameEx*)frame->pFrameSibling, parentMat);
		}
		if (frame->pFrameFirstChild)
		{
			CombineTransforms((FrameEx*)frame->pFrameFirstChild, combinedMat);
		}
	}
};

struct KeyFrame
{
	float time;
	D3DXQUATERNION	rot;
	D3DXVECTOR3		scale;
	D3DXVECTOR3		translation;

	void InterpolateBone(KeyFrame& key0, KeyFrame& key1, OUT D3DXMATRIXA16& interpolatedMat)
	{
		float t0 = key0.time;
		float t1 = key1.time;
		float lerpTime = (gTimeManager->GetDeltaTime() - t0) / (t1 - t0);

		D3DXVECTOR3 lerpedT;
		D3DXVECTOR3 lerpedS;
		D3DXQUATERNION lerpedR;
		D3DXVec3Lerp(&lerpedT, &key0.translation, &key1.translation, lerpTime);
		D3DXVec3Lerp(&lerpedS, &key0.scale, &key1.scale, lerpTime);
		D3DXQuaternionSlerp(&lerpedR, &key0.rot, &key1.rot, lerpTime);

		D3DXMATRIX scaleMat;
		D3DXMATRIX rotMat;
		D3DXMATRIX translationMat;

		D3DXMatrixScaling(&scaleMat, lerpedS.x, lerpedS.y, lerpedS.z);
		D3DXMatrixScaling(&translationMat, lerpedT.x, lerpedT.y, lerpedT.z);
		D3DXMatrixRotationQuaternion(&rotMat, &lerpedR);

		interpolatedMat = scaleMat * rotMat * translationMat;
	}
};

class AllocateHierarchyEx : public ID3DXAllocateHierarchy
{
public :
	wstring folderPath;
private :
	HRESULT AllocateName(LPCSTR Name, LPSTR* pNewName);
	HRESULT STDMETHODCALLTYPE CreateFrame(LPCSTR Name, LPD3DXFRAME* ppNewFrame) override;
	HRESULT STDMETHODCALLTYPE CreateMeshContainer(LPCSTR Name, const D3DXMESHDATA* pMeshData, const D3DXMATERIAL* pMaterials, const D3DXEFFECTINSTANCE* pEffectInstances, DWORD NumMaterials, const DWORD* pAdjacency, LPD3DXSKININFO pSkinInfo, LPD3DXMESHCONTAINER* ppNewMeshContainer) override;
	HRESULT STDMETHODCALLTYPE DestroyFrame(LPD3DXFRAME pFrameToFree) override;
	HRESULT STDMETHODCALLTYPE DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerToFree) override;
};

struct MeshContainerEx : public D3DXMESHCONTAINER
{
	LPDIRECT3DTEXTURE9* ppTextures;       // array of textures, entries are NULL if no texture specified    

	// SkinMesh info             
	LPD3DXMESH pOrigMesh;
	LPD3DXATTRIBUTERANGE pAttributeTable;
	DWORD NumAttributeGroups;
	DWORD NumInfl;
	LPD3DXBUFFER pBoneCombinationBuf;
	D3DXMATRIX** ppBoneMatrixPtrs;
	D3DXMATRIX* pBoneOffsetMatrices;
	DWORD NumPaletteEntries;
	bool UseSoftwareVP;
	DWORD iAttributeSW;     // used to denote the split between SW and HW if necessary for non-indexed skinning
};
