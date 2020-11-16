#include "BrickQuestion.h"
#include "Game.h"
#include "PlayScene.h"

CBrickQuestion::CBrickQuestion()
{
	SetState(STATE_NORMAL);
	//this->vx = 0.001;
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
