#pragma once
#include "Enemy.h"
#include "Mario.h"
#include "GameObject.h"

#define ANI_FF_WEAPON	4

class CFireFlowerWeapon : public CEnemy
{
public:
	CFireFlowerWeapon(float start_x, float start_y, int nx, int ny);
	CFireFlowerWeapon() { };
	~CFireFlowerWeapon() {};

	void GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx = 0, int dy = 0);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);

	void IsCollisionWithMario(LPCOLLISIONEVENT e);
	void HandleCollisionWithMario(LPCOLLISIONEVENT e);
	

};


