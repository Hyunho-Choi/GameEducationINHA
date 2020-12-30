#pragma once

#include "Singleton.h"
#include <fbxsdk.h>

#pragma comment(lib, "libfbxsdk-mt.lib")
#pragma comment(lib, "libxml2-mt.lib")
#pragma comment(lib, "zlib-mt.lib")

#define gFbxLoader FBXLoader::GetInstance()

struct CharacterVertex;
class SkinnedData;

struct BoneIndexAndWeight
{
	BYTE BoneIndices;
	float BoneWeight;

	bool operator < (const BoneIndexAndWeight& rhs)
	{
		return (BoneIndices > rhs.BoneWeight);
	}
};

struct CtrlPoint
{
	D3DXVECTOR3 Position;
	std::vector<BoneIndexAndWeight> BoneInfo;
	std::string BoneName;

	CtrlPoint()
	{
		BoneInfo.reserve(4);
	}

	void SortBlendingInfoByWeight()
	{
		std::sort(BoneInfo.begin(), BoneInfo.end());
	}
};

class FBXLoader : public Singleton<FBXLoader>
{
private:
	FBXLoader(); friend Singleton;
	~FBXLoader();
	FbxManager* mFbxManager;
	FbxScene* mFbxScene;
	FbxNode* mFbxNode;
	FbxMesh* mFbxMesh;
private:
	/*void loadNode(FbxNode* node);
	void GetFbxInfo(FbxNode* Node);
	void processControlPoints(FbxMesh* mesh, OUT vector<D3DXVECTOR3>& positions);
	D3DXVECTOR3 readNormal(const FbxMesh* mesh, int controlPointIndex, int vertexCounter);
	D3DXVECTOR3 readBinormal(const FbxGeometryElementBinormal* vertexBinormal, int controlPointIndex, int vertexCounter);
	D3DXVECTOR3 readTangent(const FbxGeometryElementTangent* mesh, int controlPointIndex, int vertexCounter);
	D3DXVECTOR2 readTexUV(const FbxMesh* mesh, int controlPointIndex, int uvIndex);

	D3DXVECTOR3 readNormalByControlPoint(const FbxGeometryElementNormal* vertexNormal, int controlPointIndex);
	D3DXVECTOR3 readNormalByPolygonVertex(const FbxGeometryElementNormal* vertexNormal, int vertexCounter);
	D3DXVECTOR3 readBinormalByControlPoint(const FbxGeometryElementBinormal* vertexBinormal, int controlPointIndex);
	D3DXVECTOR3 readBinormalByPolygonVertex(const FbxGeometryElementBinormal* vertexBinormal, int vertexCounter);
	D3DXVECTOR3 readTangentByControlPoint(const FbxGeometryElementTangent* vertexNormal, int controlPointIndex);
	D3DXVECTOR3 readTangentByPolygonVertex(const FbxGeometryElementTangent* vertexNormal, int vertexCounter);*/
	void GetControlPoints(FbxNode* pFbxRootNode);
	void GetVerticesAndIndice(FbxMesh* pMesh, std::vector<CharacterVertex>& outVertexVector,
		std::vector<uint32_t>& outIndexVector, SkinnedData* outSkinnedData);
	void GetMaterials(FbxNode* node, std::vector<D3DXMATERIAL>& outMaterial);
	void GetMaterialAttribute(FbxSurfaceMaterial* material, D3DXMATERIAL& outMaterial);
	void GetMaterialTexture(FbxSurfaceMaterial* material, D3DXMATERIAL& mat);
public:
	LPD3DXMESH loadFbx(string fileName); // TODO DirectX에 알맞는 아웃풋으로 변경해야함
	
private:
	std::unordered_map<std::int32_t, CtrlPoint*> mControlPoints;
	std::vector<std::string> mBoneName;

	// skinnedData Output
	/*std::vector<int> mBoneHierarchy;
	std::vector<D3DXMATRIXA16> mBoneOffsets;
	std::unordered_map<std::string, AnimationClip> mAnimations;*/
};

