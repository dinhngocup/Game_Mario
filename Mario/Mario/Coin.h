#pragma once
#include "GameObject.h"


class CCoin : public CGameObject
{
public:
	CCoin();
	~CCoin() {};

	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	void Render();
	void SetState(int state);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx = 0, int dy = 0);

};

