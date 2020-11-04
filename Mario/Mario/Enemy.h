#pragma once
#include "GameObject.h"
#include "InvisibleObject.h"


class CEnemy : public CGameObject
{
public:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx = 0, int dy = 0) {};
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render() {};
	virtual void SetState(int state) {};

	virtual void IsCollisionWithMario(LPCOLLISIONEVENT e) = 0;

	void IsCollisionWithBrick(LPCOLLISIONEVENT e);
	void IsCollisionWithEnemy(LPCOLLISIONEVENT e);
	
};

