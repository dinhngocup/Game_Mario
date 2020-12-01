#pragma once
#include "GameObject.h"
#include "Mario.h"
#include "Enemy.h"

#define STATE_HIDE 0
#define STATE_MOVING_UP 100
#define STATE_MOVING_DOWN 200
#define STATE_TOP 300

#define FLOWER_SPEED 0.1f

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

