#pragma once
#include "GameObject.h"
#include "Game.h"
#include "Item.h"
#include "Mario.h"
#include "PointBonus.h"

#define COIN_GRAVITY 0.002f
#define COIN_SCORE 100

#define COIN_VY -0.7f

class CCoin : public CItem
{
public:
	CCoin(float x, float y);
	~CCoin() {};

	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	void Render();
	void SetState(int state);
	void IsCollisionWithMario(LPCOLLISIONEVENT e);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx = 0, int dy = 0);

};

