#pragma once
#include "GameObject.h"
#include "Item.h"
#define STATE_NORMAL 100
#define STATE_EMPTY 101
#define STATE_EMPTY_BONUS 102

#define ANI_NORMAL 0
#define ANI_EMPTY 1

#define BRICK_QUESTION_SPEED_Y_LIFT	0.02f
#define SPEED_Y_FALLING	0.005f

class CBrickQuestion : public CItem
{
	bool is_lifted = false;
	int bonus;
public:
	CBrickQuestion(int bonus);
	~CBrickQuestion() {};

	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	void Render();
	void SetState(int state);
	void CreateBonus();
	void IsCollisionWithMario(LPCOLLISIONEVENT e);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx = 0, int dy = 0);
};

