#pragma once
#include "GameObject.h"
#include "Mario.h"
#define STATE_HIDE 0
#define STATE_MOVING_UP 100
#define STATE_MOVING_DOWN 200
#define STATE_ATTACKING_UP 300
#define STATE_ATTACKING_DOWN 400

#define RED_FF_ANI_MOVING_DOWN 0
#define RED_FF_ANI_ATTACKING_DOWN 1
#define RED_FF_ANI_MOVING_UP 2
#define RED_FF_ANI_ATTACKING_UP 3

#define GREEN_FF_ANI_MOVING_DOWN 4
#define GREEN_FF_ANI_ATTACKING_DOWN 5
#define GREEN_FF_ANI_MOVING_UP 6
#define GREEN_FF_ANI_ATTACKING_UP 7

#define RED_FLOWER 1
#define GREEN_FLOWER 2

#define FF_SPEED 0.08f
class CFireFlower : public CGameObject
{
	int flower_color;
	DWORD start_count;
public: 
	CFireFlower() {};
	CFireFlower(int state);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx = 0, int dy = 0);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);
	void IsCollisionWithMario(LPCOLLISIONEVENT e);

	void IsCollisionWithEnemy(LPCOLLISIONEVENT e);
};

