#include "StaticCoin.h"

CStaticCoin::CStaticCoin(float x, float y)
{
	DebugOut(L"new\n");
	this->x = x;
	this->y = y;
}

void CStaticCoin::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
}

void CStaticCoin::Render()
{
	animation_set->at(0)->Render(x, y, 255, 1, 0, 0);
}

void CStaticCoin::IsCollisionWithMario(LPCOLLISIONEVENT e)
{
	// e is coin
	CMario* mario = CMario::GetInstance();
	mario->AddScore(STATIC_COIN_SCORE);
	mario->AddCoin();
	SetHealth(false);
	ableToCheckCollision = false;
}

void CStaticCoin::GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx, int dy)
{
	left = x;
	top = y;
	right = left + w;
	bottom = top + h;
}
