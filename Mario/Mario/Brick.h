#pragma once
#include "DefineLibs.h"
#include "GameObject.h"
#define BRICK_BBOX_WIDTH  48
#define BRICK_BBOX_HEIGHT 48
class CBrick : public CGameObject
{
private:
	float width, height;
public:
	CBrick() {};
	CBrick(float x, float y, float width, float height);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx, int dy);
	virtual void Render();
	~CBrick() {};
};

