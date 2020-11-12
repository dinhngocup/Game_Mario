#pragma once
#include "Enemy.h"
#include "DefineLibs.h"
#include "Brick.h"
#include "Utils.h"
#include "Mario.h"
#include "Game.h"
#include "Goomba.h"
#include "HoldingState.h"

#define KOOPA_WALKING_SPEED 0.09f;
#define KOOPA_SPIN_SPEED 0.5f;
#define KOOPA_JUMP_SPEED_Y 0.5f;
#define KOOPA_JUMP_SPEED_X 0.2f;

#define KOOPA_BBOX_WIDTH 48
#define KOOPA_BBOX_HEIGHT 78
#define KOOPA_BBOX_HEIGHT_DIE 48

#define STATE_WALKING 100
#define STATE_DIE 101
#define STATE_WALKING_SWINGS 102
#define STATE_DIE_BY_WEAPON 103
#define STATE_SPIN 104
#define STATE_HOLD 105
#define STATE_UNHOLD 106

#define KOOPA_ANI_WALKING 0
#define KOOPA_ANI_DIE 1
#define KOOPA_ANI_SPIN 2
#define KOOPA_ANI_WALKING_SWINGS 3

#define KOOPA_DISPARITIES 15


#define KOOPA_GRAVITY 0.002f

class CKoopa : public CEnemy
{
public:
	CKoopa(int state);
	CKoopa();

	void GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx = 0, int dy = 0);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);

	void IsCollisionWithMario(LPCOLLISIONEVENT e);
	void IsCollisionWithEnemy(LPCOLLISIONEVENT e);
	void AttackedByShell();
	
};

