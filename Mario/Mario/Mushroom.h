#pragma once
#include "GameObject.h"
#include "Game.h"
#include "AnimationSets.h"
#include "Item.h"
#include "GrowingUpState.h"

#define MUSHROOM_GRAVITY 0.002f

#define MUSHROOM_STATE_HIDE 0
#define MUSHROOM_STATE_UNHIDE 1
#define MUSHROOM_SCORE 1000

#define MUSHROOM_VX 0.08f

#define ANI_RED_MUSHROOM 0
#define ANI_GREEN_MUSHROOM 1

class CMushroom : public CItem
{
	int extra;
public:
	CMushroom(float x, float y, int extra = 0);
	~CMushroom() {};

	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	void Render();
	void SetState(int state);
	void IsCollisionWithMario(LPCOLLISIONEVENT e);
	void IsCollisionWithBrick(LPCOLLISIONEVENT e);
	void HandleCollisionWithMario(LPCOLLISIONEVENT e);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx = 0, int dy = 0);
};

