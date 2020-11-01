#include "Koopa.h"

CKoopa::CKoopa()
{
}

CKoopa::CKoopa(int state)
{
	SetState(state);
	//SetState(KOOPA_STATE_DIE);
}

void CKoopa::GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx, int dy)
{
	left = x - KOOPA_BBOX_WIDTH / 2;
	right = left + KOOPA_BBOX_WIDTH;

	if (state == KOOPA_STATE_DIE || state == KOOPA_STATE_DIE_BY_WEAPON) {
		top = y - KOOPA_BBOX_HEIGHT_DIE / 2;
		bottom = top + KOOPA_BBOX_HEIGHT_DIE;
	}
	else {
		top = y - KOOPA_BBOX_HEIGHT / 2;
		bottom = top + KOOPA_BBOX_HEIGHT;
	}
	//DebugOut(L"t %f\n", top);
}

void CKoopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//DebugOut(L"ruaaa\n");
	CGameObject::Update(dt);
	// Simple fall down
	vy += KOOPA_GRAVITY * dt;
	//DebugOut(L"y %f\n", y);
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	
	vector<LPGAMEOBJECT>* bricks = new vector<LPGAMEOBJECT>();
	for (auto x : *coObjects)
	{
		if (dynamic_cast<CBrick*>(x))
			bricks->push_back(x);
	}

	coEvents.clear();
	if(state != KOOPA_STATE_DIE_BY_WEAPON)
		CalcPotentialCollisions(bricks, coEvents);

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
		//DebugOut(L"y rùa %f\n", y);

		if (nx != 0) vx *= -1;
		if (ny != 0) vy = 0;

		// clean up collision events
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
	CGame* game = CGame::GetInstance();
	if (GetY() > game->GetCamY() + game->GetScreenHeight()) {
		SetHealth(false);
	}
}

void CKoopa::Render()
{
	int ani = 0;
	int ny = 1;
	if (state == KOOPA_STATE_DIE)
		ani = KOOPA_ANI_DIE;
	else if (state == KOOPA_STATE_WALKING)
		ani = KOOPA_ANI_WALKING;
	else if (state == KOOPA_STATE_WALKING_SWINGS)
		ani = KOOPA_ANI_WALKING_SWINGS;
	else if (state == KOOPA_STATE_DIE_BY_WEAPON) {
		ani = KOOPA_ANI_DIE;
		ny = -1;
	}
	if (vx >= 0)
		animation_set->at(ani)->Render(x, y, 255, -1, 0, 0, ny);
	else
		animation_set->at(ani)->Render(x, y, 255, 1, 0, 0, ny);

	RenderBoundingBox();
}

void CKoopa::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPA_STATE_WALKING:
		vx = -KOOPA_WALKING_SPEED;
		vx = 0;
		break;
	case KOOPA_STATE_DIE:
		y += KOOPA_DISPARITIES;
		vx = 0;
		vy = 0;
		break;
	case KOOPA_STATE_DIE_BY_WEAPON:
		y += KOOPA_DISPARITIES;
		vx = 0.2f;
		vy = -0.5f;
		break;
	case KOOPA_STATE_WALKING_SWINGS:
		vx = -KOOPA_WALKING_SPEED;
		break;
	}

}