#pragma once
#include "Enemy.h"
#include "Mario.h"
#include "GameObject.h"

#define BOOMERANG_BB_WIDTH 48
#define BOOMERANG_BB_HEIGHT 48


class CBoomerang : public CEnemy
{
	DWORD start_count = 0;
public:
	CBoomerang(float start_x, float start_y);
	CBoomerang() { };
	~CBoomerang() {};

	void GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx = 0, int dy = 0);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);

	void IsCollisionWithMario(LPCOLLISIONEVENT e);
	void HandleCollisionWithMario(LPCOLLISIONEVENT e);


};


