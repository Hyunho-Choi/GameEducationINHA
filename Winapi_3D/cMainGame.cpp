#include "stdafx.h"
#include "cMainGame.h"
#include "cMatrix.h"
#include "cVector3.h"


cMainGame::cMainGame()
	: m_hBitmap(NULL)
	, m_vEye(0,0,-5)
	, m_vLookAt(0,0,0)
	, m_vUp(0,1,0)
	, m_vVertexLook(0,0,1)
	, m_defaultDistance(0.0f, 5.0f, -5.0f)
{
	
}

cMainGame::~cMainGame()
{
	SelectObject(m_MemDC, m_hOldBitmap);
	DeleteObject(m_hBitmap);
	DeleteDC(m_MemDC);
}

void cMainGame::SetGrid()
{
	for (float i = -5.f; i < 6.f; i += 1.f)
	{
		m_vecGrid.push_back(cVector3(i, 0, -5));
		m_vecGrid.push_back(cVector3(i, 0, 5));
		m_vecGrid.push_back(cVector3(-5, 0, i));
		m_vecGrid.push_back(cVector3(5, 0, i));
	}
}

void cMainGame::RenderCube()
{
	cMatrix matWVP = m_matWorld * m_matView * m_matProj;

	for (size_t i = 0; i < m_vecIndex.size(); i += 3)
	{
		cVector3 v1 = m_vecVertex[m_vecIndex[i + 0]];
		cVector3 v2 = m_vecVertex[m_vecIndex[i + 1]];
		cVector3 v3 = m_vecVertex[m_vecIndex[i + 2]];
		
		v1 = cVector3::TransformCoord(v1, m_matVertexR);
		v2 = cVector3::TransformCoord(v2, m_matVertexR);
		v3 = cVector3::TransformCoord(v3, m_matVertexR);

		v1 = cVector3::TransformCoord(v1, m_matVertexT);
		v2 = cVector3::TransformCoord(v2, m_matVertexT);
		v3 = cVector3::TransformCoord(v3, m_matVertexT);
		
		
		if (cVector3::Dot(cVector3::Cross(v2 - v1, v2 - v3), v1 - m_vEye) < 0.f)
		{
			continue;
		}
		
		v1 = cVector3::TransformCoord(v1, matWVP);
		v2 = cVector3::TransformCoord(v2, matWVP);
		v3 = cVector3::TransformCoord(v3, matWVP);

		if (cVector3::Dot(cVector3::Cross(v2 - v1, v3 - v1), cVector3(0,0,1)) > 0.f)
		{
			//continue;
		}
			
		v1 = cVector3::TransformCoord(v1, m_matViewport);
		v2 = cVector3::TransformCoord(v2, m_matViewport);
		v3 = cVector3::TransformCoord(v3, m_matViewport);
		/*
		MoveToEx(m_MemDC, v1.GetX(), v1.GetY(), NULL);
		LineTo(m_MemDC, v2.GetX(), v2.GetY());
		LineTo(m_MemDC, v3.GetX(), v3.GetY());
		LineTo(m_MemDC, v1.GetX(), v1.GetY());
		*/
		POINT temp[3] = { { v1.GetX(), v1.GetY() },{ v2.GetX(), v2.GetY() },{ v3.GetX(), v3.GetY() } };
		Polygon(m_MemDC, temp, 3);
	}
}

void cMainGame::RenderGrid()
{
	cMatrix matWVP = m_matWorld * m_matView * m_matProj;

	for (int i = 0; i < 11; i++)
	{
		cVector3 v1 = m_vecGrid[4 * i + 0];
		cVector3 v2 = m_vecGrid[4 * i + 1];
		cVector3 v3 = m_vecGrid[4 * i + 2];
		cVector3 v4 = m_vecGrid[4 * i + 3];

		v1 = cVector3::TransformCoord(v1, matWVP);
		v2 = cVector3::TransformCoord(v2, matWVP);
		v3 = cVector3::TransformCoord(v3, matWVP);
		v4 = cVector3::TransformCoord(v4, matWVP);

		v1 = cVector3::TransformCoord(v1, m_matViewport);
		v2 = cVector3::TransformCoord(v2, m_matViewport);
		v3 = cVector3::TransformCoord(v3, m_matViewport);
		v4 = cVector3::TransformCoord(v4, m_matViewport);

		MoveToEx(m_MemDC, v1.GetX(), v1.GetY(), NULL);
		LineTo(m_MemDC, v2.GetX(), v2.GetY());
		MoveToEx(m_MemDC, v3.GetX(), v3.GetY(), NULL);
		LineTo(m_MemDC, v4.GetX(), v4.GetY());
	}


	//TextOut(m_MemDC, )
}

void cMainGame::UpdateMove()
{

	if (GetKeyState('W') & 0x8000)
	{
		m_matVertexT = m_matVertexT * cMatrix::Translation(m_vVertexLook / 10.f);
		m_vLookAt = cVector3::TransformCoord(m_vLookAt, cMatrix::Translation(m_vVertexLook / 10.f));
		m_vEye = cVector3::TransformCoord(m_vEye, cMatrix::Translation(m_vVertexLook / 10.f));
	}
	
	if (GetKeyState('S') & 0x8000)
	{
		m_matVertexT = m_matVertexT * cMatrix::Translation(m_vVertexLook / -10.f);
		m_vLookAt = cVector3::TransformCoord(m_vLookAt, cMatrix::Translation(m_vVertexLook / -10.f));
		m_vEye = cVector3::TransformCoord(m_vEye, cMatrix::Translation(m_vVertexLook / -10.f));
	}
	
	if (GetKeyState('A') & 0x8000)
	{
		m_matVertexR = m_matVertexR * cMatrix::RotationY(-2.f);
		m_vVertexLook = cVector3::TransformNormal(m_vVertexLook, cMatrix::RotationY(-2.f));
		m_vVertexLook.Normalize();

		//m_vEye = cVector3::TransformCoord(m_vEye - m_vLookAt, cMatrix::RotationY(-2.f)) + m_vLookAt;
	}
	
	if (GetKeyState('D') & 0x8000)
	{
		m_matVertexR = m_matVertexR * cMatrix::RotationY(2.f);
		m_vVertexLook = cVector3::TransformNormal(m_vVertexLook, cMatrix::RotationY(2.f));
		m_vVertexLook.Normalize();

		//m_vEye = cVector3::TransformCoord(m_vEye - m_vLookAt, cMatrix::RotationY(2.f)) + m_vLookAt;
	}
}

void cMainGame::Setup()
{
	HDC hdc = GetDC(g_hWnd);
	m_MemDC = CreateCompatibleDC(hdc);

	RECT rc;
	GetClientRect(g_hWnd, &rc);
	m_hBitmap = CreateCompatibleBitmap(hdc, rc.right, rc.bottom);
	m_hOldBitmap = (HBITMAP)SelectObject(m_MemDC, m_hBitmap);
	ReleaseDC(g_hWnd, hdc);

	m_vecVertex.push_back(cVector3(-1.0f, -1.0f, -1.0f));
	m_vecVertex.push_back(cVector3(-1.0f, 1.0f, -1.0f));
	m_vecVertex.push_back(cVector3(1.0f, 1.0f, -1.0f));
	m_vecVertex.push_back(cVector3(1.0f, -1.0f, -1.0f));

	m_vecVertex.push_back(cVector3(-1.0f, -1.0f, 1.0f));
	m_vecVertex.push_back(cVector3(-1.0f, 1.0f, 1.0f));
	m_vecVertex.push_back(cVector3(1.0f, 1.0f, 1.0f));
	m_vecVertex.push_back(cVector3(1.0f, -1.0f, 1.0f));

	// 앞
	m_vecIndex.push_back(0);
	m_vecIndex.push_back(1);
	m_vecIndex.push_back(2);

	m_vecIndex.push_back(0);
	m_vecIndex.push_back(2);
	m_vecIndex.push_back(3);
	// 뒤
	m_vecIndex.push_back(4);
	m_vecIndex.push_back(6);
	m_vecIndex.push_back(5);

	m_vecIndex.push_back(4);
	m_vecIndex.push_back(7);
	m_vecIndex.push_back(6);
	// 왼쪾
	m_vecIndex.push_back(4);
	m_vecIndex.push_back(5);
	m_vecIndex.push_back(1);

	m_vecIndex.push_back(4);
	m_vecIndex.push_back(1);
	m_vecIndex.push_back(0);

	// 오른
	m_vecIndex.push_back(3);
	m_vecIndex.push_back(2);
	m_vecIndex.push_back(6);

	m_vecIndex.push_back(3);
	m_vecIndex.push_back(6);
	m_vecIndex.push_back(7);

	// 위
	m_vecIndex.push_back(1);
	m_vecIndex.push_back(5);
	m_vecIndex.push_back(6);

	m_vecIndex.push_back(1);
	m_vecIndex.push_back(6);
	m_vecIndex.push_back(2);
	
	// 아래
	m_vecIndex.push_back(4);
	m_vecIndex.push_back(0);
	m_vecIndex.push_back(3);

	m_vecIndex.push_back(4);
	m_vecIndex.push_back(3);
	m_vecIndex.push_back(7);

	SetGrid();

	m_matVertexR = cMatrix::Identity(4);
	m_matVertexT = cMatrix::Identity(4);
	m_matWorld = cMatrix::Identity(4);
	m_matView = cMatrix::Identity(4);
	m_matProj = cMatrix::Identity(4);
	m_matViewport = cMatrix::Identity(4);

	m_vLookAt = cVector3(0.0f, 0.0f, 0.0f);
	m_vEye = cVector3(0.0f, 5.0f, -5.0f);

}

void cMainGame::Update()
{
	RECT rc;
	GetClientRect(g_hWnd, &rc);
	
	UpdateMove();
	
	m_matView = cMatrix::View(m_vEye, m_vLookAt, m_vUp);
	m_matProj = cMatrix::Projection(FOV, (float)rc.right /(float)rc.bottom, 1.0f, 1000.f);
	m_matViewport = cMatrix::Viewport(0, 0, rc.right, rc.bottom, 0, 1);
}

void cMainGame::Render(HDC hdc)
{
	RECT rc;
	GetClientRect(g_hWnd, &rc);

	PatBlt(m_MemDC, rc.left, rc.top, rc.right, rc.bottom, WHITENESS);

	RenderGrid();
	RenderCube();
	
	BitBlt(hdc, 0, 0, rc.right, rc.bottom, m_MemDC, 0, 0, SRCCOPY);
}

void cMainGame::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static POINT lastMousePos;
	
	switch (message)
	{
	case WM_LBUTTONDOWN:
		break;
	case WM_LBUTTONUP:
		break;
	case WM_MOUSEMOVE:
		switch (wParam)
		{
		case MK_LBUTTON:
			//float size = (m_vLookAt - m_vEye).Length();
			//m_vEye.SetX(m_vEye.GetX() + (LOWORD(lParam) - lastMousePos.x)*0.04f);
			//m_vEye.SetY(m_vEye.GetY() + (HIWORD(lParam) - lastMousePos.y)*0.04f);
			m_vEye = cVector3::TransformCoord(m_vEye, 
				cMatrix::RotationRight((lastMousePos.y - HIWORD(lParam)) * MOUSE_SENSITIVITY,
				cVector3::Cross(m_vUp, m_vLookAt-m_vEye).Normalize()));
			
			//m_vEye = cVector3::TransformCoord(m_vEye, cMatrix::RotationX((HIWORD(lParam) - lastMousePos.y)*0.04f));
			m_vEye = cVector3::TransformCoord(m_vEye, cMatrix::RotationY((LOWORD(lParam) - lastMousePos.x)*0.06f));
			/*
			m_vEye = cVector3::TransformCoord(m_vEye,
				cMatrix::RotationRight((lastMousePos.x - LOWORD(lParam)) * MOUSE_SENSITIVITY,
					cVector3::Cross(cVector3::Cross(m_vUp, m_vLookAt - m_vEye), m_vLookAt - m_vEye).Normalize()));
			*/
			
			//m_vEye = m_vEye.Normalize(size);
			break;
		}
		lastMousePos = { LOWORD(lParam), HIWORD(lParam) };
		
		break;
	case WM_MOUSEWHEEL:

		if(GET_WHEEL_DELTA_WPARAM(wParam) > 0)
			m_vEye = m_vEye + (m_vEye - m_vLookAt).Normalize();
		else if(m_vEye - m_vLookAt < 3)
		{}
		else
			m_vEye = m_vEye - (m_vEye - m_vLookAt).Normalize();

		break;
		/*
	case WM_KEYDOWN:

		switch (wParam)
		{
		case 'd':
		case 'D':
			m_matVertexR = m_matVertexR * cMatrix::RotationY(2.f);
			m_vVertexLook = cVector3::TransformCoord(m_vVertexLook, cMatrix::RotationY(2.f));
			
			m_vVertexLook.Normalize();
			
			m_vEye = cVector3::TransformCoord(m_vEye - m_vLookAt , cMatrix::RotationY(2.f))+ m_vLookAt;
			//m_vEye = cVector3::TransformCoord(m_vEye, cMatrix::Translation(m_vLookAt));
			break;
		case 'a':
		case 'A':
			m_matVertexR = m_matVertexR * cMatrix::RotationY(-2.f);
			m_vVertexLook = cVector3::TransformNormal(m_vVertexLook, cMatrix::RotationY(-2.f));
			m_vVertexLook.Normalize();
		
			m_vEye = cVector3::TransformCoord(m_vEye - m_vLookAt, cMatrix::RotationY(-2.f)) + m_vLookAt;
			//m_vEye = cVector3::TransformCoord(m_vEye, cMatrix::Translation(m_vLookAt));
			break;
		case 'W':
		case 'w':
			m_matVertexT = m_matVertexT * cMatrix::Translation(m_vVertexLook / 10.f);
			m_vLookAt = cVector3::TransformCoord(m_vLookAt, cMatrix::Translation(m_vVertexLook / 10.f));
			m_vEye = cVector3::TransformCoord(m_vEye, cMatrix::Translation(m_vVertexLook / 10.f));
			break;
		case 's':
		case 'S':
			
			m_matVertexT = m_matVertexT * cMatrix::Translation(m_vVertexLook / -10.f);
			m_vLookAt = cVector3::TransformCoord(m_vLookAt, cMatrix::Translation(m_vVertexLook / -10.f));
			m_vEye = cVector3::TransformCoord(m_vEye, cMatrix::Translation(m_vVertexLook / -10.f));
			break;
		}
	default:
		break;
		*/
		
	}
	//m_vEye = m_vLookAt + m_defaultDistance;

}
