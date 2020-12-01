#include "Debris.h"

CDebris::CDebris(float x, float y, int dir_top_bottom, int dir_left_right)
{
	//DebugOut(L"new \n");
	generate_id++;
	this->id = generate_id;
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(ANI_DEBRIS);
	SetAnimationSet(ani_set);
	this->x = x;
	this->y = y;
	this->dir_left_right = dir_left_right;
	this->dir_top_bottom = dir_top_bottom;
	if (dir_top_bottom == 0)
		vy = VY_TOP;
	else
		vy = VY_BOTTOM;
	if (dir_left_right == 0)
		vx = VX_LEFT;
	else
		vx = VX_RIGHT;
}

void CDebris::Render()
{
	animation_set->at(0)->Render(x, y, 255, 1, 0, false, 1, 1);
}

void CDebris::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	DebugOut(L"hi\n");
	CGame* game = CGame::GetInstance();
	vy += DEBRIS_GRAVITY * dt;
	CGameObject::Update(dt);
	x += dx;
	y += dy;

	if (GetX() >= game->GetCamX() + game->GetScreenWidth() ||
		GetX() <= game->GetCamX() ||
		GetY() <= game->GetCamY() ||
		GetY() >= game->GetCamY() + game->GetScreenHeight()) {
		SetHealth(false);
		return;
	}
}
