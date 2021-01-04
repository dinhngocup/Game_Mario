#pragma once
#include "GameObject.h"
#include "Mario.h"

class CMovingPlatform : public CGameObject
{
public:
	CMovingPlatform();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx = 0, int dy = 0);
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void SetState(int state);
	void IsCollisionWithMario(LPCOLLISIONEVENT e);
	~CMovingPlatform() {};
};

