#include "stdafx.h"
#include "QuarterMap.h"
#include "cObjLoader.h"
#include "cMtlTex.h"

QuarterMap::QuarterMap()
	: mMeshWall(NULL)
	, mMeshFloor(NULL)
	, mMeshDoor(NULL)
	, mTextrue(NULL)
	, mTextrueFloor(NULL)
	, mShader(NULL)
{
}


QuarterMap::~QuarterMap()
{
	Destroy();
	
}

void QuarterMap::Setup(char* szFolder, char* szRaw, char* szTex, DWORD dwBytesPerPixel)
{
	//todo 추후 파일 입출력으로 변경
	float textureMultify = 5;
	
	string sFolder(szFolder);
	string sRaw = sFolder + string(szRaw);
	string sTex = sFolder + string(szTex);

	mTextrue = gTextureManager->GetTexture(sTex);
	mTextrueFloor = gTextureManager->GetTexture("Texture/stones.png");

	FILE* fp = NULL;
	fopen_s(&fp, sRaw.c_str(), "rb");
	
	fseek(fp, 0, SEEK_END);
	int nFileSize = ftell(fp);
	int nNumVertex = nFileSize / dwBytesPerPixel;
	int nTile = pow(nNumVertex, 0.5);
	float nWallMargin = 1.f;
	//int nNumVertex = (mapHeight) * (mapWidth);

	
	fseek(fp, 0, SEEK_SET);
	vector<Vertex> vecVertex(nNumVertex);
	mVertexContainer.resize(nNumVertex);

	vector<DWORD> vecIndex;
	vecIndex.reserve((nTile - 1) * (nTile - 1) * 2 * 3);

	for (int i = 0; i < nTile; i++)
	{
		for (int j = 0; j < nTile; j++)
		{
			int tempY;
			tempY = fgetc(fp);
			if ((tempY > 250.f) || (tempY == EOF))
				tempY = 252.f;
			else
				tempY = 0.f;

			Vertex v;
			v.Pos = D3DXVECTOR3(j - nWallMargin, tempY, i - nWallMargin);
			v.Normal = D3DXVECTOR3(0, 1, 0);
			v.TexUV = D3DXVECTOR2(j / (float)nTile * textureMultify, i / (float)nTile * textureMultify);

			vecVertex[((i * (nTile)) + j)] = v;
			mVertexContainer[((i * (nTile)) + j)] = v.Pos;

			if (dwBytesPerPixel == 3)
			{
				fgetc(fp), fgetc(fp);
			}
		}
	}
	fclose(fp);

	for (int x = 0; x < nTile - 1; ++x)
	{
		for (int z = 0; z < nTile - 1; ++z)
		{
			int _0 = (x + 0)* (nTile)+z + 0;
			int _1 = (x + 1)* (nTile)+z + 0;
			int _2 = (x + 1)* (nTile)+z + 1;
			int _3 = (x + 0)* (nTile)+z + 1;

			vecIndex.push_back(_0);
			vecIndex.push_back(_1);
			vecIndex.push_back(_3);

			vecIndex.push_back(_2);
			vecIndex.push_back(_3);
			vecIndex.push_back(_1);
		}
	}

	D3DXCreateMeshFVF(vecIndex.size() / 3, vecVertex.size(),
		D3DXMESH_MANAGED | D3DXMESH_32BIT,
		Vertex::FVF, gD3Device, &mMeshMap);

	Vertex * pV = NULL;
	mMeshMap->LockVertexBuffer(0, (LPVOID*)&pV);
	memcpy(pV, &vecVertex[0], vecVertex.size() * sizeof(Vertex));
	mMeshMap->UnlockVertexBuffer();

	DWORD* pI = NULL;
	mMeshMap->LockIndexBuffer(0, (LPVOID*)&pI);
	memcpy(pI, &vecIndex[0], vecIndex.size() * sizeof(DWORD));
	mMeshMap->UnlockIndexBuffer();

	DWORD* pA = NULL;
	mMeshMap->LockAttributeBuffer(0, &pA);
	ZeroMemory(pA, (vecIndex.size() / 3) * sizeof(DWORD));
	mMeshMap->UnlockAttributeBuffer();

	vector<DWORD> vecAdj(vecIndex.size());
	mMeshMap->GenerateAdjacency(0.0f, &vecAdj[0]);
	mMeshMap->OptimizeInplace(
		D3DXMESHOPT_ATTRSORT |
		D3DXMESHOPT_COMPACT |
		D3DXMESHOPT_VERTEXCACHE,
		&vecAdj[0],
		0, 0, 0);

	ZeroMemory(&mMaterial, sizeof(D3DMATERIAL9));
	mMaterial.Ambient = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.f);
	mMaterial.Diffuse = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.f);
	mMaterial.Specular = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.f);

	if (mMeshMap)
	{
		mMeshMap->Release();
		mMeshMap = NULL;
	}

	cObjLoader loader;
	mMeshWall = loader.LoadMesh(vecMtlTex, "obj", "SampleRoom.obj");
}

void QuarterMap::Update()
{
}

void QuarterMap::Render()
{
	/*
	gD3Device->SetRenderState(D3DRS_LIGHTING, true);
	gD3Device->SetRenderState(D3DRS_AMBIENT, 0xffffffff);
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);
	gD3Device->SetTransform(D3DTS_WORLD, &matWorld);
	gD3Device->SetMaterial(&mMaterial);
	gD3Device->SetTexture(0, mTextrue);
	mMeshWall->DrawSubset(0);
	*/
}

void QuarterMap::Destroy()
{
	if (mMeshWall)
	{
		mMeshWall->Release();
		mMeshWall = NULL;
	}
	

	mVertexContainer.clear();

	if (mTextrue)
	{
		mTextrue->Release();
		mTextrue = NULL;
	}

	if (mShader)
	{
		mShader->Release();
		mTextrue = NULL;
	}
}

float QuarterMap::GetHeight(float posX, float posZ)
{
	int mapHeight = 19;
	int mapWidth = 19;

	int nX = posX;
	int nZ = posZ;

	float fDeltaX = posX - nX;
	float fDeltaZ = posZ - nZ;

	int _0 = (nZ + 0) * mapWidth + nX + 0;
	int _1 = (nZ + 1) * mapWidth + nX + 0;
	int _2 = (nZ + 1) * mapWidth + nX + 1;
	int _3 = (nZ + 0) * mapWidth + nX + 1;


	if (fDeltaX + fDeltaZ < 1.f)
	{
		D3DXVECTOR3 v01 = mVertexContainer[_1] - mVertexContainer[_0];
		D3DXVECTOR3 v03 = mVertexContainer[_3] - mVertexContainer[_0];

		if ((mVertexContainer[_1].y + mVertexContainer[_3].y + +mVertexContainer[_0].y) *0.333333f > 20.f)
			int temp = 1;

		return (mVertexContainer[_1].y + mVertexContainer[_3].y + +mVertexContainer[_0].y) *0.333333f;
	}
	else
	{
		fDeltaX = 1.f - fDeltaX;
		fDeltaZ = 1.f - fDeltaZ;
		D3DXVECTOR3 v21 = mVertexContainer[_1] - mVertexContainer[_2];
		D3DXVECTOR3 v23 = mVertexContainer[_3] - mVertexContainer[_2];

		if ((mVertexContainer[_1].y + mVertexContainer[_3].y + +mVertexContainer[_2].y) *0.333333f > 20.f)
			int temp = 1;

		return (mVertexContainer[_1].y + mVertexContainer[_3].y + +mVertexContainer[_2].y) *0.333333f;
	}
}
