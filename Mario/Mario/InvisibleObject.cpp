#include "InvisibleObject.h"

CInvisibleObject::CInvisibleObject(float x, float y, float width, float height)
{
	this->x = x;
	this->y = y;
	this->w = width;
	this->h = height;
	id++;
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

}

void CInvisibleObject::Render()
{
	RenderBoundingBox();
}
