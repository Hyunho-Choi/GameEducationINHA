#pragma once



class cMainGame
{
public:
	cMainGame();
	~cMainGame();

private:
	HDC		m_MemDC;
	HBITMAP	m_hOldBitmap, m_hBitmap;

	
	std::vector<cVector3>	m_vecVertex;
	std::vector<DWORD>		m_vecIndex;
	std::vector<cVector3>	m_vecGrid;  //그리드 정점

	cVector3				m_vVertexLook; //박스 정면방향
	cMatrix					m_matVertexT; //박스위치
	cMatrix					m_matVertexR; //박스로테이션

	cMatrix					m_matEyeT; //카메라위치
	cMatrix					m_matEyeR; //카메라회전각

	cMatrix					m_matWorld;
	cMatrix					m_matView;
	cMatrix					m_matProj;
	cMatrix					m_matViewport;

	cVector3				m_vEye;
	cVector3				m_vLookAt;
	cVector3				m_vUp;

	cVector3				m_defaultDistance;


	void SetGrid();
	void RenderCube();
	void RenderGrid();
	void UpdateMove();


	
public:
	void Setup();
	void Update();
	void Render(HDC hdc);
	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	


	
};

