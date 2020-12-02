#include "Brick.h"
#include "Utils.h"
#include "PlayScene.h"

CBrick::CBrick(float x, float y, float w, float h, int state, int bonus)
{
	this->w = w;
	this->h = h;
	this->state = state;
	this->x = x;
	this->y = y;
	generate_id++;
	this->id = generate_id;
	this->bonus = bonus;
}

void CBrick::Render()
{
	if (state == NORMAL_BRICK)
		animation_set->at(ANI_NORMAL_BRICK)->Render(x, y);
	else if (state == BLING_BLING_BRICK) {
		animation_set->at(ANI_BLING_BLING_BRICK)->Render(x, y);
	}
	else {
		animation_set->at(ANI_EMPTY_BRICK)->Render(x, y);
	}
	RenderBoundingBox();
}



void CBrick::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state) {
	case BLING_BLING_BREAK:
		BreakBrick();
		break;
	}
}

void CBrick::BreakBrick()
{
	CGame* game = CGame::GetInstance();
	SetHealth(false);
	// create 4 debris with x,y,top, left
	CPlayScene* scene = (CPlayScene*)game->GetCurrentScene();
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			CDebris* debris = new CDebris(x, y, i, j);
			scene->effects.push_back(debris);
		}
	}
}

void CBrick::IsCollisionWithMario(LPCOLLISIONEVENT e)
{
	CMario* mario = CMario::GetInstance();
	if (e->ny != 0) {
		if (e->ny > 0){
			if (bonus == 0)
				e->obj->SetState(BLING_BLING_BREAK);
			else
				CreateBonus();
		}
		mario->vy = 0;
	}
	if (e->nx != 0 && mario->is_attacking_by_spinning) {
		if (bonus == 0)
			e->obj->SetState(BLING_BLING_BREAK);
		else
			CreateBonus();
		mario->vx = 0;
	}
}

void CBrick::CreateBonus()
{
	CGame* game = CGame::GetInstance();
	CPlayScene* scene = (CPlayScene*)game->GetCurrentScene();

	switch (bonus) {
	case P_BUTTON:
		SetState(EMPTY_BRICK);
		scene->grid->AddObjectIntoGrid(eTYPE::BUTTON_P, x, y - 48, 48, 48, BUTTON_P_ANI, eTYPE_OBJECT::ITEM);
		break;
	}
}

void CBrick::GetBoundingBox(float& l, float& t, float& r, float& b, int dx, int dy)
{
	l = x;
	t = y;
	r = x + w;
	b = y + h;
}
