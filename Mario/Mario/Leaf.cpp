#include "Leaf.h"

CLeaf::CLeaf(float x, float y)
{
	this->x = x;
	this->y = y;
	vy = -0.9f;
	start_x = x;
	start_y = y;
	SetState(STATE_FLYING);
}

void CLeaf::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	CGame* game = CGame::GetInstance();
	if (vy < 0)
		vy += LEAF_GRAVITY * dt;
	if (vy >= 0) {
		if (state == STATE_FLYING)
			SetState(STATE_FALLING);
		else {
			if(abs(x + dx - start_x) >= 50) {
				vx *= -1;
				nx *= -1;
			}
		}
	}

	CGameObject::Update(dt, colliable_objects);
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	vector<LPGAMEOBJECT> obj;
	CMario* mario = CMario::GetInstance();
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

void CLeaf::Render()
{
	animation_set->at(0)->Render(x, y, 255, nx, 0, 0);
	RenderBoundingBox();
}

void CLeaf::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state) {
	case STATE_FALLING:
		vy = 0.1f;
		vx = 0.1f;
		nx = 1;
		break;
	}
}

void CLeaf::IsCollisionWithMario(LPCOLLISIONEVENT e)
{
	// e is leaf
	CMario* mario = CMario::GetInstance();
	mario->vy = -MARIO_JUMP_DEFLECT_SPEED;
	int level = mario->GetLevel() + 1;
	mario->SetLevel(level);
	mario->player_state->SetAnimation(level);
	mario->AddScore(LEAF_SCORE);

	SetHealth(false);
	ableToCheckCollision = false;
}

void CLeaf::HandleCollisionWithMario(LPCOLLISIONEVENT e)
{
	// e is mario
	CMario* mario = CMario::GetInstance();
	mario->vy = -MARIO_JUMP_DEFLECT_SPEED;
	int level = mario->GetLevel() + 1;
	mario->SetLevel(level);
	mario->player_state->SetAnimation(level);
	mario->AddScore(LEAF_SCORE);

	SetHealth(false);
	ableToCheckCollision = false;
}

void CLeaf::GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx, int dy)
{
	left = x;
	top = y;
	right = left + w;
	bottom = top + h;
}
