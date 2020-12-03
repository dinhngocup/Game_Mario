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
#define ANI_FIRE_BALL	4

#define FIRE_BALL_STATE_FLY 100
#define FIRE_BALL_STATE_EXPLODE 101

#define FIRE_BALL_ANI_FLY 0
#define FIRE_BALL_ANI_EXPLODE 1

#define EXPLOSIVE_TIME 240

#define FIRE_BALL_VX 0.9f
#define FIRE_BALL_W 24
#define FIRE_BALL_H 24

class CFireBall : public CGameObject
{
	DWORD start_ani;
public:
	CFireBall();
	CFireBall(float start_x, float start_y, int nx);
	~CFireBall() {};

	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	void Render();
	void SetState(int state);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx = 0, int dy = 0);

	void IsCollisionWithBrick(LPCOLLISIONEVENT e);
	void IsCollisionWithEnemy(LPCOLLISIONEVENT e);

};

