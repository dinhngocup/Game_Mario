#include "FireFlower.h"

CFireFlower::CFireFlower(int flower_color)
{
	generate_id++;
	this->id = generate_id;
	this->flower_color = flower_color;
	SetState(STATE_MOVING_UP);
}

void CFireFlower::GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx, int dy)
{
	left = x;
	top = y;

	right = left + w;
	bottom = top + h;
}

void CFireFlower::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	
	if (state == STATE_MOVING_UP) {
		if (y <= start_y - h) {
			y = start_y - h;
			SetState(STATE_ATTACKING_UP);
		}
		else y += dy;
	}
	else if (state == STATE_MOVING_DOWN) {
		if (y >= start_y)
			SetState(STATE_HIDE);
		else y += dy;
	}
	else if (state == STATE_ATTACKING_UP || state == STATE_ATTACKING_DOWN) {
		if (GetTickCount64() - start_count >= 1000) {
			SetState(STATE_MOVING_DOWN);
		}
	}
	else {
		if (GetTickCount64() - start_count >= 500) {
			SetState(STATE_MOVING_UP);
		}
	}

}

void CFireFlower::Render()
{
	int ani;
	if (flower_color == RED_FLOWER) {
		switch (state) {
		case STATE_HIDE:
			ani = RED_FF_ANI_ATTACKING_UP;
			break;
		case STATE_ATTACKING_UP:
			ani = RED_FF_ANI_ATTACKING_UP;
			break;
		case STATE_ATTACKING_DOWN:
			ani = RED_FF_ANI_ATTACKING_DOWN;
			break;
		case STATE_MOVING_UP:
			ani = RED_FF_ANI_MOVING_UP;
			break;
		case STATE_MOVING_DOWN:
			ani = RED_FF_ANI_MOVING_DOWN;
			break;
		}
	}
	else {
		switch (state) {
		case STATE_HIDE:
			ani = GREEN_FF_ANI_ATTACKING_UP;
			break;
		case STATE_ATTACKING_UP:
			ani = GREEN_FF_ANI_ATTACKING_UP;
			break;
		case STATE_ATTACKING_DOWN:
			ani = GREEN_FF_ANI_ATTACKING_DOWN;
			break;
		case STATE_MOVING_UP:
			ani = GREEN_FF_ANI_MOVING_UP;
			break;
		case STATE_MOVING_DOWN:
			ani = GREEN_FF_ANI_MOVING_DOWN;
			break;
		}
	}
	CMario* mario = CMario::GetInstance();

	animation_set->at(ani)->Render(x, y);

	RenderBoundingBox();
}

void CFireFlower::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state) {
	case STATE_MOVING_UP:
		vy = -FF_SPEED;
		break;
	case STATE_MOVING_DOWN:
		vy = FF_SPEED;
		break;
	case STATE_ATTACKING_UP:
		start_count = GetTickCount64();
		vy = 0;
		break;
	case STATE_ATTACKING_DOWN:
		start_count = GetTickCount64();
		vy = 0;
		break;
	case STATE_HIDE:
		start_count = GetTickCount64();
		vy = 0;
		break;
	}
}

void CFireFlower::IsCollisionWithMario(LPCOLLISIONEVENT e)
{
}

void CFireFlower::IsCollisionWithEnemy(LPCOLLISIONEVENT e)
{
}
