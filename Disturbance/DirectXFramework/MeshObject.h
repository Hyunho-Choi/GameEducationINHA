#pragma once
#include "BaseObject.h"

class cMtlTex;

class MeshObject :
	public BaseObject
{
	LPD3DXMESH mMesh;

	
	map<string, cMtlTex*>mMapMtltex;
	vector<cMtlTex*> mMtlTex;
public:
	MeshObject();
	virtual ~MeshObject() = default;

	void Setup(string filename);
	void Update() override;
	void Render() override;
	
	void LoadMesh(char* szFolder, char* szFile);
	void LoadMtlLib(char* filePath, char* szFile);
};

