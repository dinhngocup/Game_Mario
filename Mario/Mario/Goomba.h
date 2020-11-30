#pragma once
#include "Enemy.h"
#include "Brick.h"
#include "Mario.h"

#define GOOMBA_BBOX_WIDTH 48
#define GOOMBA_BBOX_HEIGHT 48
#define GOOMBA_BBOX_HEIGHT_DIE 48

#define GOOMBA_WALKING_SPEED 0.08f
#define GOOMBA_JUMP_SPEED_Y 0.8f
#define GOOMBA_JUMP_SPEED_X 0.2f

#define GOOMBA_JUMP_SPEED_Y_LOW 0.4f
#define GOOMBA_JUMP_SPEED_X_LOW 0.15f

#define GOOMBA_STATE_WALKING 100
#define GOOMBA_STATE_DIE 101
#define GOOMBA_STATE_WALKING_SWINGS 102
#define GOOMBA_STATE_DIE_BY_WEAPON 103
#define GOOMBA_STATE_WALKING_SWINGS_OPEN 104
#define GOOMBA_STATE_WALKING_SWINGS_CLOSE 105
#define GOOMBA_STATE_JUMPING_SWINGS_CLOSE 106

#define GOOMBA_ANI_WALKING 0
#define GOOMBA_ANI_DIE 1
#define GOOMBA_ANI_DIE_BY_WEAPON 2
#define GOOMBA_ANI_WALKING_SWINGS 3
#define GOOMBA_ANI_WALKING_SWINGS_OPEN 4
#define GOOMBA_ANI_WALKING_SWINGS_CLOSE 5

#define GOOMBA_DISPARITIES 15


#define GOOMBA_GRAVITY 0.002f

class CGoomba : public CEnemy
{
	DWORD start_walking = 0;
	int index = 0;
	float boundary_left, boundary_right;
	bool change = false;
	bool changed = false;
	bool smart_goomba = false;
public:
	CGoomba() {};
	CGoomba(int state);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx=0, int dy=0);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);
	void IsCollisionWithMario(LPCOLLISIONEVENT e);
	void IsCollisionWithBrickSpecially(LPCOLLISIONEVENT e);
	void IsCollisionWithGhostPlatformSpecially(LPCOLLISIONEVENT e);
	void AttackedByShell();
	void IsCollisionWithEnemy(LPCOLLISIONEVENT e);
	void UpdateWidthHeightGoomba();
	//void RenderBoundingBox();
};

