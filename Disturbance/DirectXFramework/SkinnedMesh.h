#pragma once

class ST_BONE;

class SkinnedMesh
{
public:
	SkinnedMesh();
	~SkinnedMesh();
	SkinnedMesh(char * szFolder, char*szFilename);
	void Load(char* szFolder, char* szFilename);
	void Destroy();
	void UpdateAndRender();
	void Update(ST_BONE* pCurrent, D3DXMATRIXA16* pMatWorld);
	void SetRandomTrackPosition();
	void SetTransform(D3DXMATRIXA16* pMat);
	D3DXMATRIXA16 m_matWorldTM;
	Synthesize(D3DXVECTOR3, m_vMin, Min);
	Synthesize(D3DXVECTOR3, m_vMax, Max);
	LPD3DXFRAME m_pRoot;

protected:
	float		m_fBlendTime;
	float		m_fPassedBlendTime;
	UINT		m_animationStartTime;
	bool		m_isAnimBlend;
public :
	LPD3DXANIMATIONCONTROLLER m_pAnimController;
	void Setup(char * szFolder, char * szFile);
	void Update();
	void Update(LPD3DXFRAME pFrame, LPD3DXFRAME pParent);
	void Render(LPD3DXFRAME pFrame);
	void SetupBoneMatrixPtrs(LPD3DXFRAME pFrame);
	void UpdateSkinnedMesh(LPD3DXFRAME pFrame);
	void SetAnimationIndex(int nIndex);
	void SetAnimationIndexBlend(int nIndex);
	void SetCurrentAnimationSpeed(float time);
	void PrintAnimationList();

	float GetBlendTime()
	{
		return m_fBlendTime;
	}

};

