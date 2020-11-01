#pragma once
#include "GameObject.h"
#include "DefineLibs.h"
#include "Brick.h"
#include "Utils.h"
#include "Mario.h"
#include "Game.h"

#define KOOPA_WALKING_SPEED 0.08f;

#define KOOPA_BBOX_WIDTH 48
#define KOOPA_BBOX_HEIGHT 78
#define KOOPA_BBOX_HEIGHT_DIE 48

#define KOOPA_STATE_WALKING 100
#define KOOPA_STATE_DIE 101
#define KOOPA_STATE_WALKING_SWINGS 102
#define KOOPA_STATE_DIE_BY_WEAPON 103

#define KOOPA_ANI_WALKING 0
#define KOOPA_ANI_DIE 1
#define KOOPA_ANI_SPIN 2
#define KOOPA_ANI_WALKING_SWINGS 3

#define KOOPA_DISPARITIES 15


#define KOOPA_GRAVITY 0.002f

class CKoopa : public CGameObject
{
public:
	CKoopa(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx = 0, int dy = 0);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	CKoopa();
	virtual void SetState(int state);
};

