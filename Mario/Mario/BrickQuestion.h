#pragma once
#include "GameObject.h"
class CBrickQuestion : public CGameObject
{
public:
	CBrickQuestion() {};
	~CBrickQuestion() {};

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();


	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx = 0, int dy = 0);
};

