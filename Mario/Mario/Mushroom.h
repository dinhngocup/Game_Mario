#pragma once
#include "GameObject.h"
#include "Game.h"
#define MUSHROOM_GRAVITY 0.002f
class CMushroom : public CGameObject
{
public:
	
	CMushroom(float x, float y);
	~CMushroom() {};

	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	void Render();
	void SetState(int state);

	void GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx = 0, int dy = 0);
};

