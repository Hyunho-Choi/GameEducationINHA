#pragma once

#include "Singleton.h"

#define gD3Device DeviceManager::GetInstance()->GetDevice()

class DeviceManager : public Singleton<DeviceManager>
{
private :
	friend Singleton;
	DeviceManager();
	LPDIRECT3D9 mD3D;
	LPDIRECT3DDEVICE9 mD3DDevice;

public :
	LPDIRECT3DDEVICE9 GetDevice();
	void Destroy();
};
