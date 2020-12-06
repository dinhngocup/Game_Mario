#pragma once
#include "GameObject.h"
#include "Mario.h"

#define ANI_POINT_BONUS 23
#define TIME_POINT_EFFECT 300

#define STATE_100_POINTS 0
#define STATE_200_POINTS 1
#define STATE_1000_POINTS 2
#define STATE_1UP 3

#define ANI_100_POINTS 0
#define ANI_200_POINTS 1
#define ANI_1000_POINTS 2
#define ANI_1UP 3

#define POINT_100 100
#define POINT_200 200
#define POINT_1UP 300
#define POINT_1000 1000

#define VY_POINT_BONUS -0.1f

class CPointBonus : public CGameObject
{
public:
	DWORD start_ani;
	void GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx = 0, int dy = 0) {};
	CPointBonus(float x, float y, int state);
	void SetState(int state);
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	~CPointBonus() {};
};
