#pragma once

#include "GameObject.h"
#include "Item.h"
#include "Mario.h"

#define RANDOM_BONUS_W	48
#define RANDOM_BONUS_H	48

#define STATE_RANDOM_BONUS_MUSHROOM	0
#define STATE_RANDOM_BONUS_STAR	1
#define STATE_RANDOM_BONUS_FIRE_FLOWER	2
#define STATE_GOT_BONUS_MUSHROOM	3
#define STATE_GOT_BONUS_STAR	4
#define STATE_GOT_BONUS_FIRE_FLOWER	5


#define ANI_BONUS_MUSHROOM	0
#define ANI_BONUS_STAR	1
#define ANI_BONUS_FIRE_FLOWER	2
#define ANI_GOT_BONUS_MUSHROOM	3
#define ANI_GOT_BONUS_STAR	4
#define ANI_GOT_BONUS_FIRE_FLOWER	5

#define MUSHROOM_CARD 1
#define STAR_MAN_CARD 2
#define FIRE_FLOWER_CARD 3

#define BONUS_VY -0.3f

class CRandomBonus : public CItem
{
	DWORD start_ani = 0;
	bool got_bonus = false;
public:
	CRandomBonus(float x, float y);
	~CRandomBonus() {};

	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	void Render();
	void SetState(int state);
	void IsCollisionWithMario(LPCOLLISIONEVENT e);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx = 0, int dy = 0);
	//void AddCardForMario();
};


