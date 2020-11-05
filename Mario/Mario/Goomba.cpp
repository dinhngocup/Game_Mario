#include "Goomba.h"

CGoomba::CGoomba(int state)
{
	SetState(state);
}

void CGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx, int dy)
{
	left = x - GOOMBA_BBOX_WIDTH / 2;
	top = y - GOOMBA_BBOX_HEIGHT / 2;
	right = left + GOOMBA_BBOX_WIDTH;
	bottom = top + GOOMBA_BBOX_HEIGHT;
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += GOOMBA_GRAVITY * dt;

	CGameObject::Update(dt);
	
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	CGame* game = CGame::GetInstance();
	vector<LPGAMEOBJECT>* enemies = game->GetCurrentScene()->GetEnemiesInScene();
	vector<LPGAMEOBJECT>* bricks = game->GetCurrentScene()->GetGhostPlatformsInScene();

	coEvents.clear();

	if (state != STATE_DIE_BY_WEAPON) {
		CalcPotentialCollisions(bricks, coEvents);
		CalcPotentialCollisions(enemies, coEvents);

	}

	if (coEvents.size() == 0)
	{
		
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// block every object first!
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;


		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (state != STATE_DIE_BY_WEAPON) {
				if (dynamic_cast<CBrick*>(e->obj)) {
					IsCollisionWithBrick(e);
				}
				else if (dynamic_cast<CInvisibleObject*>(e->obj)) {
					IsCollisionWithGhostPlatform(e);
				}
				else if (dynamic_cast<CEnemy*>(e->obj)) {
					IsCollisionWithEnemy(e);
				}

			}
		}

		// clean up collision events
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}

	if (state == STATE_DIE_BY_WEAPON || state == STATE_DIE) {
		if (y > game->GetCamY() + game->GetScreenHeight() ||
			x > game->GetCamX() + game->GetScreenWidth() ||
			x < game->GetCamX())
			SetHealth(false);
	}
}

void CGoomba::Render()
{
	int ani = 0;
	int ny = 1;
	if (state == STATE_DIE)
		ani = GOOMBA_ANI_DIE;
	else if (state == STATE_WALKING)
		ani = GOOMBA_ANI_WALKING;
	else if (state == STATE_WALKING_SWINGS)
		ani = GOOMBA_ANI_WALKING_SWINGS;
	else if (state == STATE_DIE_BY_WEAPON) {
		ani = GOOMBA_ANI_DIE_BY_WEAPON;
		ny = -1;
	}
	if (vx >= 0)
		animation_set->at(ani)->Render(x, y, 255, -1, 0, 0, ny);
	else
		animation_set->at(ani)->Render(x, y, 255, 1, 0, 0, ny);

	RenderBoundingBox();
}



void CGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case STATE_WALKING:
		vx = 0;
		vx = -GOOMBA_WALKING_SPEED;
		break;
	case STATE_DIE:
		y += GOOMBA_DISPARITIES;
		vx = 0;
		vy = 0;
		break;
	case STATE_DIE_BY_WEAPON:
		y += GOOMBA_DISPARITIES;
		vx = 0.4f * nx;
		vy = -0.5f;
		break;
	case STATE_WALKING_SWINGS:
		vx = -GOOMBA_WALKING_SPEED;
		break;
	}
}

void CGoomba::IsCollisionWithMario(LPCOLLISIONEVENT e)
{

	CMario* mario = CMario::GetInstance();
	// nhảy lên đầu nấm
	if (e->ny < 0)
	{
		if (state != STATE_DIE)
		{
			SetState(STATE_DIE);
			mario->vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
		else {
			mario->vy = 0;
		}
	}
	// đụng bên hông nấm
	else if (e->nx != 0)
	{
		//block vx trước
		mario->vx = 0;

		// đụng ngang nấm còn đang sống
		if (state != STATE_DIE)
		{
			if (mario->is_attacking_by_spinning) {
				// đụng bên phải
				if (e->nx > 0) {
					e->obj->nx = -1;
				}
				else
					e->obj->nx = 1;
				SetState(STATE_DIE_BY_WEAPON);
			}
			else {
				DebugOut(L"xu ly mario giam level hay chet\n");
				//if (mario->level > MARIO_LEVEL_SMALL)
				//{
				//	level = MARIO_LEVEL_SMALL;
				//	player_state->SetLevel(level);
				//	StartUntouchable();
				//}
				//// mario chết
				//else
				//	SetState(MARIO_STATE_DIE);
			}
		}
	}
}

void CGoomba::AttackedByShell()
{
	SetState(STATE_DIE_BY_WEAPON);
}

void CGoomba::IsCollisionWithEnemy(LPCOLLISIONEVENT e)
{
	// đụng trúng rùa đang spin
	if (e->obj->state == STATE_SPIN) {
		if (state != STATE_DIE_BY_WEAPON)
			AttackedByShell();
		e->obj->x += dx;
	}
	else if (e->obj->state == STATE_DIE) {
		vx *= -1;
	}
	else {
		if (e->nx != 0) {
			vx *= -1;
			e->obj->vx *= -1;
		}
	}
}
