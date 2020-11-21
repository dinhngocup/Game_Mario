#include "Leaf.h"

CLeaf::CLeaf(float x, float y)
{
	generate_id++;
	this->id = generate_id;
	this->x = x;
	this->y = y;
	vy = -0.9f;
	start_x = x;
	start_y = y;
	start_time = GetTickCount64();
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
			if (GetTickCount64() - start_time >= 900) {
				vx *= -1;
				nx *= -1;
				start_time = GetTickCount64();
			}
		}
	}
	CGameObject::Update(dt, colliable_objects);

	x += dx;
	y += dy;

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
		vx = 0.06f;
		vy = 0.09f;
		nx = 1;
		break;
	}
}

void CLeaf::IsCollisionWithMario(LPCOLLISIONEVENT e)
{
	CMario* mario = CMario::GetInstance();
	mario->vy = -MARIO_JUMP_DEFLECT_SPEED;
	int level = mario->GetLevel() - 1;
	DebugOut(L"before set\n");
	mario->SetLevel(level);
	mario->player_state->SetAnimation(level);
	SetHealth(false);
}

void CLeaf::GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx, int dy)
{
	left = x;
	top = y;
	right = left + w;
	bottom = top + h;
}
