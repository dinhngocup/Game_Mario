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
	CMario* mario = CMario::GetInstance();
	if (start_count == 0)
		start_count = GetTickCount64();
	if (GetTickCount64() - start_count >= 1200) {
		if (vx > 0)
			vx *= -1;
	}
	else if (GetTickCount64() - start_count >= 900) {
		if (vy < 0)
			vy += 0.2f;
	}
	if (x < start_x) {

		SetHealth(false);
		return;

	}
	CGameObject::Update(dt);
	if (mario->untouchable == 0) {
		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;

		vector<LPGAMEOBJECT> obj;
		obj.clear();
		obj.push_back(mario);

		coEvents.clear();


		CalcPotentialCollisions(&obj, coEvents);


		if (coEvents.size() == 0)
		{
			x += dx;
			y += dy;
		}
		else
		{
			float min_tx, min_ty, nx = 0, ny;
			float rdx = 0;
			float rdy = 0;
			FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
			//DebugOut(L"cham weapon boomerang\n");

			HandleCollisionWithMario(coEventsResult[0]);
			x += dx;
			y += dy;
			// clean up collision events
			for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
		}
	}
	else {
		x += dx;
		y += dy;
	}
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
	CMario* mario = CMario::GetInstance();
	ableToCheckCollision = false;
	mario->StartUntouchable();
}

void CBoomerang::HandleCollisionWithMario(LPCOLLISIONEVENT e)
{
	CMario* mario = CMario::GetInstance();
	ableToCheckCollision = false;
	mario->StartUntouchable();
}
