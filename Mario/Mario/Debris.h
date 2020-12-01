#pragma once
#include "GameObject.h"
#include "Mario.h"

#define ANI_DEBRIS 20

#define VY_TOP -0.9f
#define VY_BOTTOM -0.7f

#define VX_LEFT -0.2f
#define VX_RIGHT 0.2f

#define DEBRIS_GRAVITY 0.002f

class CDebris : public CGameObject
{
	int dir_top_bottom, dir_left_right;
public:
	CDebris(float x, float y, int dir_top_bottom, int dir_left_right);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx = 0, int dy = 0) {};
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	~CDebris() {};
};

