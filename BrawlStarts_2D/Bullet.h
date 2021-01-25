#pragma once
class Bullet
{
private:
	bool isFire = false;
	int mDrawCnt = 1;
	int mFireDelay = 5;
	POINT mPos;
	int GameY;
	int mAvatar;
	int mBulletSpeed;
	float mDirection;
	//ServerMap bulletMap;

public:
	Bullet();
	virtual ~Bullet();
	void Draw(HDC hdc);
	void DrawGame(HDC hdc);
	bool isFired();
	POINT GetPos();

	void Reset();
	void Fire(POINT pos, int avatar, int bulletSpeed, float direction, int drawCnt, int delay);
	void Update(POINT& user);

};

