#pragma once
#include "GameObject.h"
#include "Item.h"
#include "Mario.h"

#define STATE_NOT_PRESSED_YET 0
#define STATE_PRESSED 1

#define ANI_NOT_PRESSED_YET 0
#define ANI_PRESSED 1

#define BUTTON_PRESSED_HEIGHT 21

#define TIME_CONVERT_COIN_TO_BLINGBLING	3000

#define BLINGBLING_WIDTH	48
#define BLINGBLING_HEIGHT	48

#define COIN_WIDTH	14
#define COIN_HEIGHT	16

#define COIN_ANI	14
#define BLINGBLING_ANI	3
#define BUTTON_P_DISPARITY	27
class CButtonP : public CItem
{
	bool is_pressed = false;

public:
	DWORD start_press = 0;
	CButtonP(float x, float y);
	~CButtonP() {};

	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	void Render();
	void SetState(int state);
	void CreateBonus();
	void IsCollisionWithMario(LPCOLLISIONEVENT e);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx = 0, int dy = 0);
};


