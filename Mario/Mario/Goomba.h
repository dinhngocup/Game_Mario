#pragma once
#include "GameObject.h"

#define GOOMBA_BBOX_WIDTH 48
#define GOOMBA_BBOX_HEIGHT 48
#define GOOMBA_ANI_WALKING 0
#define GOOMBA_ANI_DIE 1

class CGoomba : public CGameObject
{
public:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx=0, int dy=0);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	CGoomba();
	virtual void SetState(int state);
};

