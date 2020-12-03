#include "Flower.h"
#include "PlayScene.h"

CFlower::CFlower(float x, float y, int w, int h)
{
	generate_id++;
	this->id = generate_id;
	start_x = x;
	start_y = y;
	start_state = STATE_MOVING_UP;
	SetState(STATE_MOVING_UP);

}

void CFlower::GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx, int dy)
{
	left = x;
	top = y;

	right = left + w;
	bottom = top + h;
}

void CFlower::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CMario* mario = CMario::GetInstance();
	if (state == STATE_MOVING_UP) {
		if (y <= start_y - h) {
			y = start_y - h;
			SetState(STATE_TOP);
		}
		else y += dy;
	}
	else if (state == STATE_MOVING_DOWN) {
		if (y >= start_y)
			SetState(STATE_HIDE);
		else y += dy;
	}
	else if (state == STATE_TOP) {
		if (GetTickCount64() - start_count >= TIME_FLOWER_ON_TOP) {
			SetState(STATE_MOVING_DOWN);
		}
	}
	else {
		// state là hide
		if (mario->x >= start_x - FLOWER_MIN_BOUNDARY && mario->x <= start_x + FLOWER_MAX_BOUNDARY) {

		}
		else {
			if (GetTickCount64() - start_count >= TIME_FLOWER_HIDE) {
				SetState(STATE_MOVING_UP);
			}
		}
	}
	CGameObject::Update(dt);
	x += dx;
	y += dy;
}

void CFlower::Render()
{
	animation_set->at(0)->Render(x, y);
	RenderBoundingBox();
}

void CFlower::SetState(int state)
{
	CGameObject::SetState(state);
	CMario* mario = CMario::GetInstance();
	CGame* game = CGame::GetInstance();
	switch (state) {
	case STATE_TOP:
	case STATE_HIDE:
		vy = 0;
		start_count = GetTickCount64();
		break;
	case STATE_MOVING_DOWN:
		vy = FLOWER_SPEED;
		break;
	case STATE_MOVING_UP:
		vy = -FLOWER_SPEED;
		break;
	}
}

void CFlower::IsCollisionWithMario(LPCOLLISIONEVENT e)
{
	CMario* mario = CMario::GetInstance();
	if (e->nx != 0) {
		if (mario->is_attacking_by_spinning) {
			CStarEffect* effect;
			if (nx > 0) {
				effect = new CStarEffect(x + w, y + h / 2);
			}
			else {
				effect = new CStarEffect(x, y + h / 2);
			}
			effect->start_ani = GetTickCount64();
			CGame* game = CGame::GetInstance();
			CPlayScene* scene = (CPlayScene*)game->GetCurrentScene();
			scene->effects.push_back(effect);
			SetHealth(false);
			return;
		}
	}
	mario->StartUntouchable();
}
