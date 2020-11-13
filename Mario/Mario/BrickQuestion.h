#pragma once
#include "GameObject.h"

#define STATE_NORMAL 100
#define STATE_EMPTY 101

#define ANI_NORMAL 0
#define ANI_EMPTY 1
class CBrickQuestion : public CGameObject
{
	bool is_lifted = false;
public:
	CBrickQuestion();
	~CBrickQuestion() {};

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	void SetState(int state);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx = 0, int dy = 0);
};

