#pragma once
class UserController
{
private:
	PacketControllerToServer toServer;
	
public:
	//������ ���� : ��ġ �̵��� / ���� / ���� ����;
	PacketControllerToServer GetBuffer();

	void PushKeyboardInput();
	void PushMouseInput(LPARAM lParam);
	void PushClickInput(LPARAM lParam);

	UserController();
	~UserController();
};

