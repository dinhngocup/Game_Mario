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
#define EMPTY_BRICK 3

#define ANI_NORMAL_BRICK 0
#define ANI_BLING_BLING_BRICK 1
#define ANI_EMPTY_BRICK 2



#define P_BUTTON 1
#define MUSHROOM_LEVEL_UP 2

#define BUTTON_P_ANI	21

#define P_BUTTON_W 48
#define P_BUTTON_H 48
#define P_BUTTON_DISPARITY	48

class CBrick : public CGameObject
{
	int bonus;
public:
	CBrick() {};
	CBrick(float x, float y, float w, float h, int state = NORMAL_BRICK, int bonus = 0);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx=0, int dy=0);
	void Render();
	void SetState(int state);
	void BreakBrick();
	void IsCollisionWithMario(LPCOLLISIONEVENT e);
	void CreateBonus();
	~CBrick() {};
};

