#include "stdafx.h"
//#include "LocalServer.h"


void LocalServer::ReceiveControllerBuffer(PacketControllerToServer ConInput)
{
	if (ConInput.isNew)
	{
		// ���콺�Է� = �ʱ����� pos�� ��ȯ �ʿ�

		// Ű���� �Է�
		User[0].IntDir = ConInput.ConIntDir;

		User[0].pos.x += ConInput.Move.x * MOVE_ONE_TICK;
		User[0].pos.y += ConInput.Move.y * MOVE_ONE_TICK;
		User[0].state = MOVE;

		CheckCollision(ConInput.Move);

		// Ŭ�� �Է�
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
		// ���콺�Է�
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

	// �ڽ��� pos�� �ڽ��� �����ϴ� ������ �ľ�
	// �ڽ��� 2�� ? 1�� ?  �ٱ��� üũ
	// �浹������ Ÿ���� ��� ���Ա��� = IntersectRect
	// �浹�� �ݴ�� ������

	//std::cout << User[0].pos.x / MAPTILE_SIZE << " " << User[0].pos.y / MAPTILE_SIZE << std::endl;
	
	// ��
	if (Map.GetMapState(User[0].pos.x / MAPTILE_SIZE - 1, User[0].pos.y / MAPTILE_SIZE) >= 350)
	{
		if (IntersectRect(&Col, Map.GetMapRt(User[0].pos.x / MAPTILE_SIZE - 1, User[0].pos.y / MAPTILE_SIZE), &CharRt))
		{
			User[0].pos.x -= rMove.x * MOVE_ONE_TICK;
			User[0].pos.y -= rMove.y * MOVE_ONE_TICK;
			return;
		}
	}
	// ��
	if (Map.GetMapState(User[0].pos.x / MAPTILE_SIZE + 1, User[0].pos.y / MAPTILE_SIZE) >= 350)
	{
		if (IntersectRect(&Col, Map.GetMapRt(User[0].pos.x / MAPTILE_SIZE + 1, User[0].pos.y / MAPTILE_SIZE), &CharRt))
		{
			User[0].pos.x -= rMove.x * MOVE_ONE_TICK;
			User[0].pos.y -= rMove.y * MOVE_ONE_TICK;
			return;
		}
	}
	// ��
	if (Map.GetMapState(User[0].pos.x / MAPTILE_SIZE, User[0].pos.y / MAPTILE_SIZE +1 ) >= 350)
	{
		if (IntersectRect(&Col, Map.GetMapRt(User[0].pos.x / MAPTILE_SIZE, User[0].pos.y / MAPTILE_SIZE + 1), &CharRt))
		{
			User[0].pos.x -= rMove.x * MOVE_ONE_TICK;
			User[0].pos.y -= rMove.y * MOVE_ONE_TICK;
			return;
		}
	}
	// ��
	else if (Map.GetMapState(User[0].pos.x / MAPTILE_SIZE, User[0].pos.y / MAPTILE_SIZE - 1) >= 350)
	{
		if (IntersectRect(&Col, Map.GetMapRt(User[0].pos.x / MAPTILE_SIZE, User[0].pos.y / MAPTILE_SIZE - 1), &CharRt))
		{
			User[0].pos.x -= rMove.x * MOVE_ONE_TICK;
			User[0].pos.y -= rMove.y * MOVE_ONE_TICK;
			return;
		}
	}

	// �»�
	if (Map.GetMapState(User[0].pos.x / MAPTILE_SIZE - 1, User[0].pos.y / MAPTILE_SIZE - 1) >= 350)
	{
		if (IntersectRect(&Col, Map.GetMapRt(User[0].pos.x / MAPTILE_SIZE - 1, User[0].pos.y / MAPTILE_SIZE - 1), &CharRt))
		{
			User[0].pos.x -= rMove.x * MOVE_ONE_TICK;
			User[0].pos.y -= rMove.y * MOVE_ONE_TICK;
			return;
		}
	}
	// ���
	if (Map.GetMapState(User[0].pos.x / MAPTILE_SIZE + 1, User[0].pos.y / MAPTILE_SIZE - 1) >= 350)
	{
		if (IntersectRect(&Col, Map.GetMapRt(User[0].pos.x / MAPTILE_SIZE + 1, User[0].pos.y / MAPTILE_SIZE - 1), &CharRt))
		{
			User[0].pos.x -= rMove.x * MOVE_ONE_TICK;
			User[0].pos.y -= rMove.y * MOVE_ONE_TICK;
			return;
		}
	}
	// ����
	if (Map.GetMapState(User[0].pos.x / MAPTILE_SIZE - 1, User[0].pos.y / MAPTILE_SIZE + 1) >= 350)
	{
		if (IntersectRect(&Col, Map.GetMapRt(User[0].pos.x / MAPTILE_SIZE - 1, User[0].pos.y / MAPTILE_SIZE + 1), &CharRt))
		{
			User[0].pos.x -= rMove.x * MOVE_ONE_TICK;
			User[0].pos.y -= rMove.y * MOVE_ONE_TICK;
			return;
		}
	}
	// ����
	if (Map.GetMapState(User[0].pos.x / MAPTILE_SIZE + 1, User[0].pos.y / MAPTILE_SIZE + 1) >= 350)
	{
		if (IntersectRect(&Col, Map.GetMapRt(User[0].pos.x / MAPTILE_SIZE + 1, User[0].pos.y / MAPTILE_SIZE + 1), &CharRt))
		{
			User[0].pos.x -= rMove.x * MOVE_ONE_TICK;
			User[0].pos.y -= rMove.y * MOVE_ONE_TICK;
			return;
		}
	}



	//�ҷ� ����
	
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
