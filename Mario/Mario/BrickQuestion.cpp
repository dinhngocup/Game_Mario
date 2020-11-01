#include "BrickQuestion.h"

void CBrickQuestion::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
}

void CBrickQuestion::Render()
{
	animation_set->at(0)->Render(x, y);
	//RenderBoundingBox();
}

void CBrickQuestion::GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx, int dy)
{
	left = x - 24;
	top = y - 24;
	right = left + 48;
	bottom = top + 48;
}
