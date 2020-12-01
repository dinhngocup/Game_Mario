#pragma once
#include "DefineLibs.h"
#include "GameObject.h"
#include "Textures.h"
#include "Game.h"

#include "Debris.h"

#define BRICK_BBOX_WIDTH  48
#define BRICK_BBOX_HEIGHT 48

#define NORMAL_BRICK 0
#define BLING_BLING_BRICK 1
#define BLING_BLING_BREAK 2

#define ANI_NORMAL_BRICK 0
#define ANI_BLING_BLING_BRICK 1

#define STATE_BLING_BLING_FULL 100
#define STATE_BLING_BLING_BREAK 101

class CBrick : public CGameObject
{
public:
	CBrick() {};
	CBrick(float x, float y, float w, float h, int state = NORMAL_BRICK);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx=0, int dy=0);
	void Render();
	void SetState(int state);
	void BreakBrick();
	~CBrick() {};
};

