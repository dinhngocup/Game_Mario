#include "BrickQuestion.h"
#include "Game.h"
#include "PlayScene.h"

CBrickQuestion::CBrickQuestion(int bonus)
{
	this->bonus = bonus;
	SetState(STATE_NORMAL);
}


void CBrickQuestion::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	if (state == STATE_EMPTY && !is_lifted) {
		if (vy == 0)
			vy -= BRICK_QUESTION_SPEED_Y_LIFT * dt;
		else
			vy += SPEED_Y_FALLING * dt;
		if (y >= start_y && vy > 0) {
			y = start_y;
			vy = 0;
			is_lifted = true;
			SetState(STATE_EMPTY_BONUS);
		}
		CGameObject::Update(dt);
		y += dy;
	}

}

void CBrickQuestion::Render()
{
	int ani = 0;
	if (state == STATE_NORMAL)
		ani = ANI_NORMAL;
	else 
		ani = ANI_EMPTY;

	animation_set->at(ani)->Render(x, y, 255, 1, 0, 0);

	RenderBoundingBox();
}

void CBrickQuestion::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case STATE_EMPTY_BONUS:
			CreateBonus();
		break;
	}

}

void CBrickQuestion::CreateBonus()
{
	CGame* game = CGame::GetInstance();
	CPlayScene* scene = (CPlayScene*)game->GetCurrentScene();
	CMario* mario = CMario::GetInstance();
	int level = mario->GetLevel();
	if (bonus == 1) {
		scene->grid->AddObjectIntoGrid(eTYPE::COIN, x + w / 4, y - 10, 0, 0, COIN_ANI, eTYPE_OBJECT::ITEM);
	}
	else {
		switch (level) {
		case MARIO_LEVEL_SMALL:
			scene->grid->AddObjectIntoGrid(eTYPE::MUSHROOM, x, y, 48, 48, MUSHROOM_ANI, eTYPE_OBJECT::ITEM);
			break;
		case MARIO_LEVEL_BIG:
			scene->grid->AddObjectIntoGrid(eTYPE::LEAF, x, y - 30, 48, 48, LEAF_ANI, eTYPE_OBJECT::ITEM);
			break;
		case RACCOON_LEVEL_BIG:
			scene->grid->AddObjectIntoGrid(eTYPE::COIN, x + w / 4, y - 10, 0, 0, COIN_ANI, eTYPE_OBJECT::ITEM);
			break;
		case FIRE_LEVEL:
			scene->grid->AddObjectIntoGrid(eTYPE::COIN, x + w / 4, y - 10, 0, 0, COIN_ANI, eTYPE_OBJECT::ITEM);
			break;
		default:
			break;
		}
	}
}

void CBrickQuestion::IsCollisionWithMario(LPCOLLISIONEVENT e)
{
	CMario* mario = CMario::GetInstance();
	if (e->ny != 0) {
		if (e->ny > 0 && state == STATE_NORMAL) {
			mario->vy = 0.0f;
			SetState(STATE_EMPTY);
		}
		mario->vy = 0;
	}
	if (e->nx != 0) {
		if (mario->is_attacking_by_spinning) {
			SetState(STATE_EMPTY);
			mario->vx = 0;
		}
		else {
			mario->vx = 0;
		}
	}
}

void CBrickQuestion::GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx, int dy)
{
	left = x;
	top = y;
	right = left + w;
	bottom = top + h;
}
