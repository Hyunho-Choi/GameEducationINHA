#include "stdafx.h"
#include "MeshObject.h"
#include "cMtlTex.h"


MeshObject::MeshObject()
	: mMesh(NULL)

{
}


void MeshObject::Setup(string filename)
{
}


void MeshObject::Render()
{
	D3DXMATRIXA16 matWorld, matS, matR, matT;
	
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matS);
	D3DXMatrixIdentity(&matR);
	D3DXMatrixIdentity(&matT);

	D3DXMatrixScaling(&matS, 1, 1, 1);
	
	matWorld = matS * matR * matT;
	
	gD3Device->SetTransform(D3DTS_WORLD, &matWorld);

	for (size_t i = 0; i < mMtlTex.size(); ++i)
	{
		gD3Device->SetMaterial(&mMtlTex[i]->GetMaterial());
		gD3Device->SetTexture(0, mMtlTex[i]->GetTexture());
		mMesh->DrawSubset(i);
	}
}


void MeshObject::Update()
{
	Base3DOb
}



void MeshObject::LoadMesh(char* szFolder, char* szFile)
{
	vector<DWORD>		vecAttrBuf;
	vector<D3DXVECTOR3> vecV;
	vector<D3DXVECTOR2> vecVT;
	vector<D3DXVECTOR3> vecVN;
	vector<Vertex>		vecVertex;

	string sFullPath(szFolder);
	sFullPath += (string("/") + string(szFile));


	FILE *fp;
	fopen_s(&fp, sFullPath.c_str(), "r");

	string sMtlName;
	while (true)
	{
		if (feof(fp)) break;

		char szTemp[1024];
		fgets(szTemp, 1024, fp);

		if (szTemp[0] == '#')
		{
			continue;
		}
		else if (szTemp[0] == 'm')
		{
			char szMtlFile[1024];
			sscanf_s(szTemp, "%*s %s", szMtlFile, 1024);
			LoadMtlLib(szFolder, szMtlFile);
		}
		else if (szTemp[0] == 'g')
		{

		}
		else if (szTemp[0] == 'v')
		{
			if (szTemp[1] == ' ')
			{
				float x, y, z;
				sscanf_s(szTemp, "%*s %f %f %f", &x, &y, &z);
				vecV.push_back(D3DXVECTOR3(x, y, z));
			}
			else if (szTemp[1] == 't')
			{
				float u, v;
				sscanf_s(szTemp, "%*s %f %f %*f", &u, &v);
				vecVT.push_back(D3DXVECTOR2(u, v));
			}
			else if (szTemp[1] == 'n')
			{
				float x, y, z;
				sscanf_s(szTemp, "%*s %f %f %f", &x, &y, &z);
				vecVN.push_back(D3DXVECTOR3(x, y, z));
			}
		}
		else if (szTemp[0] == 'u')
		{
			char szMtlName[1024];
			sscanf_s(szTemp, "%*s %s", szMtlName, 1024);
			sMtlName = string(szMtlName);
		}
		else if (szTemp[0] == 'f')
		{
			int nIndex[3][3];
			sscanf_s(szTemp, "%*s %d/%d/%d %d/%d/%d %d/%d/%d",
				&nIndex[0][0], &nIndex[0][1], &nIndex[0][2],
				&nIndex[1][0], &nIndex[1][1], &nIndex[1][2],
				&nIndex[2][0], &nIndex[2][1], &nIndex[2][2]
			);

			for (int i = 0; i < 3; ++i)
			{
				Vertex v;
				v.Pos = vecV[nIndex[i][0] - 1];
				v.TexUV = vecVT[nIndex[i][1] - 1];
				v.Normal = vecVN[nIndex[i][2] - 1];
				vecVertex.push_back(v);
			}

			vecAttrBuf.push_back(mMapMtltex[sMtlName]->GetAttrID());
		}
	}

	fclose(fp);


	mMtlTex.resize(mMapMtltex.size());
	for each(auto it in mMapMtltex)
	{
		mMtlTex[it.second->GetAttrID()] = it.second;
	}

	LPD3DXMESH	pMesh = NULL;
	D3DXCreateMeshFVF(vecAttrBuf.size(), vecVertex.size(),
		D3DXMESH_MANAGED, Vertex::FVF, gD3Device,
		&pMesh);

	Vertex * pV = NULL;
	pMesh->LockVertexBuffer(0, (LPVOID*)&pV);
	memcpy(pV, &vecVertex[0], vecVertex.size() * sizeof(Vertex));
	pMesh->UnlockVertexBuffer();

	WORD* pI = NULL;
	pMesh->LockIndexBuffer(0, (LPVOID*)& pI);
	for (int i = 0; i < vecVertex.size(); ++i)
		pI[i] = i;
	pMesh->UnlockIndexBuffer();

	DWORD* pA = NULL;
	pMesh->LockAttributeBuffer(0, &pA);
	memcpy(pA, &vecAttrBuf[0], vecAttrBuf.size() * sizeof(DWORD));
	pMesh->UnlockAttributeBuffer();

	vector<DWORD> vecAdj(vecVertex.size());
	pMesh->GenerateAdjacency(0.0f, &vecAdj[0]);

	pMesh->OptimizeInplace(D3DXMESHOPT_ATTRSORT |
		D3DXMESHOPT_COMPACT |
		D3DXMESHOPT_VERTEXCACHE,
		&vecAdj[0],
		0, 0, 0);

	mMapMtltex.clear();

	mMesh = pMesh;

}


void MeshObject::LoadMtlLib(char* szFolder , char * szFile)
{
	string sFullPath(szFolder);
	sFullPath += (string("/") + string(szFile));


	FILE *fp;
	fopen_s(&fp, sFullPath.c_str(), "r");

	string sMtlName;
	int nCnt = 0; 
	while (true)
	{
		if (feof(fp)) break;

		char szTemp[1024];
		fgets(szTemp, 1024, fp);
		if (szTemp[0] == '#')
		{
			continue;
		}
		else if (szTemp[0] == 'n')
		{
			char szMtlName[1024];
			sscanf_s(szTemp, "%*s %s", szMtlName, 1024);
			sMtlName = string(szMtlName);
			if (mMapMtltex.find(sMtlName) == mMapMtltex.end())
			{
				mMapMtltex[sMtlName] = new cMtlTex;
				mMapMtltex[sMtlName]->SetAttrID(nCnt++);
			}

		}
		else if (szTemp[0] == 'K')
		{
			if (szTemp[1] == 'a')
			{
				float r, g, b;
				sscanf_s(szTemp, "%*s %f %f %f", &r, &g, &b);
				mMapMtltex[sMtlName]->GetMaterial().Ambient.r = r;
				mMapMtltex[sMtlName]->GetMaterial().Ambient.g = g;
				mMapMtltex[sMtlName]->GetMaterial().Ambient.b = b;
				mMapMtltex[sMtlName]->GetMaterial().Ambient.a = 1.0f;
			}
			else if (szTemp[1] == 'd')
			{
				float r, g, b;
				sscanf_s(szTemp, "%*s %f %f %f", &r, &g, &b);
				mMapMtltex[sMtlName]->GetMaterial().Diffuse.r = r;
				mMapMtltex[sMtlName]->GetMaterial().Diffuse.g = g;
				mMapMtltex[sMtlName]->GetMaterial().Diffuse.b = b;
				mMapMtltex[sMtlName]->GetMaterial().Diffuse.a = 1.0f;
			}
			else if (szTemp[1] == 's')
			{
				float r, g, b;
				sscanf_s(szTemp, "%*s %f %f %f", &r, &g, &b);
				mMapMtltex[sMtlName]->GetMaterial().Specular.r = r;
				mMapMtltex[sMtlName]->GetMaterial().Specular.g = g;
				mMapMtltex[sMtlName]->GetMaterial().Specular.b = b;
				mMapMtltex[sMtlName]->GetMaterial().Specular.a = 1.0f;
			}
		}
		else if (szTemp[0] == 'd')
		{
			float d;
			sscanf_s(szTemp, "%*s %f", &d);
			mMapMtltex[sMtlName]->GetMaterial().Power = d;
		}
		else if (szTemp[0] == 'm')
		{
			char szTexFile[1024];
			sscanf_s(szTemp, "%*s %s", szTexFile, 1024);
			sFullPath = string(szFolder);
			sFullPath += (string("/") + string(szTexFile));

			LPDIRECT3DTEXTURE9 pTexture = gTextureManager->GetTexture(sFullPath);
			mMapMtltex[sMtlName]->SetTexture(pTexture);
		}
	}
	fclose(fp);
}
