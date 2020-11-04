#pragma once
#include "GameObject.h"
#include "Brick.h"
#include "Utils.h"
#include "Textures.h"
#include "Game.h"
#include "InvisibleObject.h"
#include "Koopa.h"
#include "Goomba.h"
#include "BrickQuestion.h"

#define FIRE_BALL_GRAVITY			0.002f


class CFireBall : public CGameObject
{
public:
	CFireBall();
	CFireBall(float start_x, float start_y, int nx);
	~CFireBall() {};

	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	void Render();
	void RenderBoundingBox();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx = 0, int dy = 0);

	void IsCollisionWithBrick(LPCOLLISIONEVENT e);
	void IsCollisionWithEnemy(LPCOLLISIONEVENT e);

};

