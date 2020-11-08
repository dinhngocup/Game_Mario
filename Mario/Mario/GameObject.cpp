#include "GameObject.h"
#include "Animations.h"
#include "DefineLibs.h"
#include "Game.h"
#include "Textures.h"
#include "Utils.h"
#include <d3d9types.h>

void CGameObject::RenderBoundingBox()
{

	D3DXVECTOR3 p(x, y, 0);
	RECT rect;

	LPDIRECT3DTEXTURE9 bbox = CTextures::GetInstance()->Get(-100);

	float l, t, r, b;

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	float draw_x = x - ((int)r - (int)l) / 2;
	float draw_y = y - ((int)b - (int)t) / 2;

	CGame::GetInstance()->Draw(draw_x, draw_y, bbox, rect.left, rect.top, rect.right, rect.bottom, 80);

}

void CGameObject::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	this->dt = dt;
	dx = vx * dt;
	dy = vy * dt;
}

CGameObject::CGameObject()
{
	x = y = 0;
	vx = vy = 0;
	nx = 1;
}

void CGameObject::IsCollisionWithGhostPlatform(LPCOLLISIONEVENT e)
{
	if (e->nx != 0) x += dx;
	if (e->ny < 0)
		vy = 0;
	else if (e->ny > 0)
		y += dy;
}

void CGameObject::SetAnimationSet(LPANIMATION_SET ani_set)
{
	animation_set = ani_set;
}

CGameObject::~CGameObject()
{

}


/*
	Extension of original SweptAABB to deal with two moving objects
*/
LPCOLLISIONEVENT CGameObject::SweptAABBEx(LPGAMEOBJECT coO, int horizontal, int vertical)
{
	float sl, st, sr, sb;		// static object bbox
	float ml, mt, mr, mb;		// moving object bbox
	float t, nx = 0, ny;

	coO->GetBoundingBox(sl, st, sr, sb);

	// deal with moving object: m speed = original m speed - collide object speed
	float svx, svy;
	coO->GetSpeed(svx, svy);

	float sdx = svx * dt;
	float sdy = svy * dt;

	// (rdx, rdy) is RELATIVE movement distance/velocity 
	float rdx = this->dx - sdx;
	float rdy = this->dy - sdy;


	if (horizontal || vertical)
	{
		GetBoundingBox(ml, mt, mr, mb, horizontal, vertical);
	}
	else
	{
		GetBoundingBox(ml, mt, mr, mb);
	}
	CGame::SweptAABB(
		ml, mt, mr, mb,
		rdx, rdy,
		sl, st, sr, sb,
		t, nx, ny
	);


	CCollisionEvent* e = new CCollisionEvent(t, nx, ny, rdx, rdy, coO);
	return e;
}

/*
	Calculate potential collisions with the list of colliable objects

	coObjects: the list of colliable objects
	coEvents: list of potential collisions
*/
void CGameObject::CalcPotentialCollisions(
	vector<LPGAMEOBJECT>* coObjects,
	vector<LPCOLLISIONEVENT>& coEvents,
	int dx, int dy)
{

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (coObjects->at(i)->ableToCheckCollision) {

			LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i), dx, dy);

			if (e->t > 0 && e->t <= 1.0f)
				coEvents.push_back(e);
			else
				delete e;
		}
		
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}

void CGameObject::FilterCollision(
	vector<LPCOLLISIONEVENT>& coEvents,
	vector<LPCOLLISIONEVENT>& coEventsResult,
	float& min_tx, float& min_ty,
	float& nx, float& ny, float& rdx, float& rdy)
{
	min_tx = 1.0f;
	min_ty = 1.0f;
	int min_ix = -1;
	int min_iy = -1;

	nx = 0.0f;
	ny = 0.0f;

	coEventsResult.clear();

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		LPCOLLISIONEVENT c = coEvents[i];

		if (c->t < min_tx && c->nx != 0) {
			min_tx = c->t; nx = c->nx; min_ix = i; rdx = c->dx;
		}

		if (c->t < min_ty && c->ny != 0) {
			min_ty = c->t; ny = c->ny; min_iy = i; rdy = c->dy;
		}
	}
	if (min_ix >= 0) {
		coEventsResult.push_back(coEvents[min_ix]);
	}

	if (min_iy >= 0) {
		coEventsResult.push_back(coEvents[min_iy]);
	}
}
