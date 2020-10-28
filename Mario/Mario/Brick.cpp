#include "Brick.h"
#include "Utils.h"
void CBrick::Render()
{
	animation_set->at(0)->Render(x, y);
	RenderBoundingBox();
}


CBrick::CBrick(float x, float y, float width, float height)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
}

void CBrick::GetBoundingBox(float& l, float& t, float& r, float& b, int dx = 0, int dy = 0)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}
