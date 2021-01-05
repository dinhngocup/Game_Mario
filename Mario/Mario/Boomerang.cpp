#include "Boomerang.h"

CBoomerang::CBoomerang(float start_x, float start_y)
{
	generate_id++;
	this->id = generate_id;
	SetStartPosition(start_x, start_y);
	vx = 0.3f;
	//vy = -0.06f;
	vy = -0.1f;
}

void CBoomerang::GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx, int dy)
{
	left = x;
	top = y;
	right = left + BOOMERANG_BB_WIDTH;
	bottom = top + BOOMERANG_BB_HEIGHT;
}

void CBoomerang::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (start_count == 0)
		start_count = GetTickCount64();
	if (GetTickCount64() - start_count >= 1200) {
		if (vx > 0)
			vx *= -1;
	}
	else if (GetTickCount64() - start_count >= 1000) {
		if (vy < 0)
			vy += 0.2f;
	}
	if (x < start_x) {

		SetHealth(false);
		return;

	}
	CGameObject::Update(dt);
	x += dx;
	y += dy;
}

void CBoomerang::Render()
{
	animation_set->at(0)->Render(x, y);
	//RenderBoundingBox();
}

void CBoomerang::SetState(int state)
{
}

void CBoomerang::IsCollisionWithMario(LPCOLLISIONEVENT e)
{
}

void CBoomerang::HandleCollisionWithMario(LPCOLLISIONEVENT e)
{
}
