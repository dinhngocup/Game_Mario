#pragma once
#include "GameObject.h"
#include "Brick.h"
#include "Utils.h"
#include "Textures.h"
#include "Game.h"
#include "InvisibleObject.h"
#include "Koopa.h"
#include "BrickQuestion.h"

#define FIRE_BALL_GRAVITY			0.002f


class CFireBall : public CGameObject
{
	float collision_y;
public:
	CFireBall();
	CFireBall(float start_x, float start_y, int nx);
	~CFireBall() { delete this; };

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	virtual void RenderBoundingBox();


	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx = 0, int dy = 0);
};

