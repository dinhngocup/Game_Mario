#pragma once
#include "GameObject.h"
#include "Mario.h"
#include "Enemy.h"

#define BOOMERANG_BRO_STATE_WALKING_LEFT 100
#define BOOMERANG_BRO_STATE_WALKING_RIGHT 101
#define BOOMERANG_BRO_STATE_ATTACKING 102
#define BOOMERANG_BRO_STATE_IDLE 103
#define BOOMERANG_BRO_STATE_DIE 0

#define BOOMERANG_BRO_ANI_WALKING 0
#define BOOMERANG_BRO_ANI_ATTACKING 1
#define BOOMERANG_BRO_ANI_DIE 2

#define BOOMERANG_BRO_SPEED_X_DIE_BY_WEAPON 0.4f
#define BOOMERANG_BRO_SPEED_Y_DIE_BY_WEAPON 0.4f
#define BOOMERANG_BRO_WALKING_SPEED 0.1f

class CBoomerangBro : public CEnemy
{
	DWORD start_throwing_weapon, start_walking = 0;
	int weapon_quantity = 0;
	bool attacking = false;
public:
	CBoomerangBro();
	~CBoomerangBro() {};
	void GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx = 0, int dy = 0);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);
	void IsCollisionWithMario(LPCOLLISIONEVENT e);

};

