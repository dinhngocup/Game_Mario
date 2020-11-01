#include "Goomba.h"

void CGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx, int dy)
{
	left = x - GOOMBA_BBOX_WIDTH/2;
	top = y - GOOMBA_BBOX_HEIGHT/2;
	right = left + GOOMBA_BBOX_WIDTH;
	bottom = top + GOOMBA_BBOX_HEIGHT;
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
}

void CGoomba::Render()
{
	int ani = GOOMBA_ANI_WALKING;
	animation_set->at(ani)->Render(x, y);

	RenderBoundingBox();
}

CGoomba::CGoomba()
{
}

void CGoomba::SetState(int state)
{
}
