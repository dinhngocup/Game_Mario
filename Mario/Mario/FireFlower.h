#pragma once
#include "GameObject.h"
#include "Mario.h"
#include "Enemy.h"
#include "FireFlowerWeapon.h"

#define STATE_HIDE 0
#define STATE_MOVING_UP 100
#define STATE_MOVING_DOWN 200
#define STATE_ATTACKING_UP 300
#define STATE_ATTACKING_DOWN 400

#define RED_FF_ANI_DOWN 0
#define RED_FF_ANI_ATTACKING_DOWN 1
#define RED_FF_ANI_UP 2
#define RED_FF_ANI_ATTACKING_UP 3

#define GREEN_FF_ANI_DOWN 4
#define GREEN_FF_ANI_ATTACKING_DOWN 5
#define GREEN_FF_ANI_UP 6
#define GREEN_FF_ANI_ATTACKING_UP 7

#define PIPE_ANI 8

#define RED_FLOWER 1
#define GREEN_FLOWER 2

#define FF_SPEED 0.08f
#define FF_TIME_ATTACKING	1000
#define FF_TIME_HIDE	1700

#define FF_MIN_BOUNDARY		72
#define FF_MAX_BOUNDARY		120

#define BOUNDARY_ANGLE	-192

class CFireFlower : public CEnemy
{
	int ny;
	int flower_color;
	DWORD start_count;
	float boundaryY, boundaryX_max, boudaryX_min, boundaryX_middle;
public: 
	//CFireFlower() {};
	CFireFlower(int state, float x, float y, int w, int h);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx = 0, int dy = 0);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);
	void IsCollisionWithMario(LPCOLLISIONEVENT e);
	void CreateWeapon();
	void IsCollisionWithEnemy(LPCOLLISIONEVENT e);
};

