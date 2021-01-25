#include "stdafx.h"
//#include "UserController.h"


PacketControllerToServer UserController::GetBuffer()
{
	PacketControllerToServer temp = toServer;
	toServer.isNew = false;
	toServer.isClick = false;
	toServer.Move = { 0,0 };
	return temp;
}

void UserController::PushKeyboardInput()
{
	if (GetKeyState(0x57) & 0x8000)
	{
		toServer.ConIntDir = UP;
		if (GetKeyState(0x44) & 0x8000)
		{
			toServer.isNew = true;
			toServer.Move = { 1,-1 };
		}
		else if (GetKeyState(0x41) & 0x8000)
		{
			toServer.isNew = true;
			toServer.Move = { -1,-1 };
		}
		else
		{
			toServer.isNew = true;
			toServer.Move = { 0,-1 };
		}
	}
	else if (GetKeyState(0x44) & 0x8000)
	{
		toServer.ConIntDir = RIGHT;
		if (GetKeyState(0x53) & 0x8000)
		{
			toServer.isNew = true;
			toServer.Move = { 1,1 };
		}
		else
		{
			toServer.isNew = true;
			toServer.Move = { 1,0 };
		}
	}
	else if (GetKeyState(0x53) & 0x8000)
	{
		toServer.ConIntDir = DOWN;
		if (GetKeyState(0x41) & 0x8000)
		{
			toServer.isNew = true;
			toServer.Move = { -1,1 };
		}
		else
		{
			toServer.isNew = true;
			toServer.Move = { 0,1 };
		}
	}
	else if (GetKeyState(0x41) & 0x8000)
	{
		toServer.ConIntDir = LEFT;
		if (GetKeyState(0x57) & 0x8000)
		{
			toServer.isNew = true;
			toServer.Move = { -1, -1 };
		}
		else
		{
			toServer.isNew = true;
			toServer.Move = { -1,0 };
		}
	}
}

void UserController::PushMouseInput(LPARAM lParam)
{
	// 마우스 움직이는 메세지에서 출력
	// lparam값 그대로 서버로
	//toServer.isNew = true;
	toServer.mouseLparam = lParam;
}

void UserController::PushClickInput(LPARAM lParam)
{
	toServer.isNew = true;
	toServer.mouseLparam = lParam;
	toServer.isClick = true;
}

UserController::UserController()
{

}


UserController::~UserController()
{
}
