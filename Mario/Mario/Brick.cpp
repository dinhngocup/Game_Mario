#include "Brick.h"
#include "Utils.h"
void CBrick::Render()
{
	animation_set->at(0)->Render(x, y);
	RenderBoundingBox();
}

void CBrick::RenderBoundingBox()
{
	D3DXVECTOR3 p(x, y, 0);
	RECT rect;

	LPDIRECT3DTEXTURE9 bbox = CTextures::GetInstance()->Get(-100);

	float l, t, r, b;

	GetBoundingBox(l, t, r, b);

	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	CGame::GetInstance()->Draw(x, y, bbox, rect.left, rect.top, rect.right, rect.bottom, 50);
}


CBrick::CBrick(float x, float y, float width, float height)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
}

void CBrick::GetBoundingBox(float& l, float& t, float& r, float& b, int dx, int dy)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}
