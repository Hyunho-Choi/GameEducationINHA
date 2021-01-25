#include "stdafx.h"
//#include "LocalServer.h"


void LocalServer::ReceiveControllerBuffer(PacketControllerToServer ConInput)
{
	if (ConInput.isNew)
	{
		// 마우스입력 = 맵기준의 pos로 변환 필요

		// 키보드 입력
		User[0].IntDir = ConInput.ConIntDir;

		User[0].pos.x += ConInput.Move.x * MOVE_ONE_TICK;
		User[0].pos.y += ConInput.Move.y * MOVE_ONE_TICK;
		User[0].state = MOVE;

		CheckCollision(ConInput.Move);

		// 클릭 입력
		if (ConInput.isClick)
		{
			User[0].state = FIRE;

			//{ XMARGIN + User[0].pos.x* GAME_MULTIFY, WIN_SIZEY * 0.5 - ((MAPTILE_SIZE * 0.5)* GAME_MULTIFY) }

			float temp = atan(
				(HIWORD(ConInput.mouseLparam) - (WIN_SIZEY * 0.5)) /
				(LOWORD(ConInput.mouseLparam) - (XMARGIN + User[0].pos.x* GAME_MULTIFY))
			);
	
			if ((LOWORD(ConInput.mouseLparam) - (XMARGIN + User[0].pos.x* GAME_MULTIFY)) < 0)
				temp += PI;


			if (User[0].bulletCnt >= 3)
			{
				for (int i = 0; i < (BULLET_CNT /3); i++)
				{
					User[0].myBullet[i].Fire(User[0].pos, 0, 5, temp , 10, i * 5);
				}
				User[0].bulletCnt -= 1;
			}
			else if (User[0].bulletCnt >= 2)
			{
				for (int i = 0; i < (BULLET_CNT / 3); i++)
				{
					User[0].myBullet[i+ (BULLET_CNT / 3)].Fire(User[0].pos, 0, 5, temp, 10, i * 5);
				}
				User[0].bulletCnt -= 1;
			}
			else if (User[0].bulletCnt >= 1)
			{
				for (int i = 0; i < (BULLET_CNT / 3); i++)
				{
					User[0].myBullet[i+ (BULLET_CNT / 3 * 2)].Fire(User[0].pos, 0, 5, temp, 10, i * 5);
				}
				User[0].bulletCnt -= 1;
			}
			
			std::cout << "click" << std::endl;
		}
	}
	else
	{
		// 마우스입력
		User[0].state = IDLE;
	}
}

void LocalServer::SendUserDateToClient(int userNum, PacketServerToClient * pUser)
{
	memcpy(pUser, &User[userNum], sizeof(PacketServerToClient));
}

void LocalServer::CheckCollision(POINT rMove)
{

	RECT Col;
	const RECT CharRt = { User[0].pos.x - (MAPTILE_SIZE * 0.3), User[0].pos.y - (MAPTILE_SIZE * 0.3),
		User[0].pos.x + (MAPTILE_SIZE * 0.3), User[0].pos.y + (MAPTILE_SIZE * 0.3) };

	// 자신의 pos로 자신이 존재하는 섹션을 파악
	// 자신의 2줄 ? 1줄 ?  바깥을 체크
	// 충돌가능한 타일일 경우 진입금지 = IntersectRect
	// 충돌시 반대로 움직임

	//std::cout << User[0].pos.x / MAPTILE_SIZE << " " << User[0].pos.y / MAPTILE_SIZE << std::endl;
	
	// 좌
	if (Map.GetMapState(User[0].pos.x / MAPTILE_SIZE - 1, User[0].pos.y / MAPTILE_SIZE) >= 350)
	{
		if (IntersectRect(&Col, Map.GetMapRt(User[0].pos.x / MAPTILE_SIZE - 1, User[0].pos.y / MAPTILE_SIZE), &CharRt))
		{
			User[0].pos.x -= rMove.x * MOVE_ONE_TICK;
			User[0].pos.y -= rMove.y * MOVE_ONE_TICK;
			return;
		}
	}
	// 우
	if (Map.GetMapState(User[0].pos.x / MAPTILE_SIZE + 1, User[0].pos.y / MAPTILE_SIZE) >= 350)
	{
		if (IntersectRect(&Col, Map.GetMapRt(User[0].pos.x / MAPTILE_SIZE + 1, User[0].pos.y / MAPTILE_SIZE), &CharRt))
		{
			User[0].pos.x -= rMove.x * MOVE_ONE_TICK;
			User[0].pos.y -= rMove.y * MOVE_ONE_TICK;
			return;
		}
	}
	// 하
	if (Map.GetMapState(User[0].pos.x / MAPTILE_SIZE, User[0].pos.y / MAPTILE_SIZE +1 ) >= 350)
	{
		if (IntersectRect(&Col, Map.GetMapRt(User[0].pos.x / MAPTILE_SIZE, User[0].pos.y / MAPTILE_SIZE + 1), &CharRt))
		{
			User[0].pos.x -= rMove.x * MOVE_ONE_TICK;
			User[0].pos.y -= rMove.y * MOVE_ONE_TICK;
			return;
		}
	}
	// 상
	else if (Map.GetMapState(User[0].pos.x / MAPTILE_SIZE, User[0].pos.y / MAPTILE_SIZE - 1) >= 350)
	{
		if (IntersectRect(&Col, Map.GetMapRt(User[0].pos.x / MAPTILE_SIZE, User[0].pos.y / MAPTILE_SIZE - 1), &CharRt))
		{
			User[0].pos.x -= rMove.x * MOVE_ONE_TICK;
			User[0].pos.y -= rMove.y * MOVE_ONE_TICK;
			return;
		}
	}

	// 좌상
	if (Map.GetMapState(User[0].pos.x / MAPTILE_SIZE - 1, User[0].pos.y / MAPTILE_SIZE - 1) >= 350)
	{
		if (IntersectRect(&Col, Map.GetMapRt(User[0].pos.x / MAPTILE_SIZE - 1, User[0].pos.y / MAPTILE_SIZE - 1), &CharRt))
		{
			User[0].pos.x -= rMove.x * MOVE_ONE_TICK;
			User[0].pos.y -= rMove.y * MOVE_ONE_TICK;
			return;
		}
	}
	// 우상
	if (Map.GetMapState(User[0].pos.x / MAPTILE_SIZE + 1, User[0].pos.y / MAPTILE_SIZE - 1) >= 350)
	{
		if (IntersectRect(&Col, Map.GetMapRt(User[0].pos.x / MAPTILE_SIZE + 1, User[0].pos.y / MAPTILE_SIZE - 1), &CharRt))
		{
			User[0].pos.x -= rMove.x * MOVE_ONE_TICK;
			User[0].pos.y -= rMove.y * MOVE_ONE_TICK;
			return;
		}
	}
	// 좌하
	if (Map.GetMapState(User[0].pos.x / MAPTILE_SIZE - 1, User[0].pos.y / MAPTILE_SIZE + 1) >= 350)
	{
		if (IntersectRect(&Col, Map.GetMapRt(User[0].pos.x / MAPTILE_SIZE - 1, User[0].pos.y / MAPTILE_SIZE + 1), &CharRt))
		{
			User[0].pos.x -= rMove.x * MOVE_ONE_TICK;
			User[0].pos.y -= rMove.y * MOVE_ONE_TICK;
			return;
		}
	}
	// 우하
	if (Map.GetMapState(User[0].pos.x / MAPTILE_SIZE + 1, User[0].pos.y / MAPTILE_SIZE + 1) >= 350)
	{
		if (IntersectRect(&Col, Map.GetMapRt(User[0].pos.x / MAPTILE_SIZE + 1, User[0].pos.y / MAPTILE_SIZE + 1), &CharRt))
		{
			User[0].pos.x -= rMove.x * MOVE_ONE_TICK;
			User[0].pos.y -= rMove.y * MOVE_ONE_TICK;
			return;
		}
	}



	//불렛 삭제
	
}

void LocalServer::UpdateBullet()
{
	for (int i = 0; i < BULLET_CNT; i++)
	{
		User[0].myBullet[i].Update(User[0].pos);
	}
	if (User[0].bulletCnt < 3)
	{
		User[0].bulletCnt += 0.02;
	}
	//std::cout << User[0].bulletCnt << std::endl;

	for (int i = 0; i < BULLET_CNT; i++)
	{
		if (User[0].myBullet[i].isFired())
		{
			//std::cout << Map.GetMapState(User[0].myBullet[i].GetPos().x / MAPTILE_SIZE, User[0].myBullet[i].GetPos().y / MAPTILE_SIZE) << std::endl;

			if (Map.GetMapState(User[0].myBullet[i].GetPos().x / MAPTILE_SIZE, User[0].myBullet[i].GetPos().y / MAPTILE_SIZE) >= 350)
			{
				User[0].myBullet[i].Reset();
			}
		}

	}
}


LocalServer::LocalServer()
{
	User[0].pos = { MAPTILE_SIZE * 12, MAPTILE_SIZE * 38 };
	User[1].pos = { WIN_SIZEX / 2 + 100 , WIN_SIZEY / 2 };
}


LocalServer::~LocalServer()
{
}
