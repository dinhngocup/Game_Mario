#include "RandomBonus.h"
#include "PlayScene.h"

CRandomBonus::CRandomBonus(float x, float y)
{
	//DebugOut(L"NEW RANDOM BONUS \n");
	this->x = x;
	this->y = y;
	SetState(STATE_RANDOM_BONUS_MUSHROOM);
	start_ani = GetTickCount64();
}

void CRandomBonus::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	CGame* game = CGame::GetInstance();
	CMario* mario = CMario::GetInstance();
	CPlayScene* scene = (CPlayScene*)game->GetCurrentScene();

	if (!got_bonus) {
		if (GetTickCount64() - start_ani >= 500) {
			switch (state) {
			case STATE_RANDOM_BONUS_MUSHROOM:
				SetState(STATE_RANDOM_BONUS_STAR);
				break;
			case STATE_RANDOM_BONUS_STAR:
				SetState(STATE_RANDOM_BONUS_FIRE_FLOWER);
				break;
			case STATE_RANDOM_BONUS_FIRE_FLOWER:
				SetState(STATE_RANDOM_BONUS_MUSHROOM);
				break;
			}
			start_ani = GetTickCount64();
		}
	}
	else {
		CGameObject::Update(dt);
		x += dx;
		y += dy;
		if (y + h < game->GetCamY()) {
			scene->allow_render_first_row_title = true;
			scene->start_count = GetTickCount64();
			SetHealth(false);
		}
	}
} 

void CRandomBonus::Render()
{
	int ani = 0;
	if (state == STATE_RANDOM_BONUS_MUSHROOM)
		ani = ANI_BONUS_MUSHROOM;
	else if (state == STATE_RANDOM_BONUS_STAR)
		ani = ANI_BONUS_STAR;
	else if (state == STATE_RANDOM_BONUS_FIRE_FLOWER)
		ani = ANI_BONUS_FIRE_FLOWER;
	else if (state == STATE_GOT_BONUS_MUSHROOM)
		ani = ANI_GOT_BONUS_MUSHROOM;
	else if (state == STATE_GOT_BONUS_STAR)
		ani = ANI_GOT_BONUS_STAR;
	else if (state == STATE_GOT_BONUS_FIRE_FLOWER)
		ani = ANI_GOT_BONUS_FIRE_FLOWER;

	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}

void CRandomBonus::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state) {
	case STATE_GOT_BONUS_MUSHROOM:
	case STATE_GOT_BONUS_STAR:
	case STATE_GOT_BONUS_FIRE_FLOWER:
		vy = BONUS_VY;
		break;
	}
}

void CRandomBonus::IsCollisionWithMario(LPCOLLISIONEVENT e)
{
	CRandomBonus* random_bonus = dynamic_cast<CRandomBonus*>(e->obj);
	CGame* game = CGame::GetInstance();
	CMario* mario = CMario::GetInstance();
	CPlayScene* scene = (CPlayScene*)game->GetCurrentScene();
	scene->time_scale = 0;

	switch (random_bonus->state) {
	case STATE_RANDOM_BONUS_MUSHROOM:
		SetState(STATE_GOT_BONUS_MUSHROOM);
		scene->mario_end_bonus = MUSHROOM_CARD;
		break;
	case STATE_RANDOM_BONUS_STAR:
		SetState(STATE_GOT_BONUS_STAR);
		scene->mario_end_bonus = STAR_MAN_CARD;
		break;
	case STATE_RANDOM_BONUS_FIRE_FLOWER:
		SetState(STATE_GOT_BONUS_FIRE_FLOWER);
		scene->mario_end_bonus = FIRE_FLOWER_CARD;
		break;
	}
	

	got_bonus = true;

	
}

void CRandomBonus::GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx, int dy)
{
	left = x;
	top = y;
	right = left + w;
	bottom = top + h;
}


