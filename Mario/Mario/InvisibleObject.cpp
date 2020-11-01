#include "InvisibleObject.h"

CInvisibleObject::CInvisibleObject(float x, float y, float width, float height)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
}

void CInvisibleObject::GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx, int dy)
{
	left = x;
	top = y;
	right = left + width;
	bottom = top + height;
}

void CInvisibleObject::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

}

void CInvisibleObject::Render()
{
	RenderBoundingBox();
}

void CInvisibleObject::RenderBoundingBox()
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

