#pragma once
class UserController
{
private:
	PacketControllerToServer toServer;
	
public:
	//서버행 버퍼 : 위치 이동량 / 방향 / 공격 유무;
	PacketControllerToServer GetBuffer();

	void PushKeyboardInput();
	void PushMouseInput(LPARAM lParam);
	void PushClickInput(LPARAM lParam);

	UserController();
	~UserController();
};

