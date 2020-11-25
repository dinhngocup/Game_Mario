#include "FireFlowerWeapon.h"

CFireFlowerWeapon::CFireFlowerWeapon(float start_x, float start_y, int nx, int ny, int angle)
{
	generate_id++;
	this->id = generate_id;
	vy = 0.07f * ny;
	if (angle == 0) {
		vx = SPEED_MAX * nx * -1;
	}
	else {
		vx = SPEED_MIN * nx * -1;
	}

	SetPosition(start_x, start_y);
}

void CFireFlowerWeapon::GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx, int dy)
{
	left = x;
	top = y;
	right = left + 24;
	bottom = top + 24;
}

void CFireFlowerWeapon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//DebugOut(L"vu khi ban lua\n");
	CGame* game = CGame::GetInstance();
	CMario* mario = CMario::GetInstance();
	if (GetX() >= game->GetCamX() + game->GetScreenWidth() ||
		GetX() <= game->GetCamX() ||
		GetY() <= game->GetCamY() ||
		GetY() >= game->GetCamY() + game->GetScreenHeight()) {
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

			// block every object first!
			x += min_tx * dx + nx * 0.4f;
			y += min_ty * dy + ny * 0.4f;
			HandleCollisionWithMario(coEventsResult[0]);

			// clean up collision events
			for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
		}
	}
	else {
		x += dx;
		y += dy;
	}

}

void CFireFlowerWeapon::Render()
{
	animation_set->at(0)->Render(x, y);

}

void CFireFlowerWeapon::SetState(int state)
{
}

void CFireFlowerWeapon::IsCollisionWithMario(LPCOLLISIONEVENT e)
{
	CMario* mario = CMario::GetInstance();
	DebugOut(L"vc ben mario\n");
	
	mario->StartUntouchable();
	
}

void CFireFlowerWeapon::HandleCollisionWithMario(LPCOLLISIONEVENT e)
{
	CMario* mario = CMario::GetInstance();
	DebugOut(L"vc ben weapon\n");
		mario->StartUntouchable();
}