﻿#pragma once
#include "DefineLibs.h"
#include "AnimationSets.h"
#include "ObjectType.h"
class CGameObject;
typedef CGameObject* LPGAMEOBJECT;


struct CCollisionEvent;
typedef CCollisionEvent* LPCOLLISIONEVENT;
struct CCollisionEvent
{
	LPGAMEOBJECT obj;
	float t, nx, ny;

	float dx, dy;		// *RELATIVE* movement distance between this object and obj

	CCollisionEvent(float t, float nx, float ny, float dx = 0, float dy = 0, LPGAMEOBJECT obj = NULL)
	{
		this->t = t;
		this->nx = nx;
		this->ny = ny;
		this->dx = dx;
		this->dy = dy;
		this->obj = obj;
	}

	static bool compare(const LPCOLLISIONEVENT& a, LPCOLLISIONEVENT& b)
	{
		return a->t < b->t;
	}
};


class CGameObject
{
public:
	static int generate_id;
	int id;
	eTYPE type;
	int type_object;

	float x;
	float y;

	bool health = true;
	bool is_in_grid = false;
	bool is_appeared = false;

	float dx;	// dx = vx*dt
	float dy;	// dy = vy*dt

	float vx;
	float vy;

	int nx;

	int state;
	int id_grid;

	DWORD dt;

	LPANIMATION_SET animation_set;
	bool ableToCheckCollision = true;
	
	float start_x, start_y;


public:
	int h, w;
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	void SetSpeed(float vx, float vy) { this->vx = vx, this->vy = vy; }
	void GetPosition(float& x, float& y) { x = this->x; y = this->y; }
	float GetX() { return x; }
	float GetY() { return y; }
	void SetX(float x) { this->x = x; }
	void SetY(float y) { this->y = y; }
	void SetId(int id) { this->id = id; }
	int GetId() { return id; }
	eTYPE GetType() { return this->type; }

	void SetHealth(bool health) { this->health = health; }
	bool GetHealth() { return this->health; }
	bool GetStatusCollision() { return this->ableToCheckCollision; }
	void GetSpeed(float& vx, float& vy) { vx = this->vx; vy = this->vy; }

	int GetState() { return this->state; }
	
	virtual void RenderBoundingBox();

	void SetStartPosition(float start_x, float start_y) {
		this->start_x = start_x;
		this->start_y = start_y;
	}
	
	CGameObject();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx = 0, int dy = 0) = 0;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
	virtual void Render() = 0;
	virtual void SetState(int state) { this->state = state; }
	virtual void IsCollisionWithMario(LPCOLLISIONEVENT e) {};
	virtual void IsCollisionWithGhostPlatform(LPCOLLISIONEVENT e);
	virtual void ResetPosition() {};
	void SetAnimationSet(LPANIMATION_SET ani_set);

	~CGameObject();

	LPCOLLISIONEVENT SweptAABBEx(LPGAMEOBJECT coO, int horizontal = 0, int vertical = 0);
	void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents, int dx = 0, int dy = 0);
	void FilterCollision(
		vector<LPCOLLISIONEVENT>& coEvents,
		vector<LPCOLLISIONEVENT>& coEventsResult,
		float& min_tx,
		float& min_ty,
		float& nx,
		float& ny,
		float& rdx,
		float& rdy);
};

