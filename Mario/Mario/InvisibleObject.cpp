#include "InvisibleObject.h"

CInvisibleObject::CInvisibleObject(float x, float y, float width, float height, int state)
{
	this->x = x;
	this->y = y;
	this->w = width;
	this->h = height;
	this->state = state;
	generate_id++;
	this->id = generate_id;
	if (state == STATE_GUARD)
		vx = 0.05f;
	//DebugOut(L"state %d\n", state);
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
	animation_set->at(ani)->Render(x, y, 255, nx);
	RenderBoundingBox();
}
