#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Mario.h"
#include "Game.h"


#include "Koopas.h"
#include "Brick.h"
#include "MarioConst.h"
CMario* CMario::__instance = NULL;
CMario::CMario() : CGameObject()
{

	untouchable = 0;
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);
	player_state->Update(dt);
	// Simple fall down
	vy += MARIO_GRAVITY * dt;


	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;


	coEvents.clear();



	// turn off collision when die 
	if (state != MARIO_STATE_DIE) {
		CalcPotentialCollisions(coObjects, coEvents);
	}


	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
		if (x <= 0) x = 0;
	}
	else
	{

		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<CInvisibleObject*>(e->obj)) {
				if (e->nx != 0) x += dx;
				if (e->ny > 0) y += dy;
				else vy = 0;
			}
			else {
				if (e->nx != 0) vx = 0;
				if (e->ny != 0)	vy = 0;
			}
		}
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
}

void CMario::Render()
{
	this->ani = player_state->GetAnimation();

	if (state == MARIO_STATE_DIE)
		ani = MARIO_ANI_DIE;

	int alpha = 255;
	if (untouchable) alpha = 128;

	int offset = 0;
	if (level == RACCOON_LEVEL_BIG)
		offset = 4;

	bool spinningFlag = false;
	if (ani == RACCOON_ANI_SPINNING_BIG)
		spinningFlag = true;

	animation_set->at(ani)->Render(x, y, alpha, nx, offset, spinningFlag);

	RenderBoundingBox();
}

void CMario::GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx = 0, int dy = 0)
{

	if (level == MARIO_LEVEL_BIG || level == FIRE_LEVEL || level == RACCOON_LEVEL_BIG)
	{

		left = x - MARIO_BIG_BBOX_WIDTH / 2;
		right = left + MARIO_BIG_BBOX_WIDTH;
		if (!is_crouching) {
			top = y - MARIO_BIG_BBOX_HEIGHT / 2;
			bottom = top + MARIO_BIG_BBOX_HEIGHT;
		}
		else {
			top = y - MARIO_CROUCH_BBOX_HEIGHT / 2;
			bottom = top + MARIO_CROUCH_BBOX_HEIGHT;
		}
	}
	else
	{
		left = x - MARIO_SMALL_BBOX_WIDTH / 2;
		top = y - MARIO_SMALL_BBOX_HEIGHT / 2;
		right = left + MARIO_SMALL_BBOX_WIDTH;
		bottom = top + MARIO_SMALL_BBOX_HEIGHT;

	}
}

CMario* CMario::GetInstance()
{
	if (__instance == NULL)
		__instance = new CMario();
	return __instance;
}

void CMario::SetStartPosition(float start_x, float start_y)
{
	this->start_x = start_x;
	this->start_y = start_y;
}

void CMario::SetPosition(float x, float y)
{
	this->x = x;
	this->y = y;
}

void CMario::OnKeyDown(int KeyCode)
{
	player_state->OnKeyDown(KeyCode);
}

void CMario::OnKeyUp(int KeyCode)
{
	player_state->OnKeyUp(KeyCode);
}

void CMario::KeyState(BYTE* states)
{
	player_state->KeyState(states);
}

void CMario::ChangeState(CPlayerState* newState)
{
	delete player_state;
	player_state = NULL;
	player_state = newState;

	if (((dynamic_cast<CSpinningState*>(player_state) ||
		dynamic_cast<CRunningState*>(player_state))) && level == RACCOON_LEVEL_BIG) {
		ani = player_state->GetAnimation();
		animation_set->at(ani)->ResetFlagLastFrame();
		player_state->SetAnimation(animation_set->at(ani));
	}
	else if (level == FIRE_LEVEL && dynamic_cast<CAttackingState*>(player_state)) {
		ani = player_state->GetAnimation();
		animation_set->at(ani)->ResetFlagLastFrame();
		player_state->SetAnimation(animation_set->at(ani));
	}
}

/*
	Reset Mario status to the beginning state of a scene
*/
void CMario::Reset()
{
	SetPosition(GetX(), 0);
	SetSpeed(0, 0);
	ChangeState(new CStandingState(this->level));
}

void CMario::RenderBoundingBox()
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


	CGame::GetInstance()->Draw(draw_x, draw_y, bbox, rect.left, rect.top, rect.right, rect.bottom, 50);
}


