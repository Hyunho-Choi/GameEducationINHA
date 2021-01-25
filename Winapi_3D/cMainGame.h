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
	std::vector<cVector3>	m_vecGrid;  //�׸��� ����

	cVector3				m_vVertexLook; //�ڽ� �������
	cMatrix					m_matVertexT; //�ڽ���ġ
	cMatrix					m_matVertexR; //�ڽ������̼�

	cMatrix					m_matEyeT; //ī�޶���ġ
	cMatrix					m_matEyeR; //ī�޶�ȸ����

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

