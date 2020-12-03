#pragma once
#include "GameObject.h"
#include "Mario.h"
#include "Enemy.h"

#define STATE_HIDE 0
#define STATE_MOVING_UP 100
#define STATE_MOVING_DOWN 200
#define STATE_TOP 300

#define FLOWER_SPEED 0.1f

#define TIME_FLOWER_ON_TOP 2000
#define TIME_FLOWER_HIDE 2000

#define FLOWER_MIN_BOUNDARY		72
#define FLOWER_MAX_BOUNDARY		120

class CFlower : public CEnemy
{
	DWORD start_count;
public:
	CFlower() {};
	CFlower(float x, float y, int w, int h);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx = 0, int dy = 0);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);
	void IsCollisionWithMario(LPCOLLISIONEVENT e);
	
};

