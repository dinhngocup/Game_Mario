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

#define KOOPA_STATE_WALKING 100
#define KOOPA_STATE_DIE 101
#define KOOPA_STATE_WALKING_SWINGS 102
#define KOOPA_STATE_DIE_BY_WEAPON 103
#define KOOPA_STATE_SPIN 104
#define KOOPA_STATE_HOLD 105
#define KOOPA_STATE_UNHOLD 106
#define KOOPA_STATE_DIE_BY_TAIL 107
#define KOOPA_STATE_FLYING_DOWN 108
#define KOOPA_STATE_FLYING_UP 109

#define KOOPA_ANI_WALKING 0
#define KOOPA_ANI_DIE 1
#define KOOPA_ANI_SPIN 2
#define KOOPA_ANI_WALKING_SWINGS 3
#define KOOPA_ANI_ 3

#define KOOPA_DISPARITIES 15


#define KOOPA_GRAVITY 0.002f
#define REVIVAL_TIME 8000
#define KOOPA_REVIVAL_DISPARITY 30
#define KOOPA_DIRECTION_DISTANCE 27.0f

#define KOOPA_SPEED_X_DIE_BY_WEAPON 0.4f
#define KOOPA_SPEED_Y_DIE_BY_WEAPON 0.5f

#define KOOPA_HOLD	10.0f
#define KOOPA_UNHOLD	10.0f

class CKoopa : public CEnemy
{
	int koopa_ny = 1;
	DWORD start_revival;
	DWORD start_flying;
	bool revival = false;
public:
	CKoopa(int state);
	CKoopa();

	void GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx = 0, int dy = 0);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);

	void IsCollisionWithMario(LPCOLLISIONEVENT e);
	void HandleCollisionWithMario(LPCOLLISIONEVENT e);
	void IsCollisionWithEnemy(LPCOLLISIONEVENT e);
	void IsCollisionWithBrickSpecially(LPCOLLISIONEVENT e);
	void IsCollisionWithGhostPlatformSpecially(LPCOLLISIONEVENT e);
	void IsCollisionWithBrick(LPCOLLISIONEVENT e);
	void IsCollisionWithGhostPlatform(LPCOLLISIONEVENT e);
	void IsCollisionWithBlingBlingBrick(LPCOLLISIONEVENT e);
	void AttackedByShell();
	
};

