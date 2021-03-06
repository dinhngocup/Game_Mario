#pragma once
#include "GameObject.h"
#include "Mario.h"

#define ANI_TRANSFORM 10
#define TIME_TRANSFORM_EFFECT 480

#define TRANSFORM_EFFECT_W	16
#define TRANSFORM_EFFECT_H	16


class CTransform : public CGameObject
{
private:
	DWORD start_ani;
public:
	void GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx = 0, int dy = 0);
	CTransform(float x, float y);
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	~CTransform() {};
};

