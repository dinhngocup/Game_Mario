#pragma once
#include "GameObject.h"
#include "Mario.h"

#define ANI_TRANSFORM 18

class CStarEffect : public CGameObject
{
public:
	DWORD start_ani;
	void GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx = 0, int dy = 0) {};
	CStarEffect(float x, float y);
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	~CStarEffect() {};
};

