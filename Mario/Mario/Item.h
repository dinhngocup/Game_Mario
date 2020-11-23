#pragma once
#include "GameObject.h"
class CItem : public CGameObject
{
public:

	CItem(float x, float y) { this->x = x; this->y = y; };
	CItem();
	~CItem() {};
	virtual void HandleCollisionWithMario(LPCOLLISIONEVENT e) {};
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL) {};
	virtual void Render() {};
	virtual void SetState(int state) {};
	virtual void IsCollisionWithMario(LPCOLLISIONEVENT e) {};
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx = 0, int dy = 0) {};
};

