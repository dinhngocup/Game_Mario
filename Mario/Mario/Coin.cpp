#include "Coin.h"

CCoin::CCoin(float x, float y)
{
	this->x = x;
	this->y = y;
	vy = -0.7f;
	start_y = y;
}

void CCoin::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	CGame* game = CGame::GetInstance();

	vy += COIN_GRAVITY * dt;
	CGameObject::Update(dt, colliable_objects);
	if (y + dy >= start_y - 80 && vy >0) {
		CMario* mario = CMario::GetInstance();
		mario->AddScore(COIN_SCORE);
		mario->AddCoin();
		SetHealth(false);
		ableToCheckCollision = false;
		return;
	}
	y += dy;

}

void CCoin::Render()
{
	animation_set->at(0)->Render(x, y, 255, 1, 0, 0);
}

void CCoin::SetState(int state)
{
}

void CCoin::IsCollisionWithMario(LPCOLLISIONEVENT e)
{
	
}

void CCoin::GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx, int dy)
{
}
