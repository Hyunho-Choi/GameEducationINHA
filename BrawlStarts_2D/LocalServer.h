#pragma once
class LocalServer
{
	// 모든 캐릭터 위치 저장
	// 콘트롤러 입력 처리
	// 공격판정 처리
	// AI (플레이어보다 공격처리와 가까움 -> 공격딜레이 필요)
	// 60hz로 LocalClient 에게 전송(어떻게?)
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

