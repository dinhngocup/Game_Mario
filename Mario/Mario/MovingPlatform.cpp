#include "MovingPlatform.h"

CMovingPlatform::CMovingPlatform()
{
	generate_id++;
	this->id = generate_id;
	vx = -0.1f;
}

void CMovingPlatform::GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx, int dy)
{
	left = x;
	top = y;

	right = left + w;
	bottom = top + h;
}

void CMovingPlatform::Render()
{
	animation_set->at(0)->Render(x, y);
	RenderBoundingBox();
}

void CMovingPlatform::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGame* game = CGame::GetInstance();
	CMario* mario = CMario::GetInstance();
	CGameObject::Update(dt);
	x += dx;
	y += dy;

	if (y + h > game->GetCamY() + game->GetScreenHeight() ||
		x + w < game->GetCamX()) {
		SetHealth(false);
		return;
	}

}



void CMovingPlatform::SetState(int state)
{
}

void CMovingPlatform::IsCollisionWithMario(LPCOLLISIONEVENT e)
{
	CMario* mario = CMario::GetInstance();
	if (e->ny != 0) {
		if (e->ny > 0) {
			mario->isInMovingPlatform = false;
			mario->vy = 0;
		}
		else {
			mario->isInMovingPlatform = true;
			mario->vy = 0.1f;
			vx = 0;
			vy = 0.1f;
		}
	}
	if (e->nx != 0) {
		mario->vx = 0;
	}
}