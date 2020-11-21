#pragma once
#include "GameObject.h"
#include "Game.h"
#define COIN_GRAVITY 0.002f

class CCoin : public CGameObject
{
public:
	CCoin(float x, float y);
	~CCoin() {};

	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	void Render();
	void SetState(int state);

	void GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx = 0, int dy = 0);

};

