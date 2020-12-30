#pragma once
class cMtlTex;

class QuarterMap : public BaseObject
{
	// 기본개념 : Scene에서 생성 파괴하면서 사용
	//			 데이터는 파일입출력
	// raw의 깊이 최소값은 접근할 수 없는 높이 == 맵범위 밖
	// 충돌처리로 가지 못하는 곳과 맵의 밖이라 못가는 곳 구별

private:
	LPD3DXMESH			mMeshMap;
	LPD3DXMESH			mMeshWall;
	LPD3DXMESH			mMeshFloor;
	LPD3DXMESH			mMeshDoor;
	vector<D3DXVECTOR3>	mVertexContainer;
	D3DMATERIAL9		mMaterial;
	LPDIRECT3DTEXTURE9	mTextrue;
	LPDIRECT3DTEXTURE9	mTextrueFloor;
	vector<cMtlTex*>    vecMtlTex;
	LPD3DXEFFECT		mShader;

public:
	QuarterMap();
	virtual ~QuarterMap();

	void Setup(char* szFolder, char* szRaw , char* szTex, DWORD dwBytesPerPixel = 1);
	void Update() override; 
	void Render() override;
	
	void Destroy();
	float GetHeight(float posX, float posZ);
	
	//todo 보간을 통한 높이 인식
};

