#pragma once
class LocalServer
{
	// ��� ĳ���� ��ġ ����
	// ��Ʈ�ѷ� �Է� ó��
	// �������� ó��
	// AI (�÷��̾�� ����ó���� ����� -> ���ݵ����� �ʿ�)
	// 60hz�� LocalClient ���� ����(���?)
private:
	PacketServerToClient User[2];
	ServerMap Map;
	//CollisionChecker ColChecker;

public:
	void ReceiveControllerBuffer(PacketControllerToServer ConInput);
	void SendUserDateToClient(int userNum, PacketServerToClient* pUser);
	void CheckCollision(POINT rMove);
	void UpdateBullet();
	LocalServer();
	~LocalServer();
};

