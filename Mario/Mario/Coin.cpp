#include "Coin.h"

CCoin::CCoin()
{
	DebugOut(L"new coin\n");
}

void CCoin::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
}

void CCoin::Render()
{
	animation_set->at(0)->Render(x, y, 255, 1, 0, 0);
}

void CCoin::SetState(int state)
{
}

void CCoin::GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx, int dy)
{
}
