#pragma once
#include "GameObject.h"
#include "Game.h"
#include "Item.h"
#include "Mario.h"

#define STATIC_COIN_GRAVITY 0.002f
#define STATIC_COIN_SCORE 50

class CStaticCoin : public CItem
{
public:
	CStaticCoin(float x, float y);
	~CStaticCoin() {};

	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	void Render();
	void SetState(int state) {};
	void IsCollisionWithMario(LPCOLLISIONEVENT e);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx = 0, int dy = 0);

};

