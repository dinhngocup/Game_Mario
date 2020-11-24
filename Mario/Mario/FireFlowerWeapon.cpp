#include "FireFlowerWeapon.h"

CFireFlowerWeapon::CFireFlowerWeapon(float start_x, float start_y, int nx, int ny)
{
	generate_id++;
	this->id = generate_id;
	vx = 0.1f * nx * -1;
	vy = 0.1f * ny;
	
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
	DebugOut(L"vu khi ban lua\n");
	CGame* game = CGame::GetInstance();
	if (GetX() >= game->GetCamX() + game->GetScreenWidth() || 
		GetX() <= game->GetCamX() ||
		GetY() <= game->GetCamY() ||
		GetY() >= game->GetCamY() + game->GetScreenHeight()) {
		SetHealth(false);
		return;
	}
	CGameObject::Update(dt);
	x += dx;
	y += dy;
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
}

void CFireFlowerWeapon::HandleCollisionWithMario(LPCOLLISIONEVENT e)
{
}
