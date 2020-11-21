#pragma once
#include "GameObject.h"
#include "Game.h"
#include "Mario.h"
#define LEAF_GRAVITY 0.002f
#define LEAF_RANGE	60
#define STATE_FALLING 0
#define STATE_FLYING 1
class CLeaf : public CGameObject
{
	DWORD start_time;
public:
	CLeaf(float x, float y);
	~CLeaf() {};

	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	void Render();
	void SetState(int state);
	void IsCollisionWithMario(LPCOLLISIONEVENT e);

	void GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx = 0, int dy = 0);

};

