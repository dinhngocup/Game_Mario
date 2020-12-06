#include "FireFlowerWeapon.h"

CFireFlowerWeapon::CFireFlowerWeapon(float start_x, float start_y, int nx, int ny, int angle)
{
	generate_id++;
	this->id = generate_id;
	vy = FF_WEAPON_SPEED_Y * ny;
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
	right = left + FF_WEAPON_BB_WIDTH;
	bottom = top + FF_WEAPON_BB_HEIGHT;
}

void CFireFlowerWeapon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	
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
		DebugOut(L"cham weapon FIRE FLOWER\n");

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
	ableToCheckCollision = false;
	mario->StartUntouchable();
	
}

void CFireFlowerWeapon::HandleCollisionWithMario(LPCOLLISIONEVENT e)
{
	CMario* mario = CMario::GetInstance();
	ableToCheckCollision = false;
	mario->StartUntouchable();
}
