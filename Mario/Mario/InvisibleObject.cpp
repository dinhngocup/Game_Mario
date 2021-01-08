#include "InvisibleObject.h"

CInvisibleObject::CInvisibleObject(float x, float y, float width, float height, int state)
{
	this->x = x;
	this->y = y;
	this->start_x = x;
	this->start_y = y;
	this->w = width;
	this->h = height;
	this->state = state;
	generate_id++;
	this->id = generate_id;
	if (state == STATE_GUARD)
		vx = 0.05f;
}

void CInvisibleObject::GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx, int dy)
{
	left = x;
	top = y;
	right = left + w;
	bottom = top + h;
}

void CInvisibleObject::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == STATE_GUARD) {
		if (vx < 0 && x < 250) {
			x = 250;
			vx = -vx;
			nx *= -1;

		}

		if (vx > 0 && x > 336) {
			x = 336;
			vx = -vx;
			nx *= -1;

		}
		CGameObject::Update(dt);
		x += dx;

	} else if (state == STATE_ARROW) {
		CGame* game = CGame::GetInstance();
		CIntroScene* scene = (CIntroScene*)game->GetCurrentScene();
		if (scene->change_level) {
			if (y == start_y)
				y += 48;
			else y = start_y;
			scene->change_level = false;
			return;
		}
	}
}

void CInvisibleObject::Render()
{
	int ani = 0;
	if (state == STATE_GHOST_PLATFORM)
		ani = ANI_GHOST_PLATFORM;
	else if (state == STATE_CACTUS)
		ani = ANI_CACTUS;
	else if (state == STATE_HELP_TAG)
		ani = ANI_HELP_TAG;
	else if (state == STATE_GUARD)
		ani = ANI_GUARD;
	else if (state == STATE_3_INTRO)
		ani = ANI_3_INTRO;
	else if (state == STATE_ARROW)
		ani = ANI_ARROW;
	animation_set->at(ani)->Render(x, y, 255, nx);
	RenderBoundingBox();
}
