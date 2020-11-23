#pragma once
#include "GameObject.h"
#include "Item.h"
#define STATE_NORMAL 100
#define STATE_EMPTY 101

#define ANI_NORMAL 0
#define ANI_EMPTY 1
class CBrickQuestion : public CItem
{
	bool is_lifted = false;
	int bonus_id;
public:
	CBrickQuestion(int bonus_id);
	~CBrickQuestion() {};

	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	void Render();
	void SetState(int state);
	void CreateBonus();
	void IsCollisionWithMario(LPCOLLISIONEVENT e);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx = 0, int dy = 0);
};

