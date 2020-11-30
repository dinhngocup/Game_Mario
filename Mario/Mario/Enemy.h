#pragma once
#include "GameObject.h"
#include "InvisibleObject.h"


class CEnemy : public CGameObject
{
protected:
	int start_state;
public:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx = 0, int dy = 0) {};
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render() {};
	virtual void SetState(int state) {};

	virtual void IsCollisionWithMario(LPCOLLISIONEVENT e) = 0;

	virtual void IsCollisionWithBrick(LPCOLLISIONEVENT e);
	virtual void IsCollisionWithEnemy(LPCOLLISIONEVENT e);

	virtual void AttackedByShell() {};
	
	void ResetPosition() {
		DebugOut(L"hi\n");
		this->x = start_x;
		this->y = start_y;
		SetState(start_state);
	}
};

