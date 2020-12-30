#pragma once

#define gSkinnedMeshManger SkinnedMeshManager::GetInstance()
class SkinnedMesh;

class SkinnedMeshManager : public Singleton<SkinnedMeshManager>
{
private:
	map<string, SkinnedMesh*> m_mapSkinnedMesh;
public:

	SkinnedMesh* GetSkinnedMesh(char* szFoler, char* szFilename);
	void Destroy();
	
};

