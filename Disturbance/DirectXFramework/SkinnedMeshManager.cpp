#include "stdafx.h"
#include "SkinnedMeshManager.h"
#include "SkinnedMesh.h"


SkinnedMesh* SkinnedMeshManager::GetSkinnedMesh(char* szFoler, char* szFilename)
{
	string sFullPath(szFoler);
	sFullPath += string("/") + string(szFilename);

	if(m_mapSkinnedMesh.find(sFullPath) == m_mapSkinnedMesh.end())
	{
		SkinnedMesh*p = new SkinnedMesh();
		p->Load(szFoler, szFilename);
		m_mapSkinnedMesh[sFullPath] = p;
	}
	return m_mapSkinnedMesh[sFullPath];
}

void SkinnedMeshManager::Destroy()
{
	for (auto it : m_mapSkinnedMesh)
	{
		it.second->Destroy();
		SAFE_DELETE(it.second);
	}
}
