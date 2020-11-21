#include "BrickQuestion.h"
#include "Game.h"
#include "PlayScene.h"

CBrickQuestion::CBrickQuestion(int bonus_id)
{
	generate_id++;
	this->id = generate_id;
	this->bonus_id = bonus_id;
	SetState(STATE_NORMAL);
}

void CBrickQuestion::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	if (state == STATE_EMPTY && !is_lifted) {
		if (vy == 0)
			vy -= 0.02 * dt;
		else
			vy += 0.005 * dt;
		if (y >= start_y && vy > 0) {
			y = start_y;
			vy = 0;
			is_lifted = true;
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
	else if (state == STATE_EMPTY)
		ani = ANI_EMPTY;

	animation_set->at(ani)->Render(x, y, 255, 1, 0, 0);

	RenderBoundingBox();
}

void CBrickQuestion::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case STATE_NORMAL:

		break;
	case STATE_EMPTY:
		CreateBonus();
		break;
	}

}

void CBrickQuestion::CreateBonus()
{
	CGame* game = CGame::GetInstance();
	CPlayScene* scene = (CPlayScene*)game->GetCurrentScene();
	switch (bonus_id) {
	case eTYPE::COIN:
		scene->grid->AddObjectIntoGrid(eTYPE::COIN, x, y - 10, 0, 0, COIN_ANI, eTYPE_OBJECT::ITEM);
		break;
	case eTYPE::MUSHROOM:
		scene->grid->AddObjectIntoGrid(eTYPE::MUSHROOM, x, y - 30, 48, 48, MUSHROOM_ANI, eTYPE_OBJECT::ITEM);
		break;
	case eTYPE::LEAF:
		scene->grid->AddObjectIntoGrid(eTYPE::LEAF, x, y - 30, 48, 48, LEAF_ANI, eTYPE_OBJECT::ITEM);
		break;
	default:
		break;
	}
}

void CBrickQuestion::GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx, int dy)
{
	left = x;
	top = y;
	right = left + 48;
	bottom = top + 48;
}
