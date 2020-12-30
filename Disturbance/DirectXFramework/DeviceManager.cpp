#include "stdafx.h"
#include "DeviceManager.h"


DeviceManager::DeviceManager()
	: mD3D(nullptr)
	  , mD3DDevice(nullptr)
{
	mD3D = Direct3DCreate9(D3D_SDK_VERSION);

	D3DCAPS9 stCaps;
	int nVertexProcessing;
	mD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &stCaps);
	if (stCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		nVertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else
	{
		nVertexProcessing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	D3DPRESENT_PARAMETERS stD3DPP;
	ZeroMemory(&stD3DPP, sizeof(D3DPRESENT_PARAMETERS));

	stD3DPP.SwapEffect = D3DSWAPEFFECT_DISCARD;
	stD3DPP.Windowed = TRUE;
	stD3DPP.BackBufferFormat = D3DFMT_UNKNOWN;
	stD3DPP.EnableAutoDepthStencil = TRUE;
	stD3DPP.AutoDepthStencilFormat = D3DFMT_D16;
	mD3D->CreateDevice(D3DADAPTER_DEFAULT,
	                   D3DDEVTYPE_HAL, GetActiveWindow(),
	                   nVertexProcessing | D3DCREATE_MULTITHREADED, &stD3DPP,
	                   &mD3DDevice);
}


LPDIRECT3DDEVICE9 DeviceManager::GetDevice()
{
	return mD3DDevice;
}

void DeviceManager::Destroy()
{
	SAFE_RELEASE(mD3D);
	SAFE_RELEASE(mD3DDevice);
}
