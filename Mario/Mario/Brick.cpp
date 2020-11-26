#include "Brick.h"
#include "Utils.h"
void CBrick::Render()
{
	//animation_set->at(0)->Render(x, y);
	RenderBoundingBox();
}



CBrick::CBrick(float x, float y, float width, float height)
{
	this->x = x;
	this->y = y;
	this->w = width;
	this->h = height;
	id++;
}

void CBrick::GetBoundingBox(float& l, float& t, float& r, float& b, int dx, int dy)
{
	l = x;
	t = y;
	r = x + w;
	b = y + h;
}
