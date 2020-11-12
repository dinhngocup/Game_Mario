﻿#include "Koopa.h"

CKoopa::CKoopa()
{
}

CKoopa::CKoopa(int state)
{
	SetState(state);
	nx = -1;
}

void CKoopa::GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx, int dy)
{
	left = x - KOOPA_BBOX_WIDTH / 2;
	right = left + KOOPA_BBOX_WIDTH;

	if (state == STATE_DIE || state == STATE_DIE_BY_WEAPON || state == STATE_SPIN || state == STATE_HOLD) {
		top = y - KOOPA_BBOX_HEIGHT_DIE / 2;
		bottom = top + KOOPA_BBOX_HEIGHT_DIE;
	}
	else {
		top = y - KOOPA_BBOX_HEIGHT / 2;
		bottom = top + KOOPA_BBOX_HEIGHT;
	}
}

void CKoopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	DebugOut(L"update koopa\n");

	CMario* mario = CMario::GetInstance();
	vy += KOOPA_GRAVITY * dt;
	if (state == STATE_HOLD) {
		if (!mario->is_holding) {
			SetState(STATE_UNHOLD);
			SetState(STATE_SPIN);
		}
		else {
			if (mario->is_skid ||  vx == 0) {
				x = mario->x + 30.0f * mario->nx;
				mario->is_skid = false;
			}
			else {
				vx = mario->vx;
			}
			vy = mario->vy;
		}
	}
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
					if (state == STATE_HOLD) {
						if (e->nx != 0) x += dx;
						if (e->ny != 0) vy = 0;
					}
					else if (state == STATE_WALKING_SWINGS) {
						if (e->ny != 0) {
							if (e->ny < 0) {
								vy = -KOOPA_JUMP_SPEED_Y;
								vx = this->nx * KOOPA_JUMP_SPEED_X;
							}
							else {
								vy = 0;
							}
						}
						if (e->nx != 0) {
							vx *= -1;
							this->nx *= -1;
						}
					}
					else {
						IsCollisionWithBrick(e);
					}
				}
				else if (dynamic_cast<CInvisibleObject*>(e->obj)) {
					if (state == STATE_WALKING_SWINGS) {
						if (e->ny != 0) {
							if (e->ny < 0) {
								DebugOut(L"nxxxx %d\n", this->nx);
								vy = -KOOPA_JUMP_SPEED_Y;
								vx = this->nx * KOOPA_JUMP_SPEED_X;
							}
							else {
								y += dy;
							}
						}
						if (e->nx != 0) x += dx;
					}
					else
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


	if (state == STATE_SPIN || state == STATE_DIE_BY_WEAPON) {
		if (y > game->GetCamY() + game->GetScreenHeight() ||
			x > game->GetCamX() + game->GetScreenWidth() ||
			x < game->GetCamX())
			SetHealth(false);
	}
}

void CKoopa::Render()
{
	int ani = 0;
	int ny = 1;
	if (state == STATE_DIE || state == STATE_HOLD)
		ani = KOOPA_ANI_DIE;
	else if (state == STATE_WALKING)
		ani = KOOPA_ANI_WALKING;
	else if (state == STATE_WALKING_SWINGS)
		ani = KOOPA_ANI_WALKING_SWINGS;
	else if (state == STATE_SPIN)
		ani = KOOPA_ANI_SPIN;
	else if (state == STATE_DIE_BY_WEAPON) {
		ani = KOOPA_ANI_DIE;
		ny = -1;
	}
	if (vx >= 0)
		animation_set->at(ani)->Render(x, y, 255, -1, 0, 0, ny);
	else
		animation_set->at(ani)->Render(x, y, 255, 1, 0, 0, ny);

	RenderBoundingBox();
}

void CKoopa::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case STATE_WALKING:
		vx = 0;
		vx = nx * KOOPA_WALKING_SPEED;
		break;
	case STATE_DIE:
		//y += KOOPA_DISPARITIES;
		vx = 0;
		vy = 0;
		break;
	case STATE_DIE_BY_WEAPON:
		y += KOOPA_DISPARITIES;
		vx = 0.2f * nx;
		vy = -0.5f;
		ableToCheckCollision = false;
		break;
	case STATE_WALKING_SWINGS:
		vx = nx * -KOOPA_WALKING_SPEED;
		vx = 0;
		break;

	case STATE_SPIN:
		vx = nx * KOOPA_SPIN_SPEED;
		vy = 0;
		break;
	case STATE_HOLD:
		y -= 10;
		x -= 10 * nx;
		break;
	case STATE_UNHOLD:
		y += 10;
		x += 10 * nx;
		break;
	}

}

void CKoopa::IsCollisionWithMario(LPCOLLISIONEVENT e)
{

	CMario* mario = CMario::GetInstance();
	// nhảy lên đầu koopa
	if (e->ny < 0)
	{
		if (state != STATE_DIE)
		{
			// rùa có cánh
			if (state == STATE_WALKING_SWINGS) {
				SetState(STATE_WALKING);
			}
			// rùa ko cánh, rùa spin
			else
				SetState(STATE_DIE);
			mario->vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
		else {
			mario->vy = 0;
		}
	}
	// đụng bên hông koopa
	else if (e->nx != 0)
	{
		//block vx trước
		//mario->vx = 0;

		// đụng ngang koopas còn đang sống
		if (state != STATE_DIE)
		{
			mario->vx = 0;
			if (mario->is_attacking_by_spinning) {
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
		else {
			// mai rùa đang đứng yên
			if (vx == 0) {
				// đẩy vỏ rùa từ phải sang
				if (e->nx > 0) {
					nx = -1;
					// set trạng thái mario kick or hold
					if (dynamic_cast<CRunningState*>(mario->player_state)) {
						SetState(STATE_HOLD);
						mario->is_holding = true;
						mario->ChangeState(new CHoldingState(mario->GetLevel()));
					}
					else {
						mario->vx = 0;
						mario->ChangeState(new CKickingState(mario->GetLevel()));
						SetState(STATE_SPIN);
					}
				}
				else {
					nx = 1;
					// set trạng thái mario kick
					if (dynamic_cast<CRunningState*>(mario->player_state)) {
						SetState(STATE_HOLD);
						mario->is_holding = true;
						mario->ChangeState(new CHoldingState(mario->GetLevel()));
					}
					else {
						mario->vx = 0;
						mario->ChangeState(new CKickingState(mario->GetLevel()));
						SetState(STATE_SPIN);
					}
				}
			}
			// mai rùa di chuyển
			else {
				mario->vx = 0;
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
					//if (level > MARIO_LEVEL_SMALL)
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
}

void CKoopa::IsCollisionWithEnemy(LPCOLLISIONEVENT e)
{
	
	/*DebugOut(L"state %d\n", state);
	DebugOut(L"e state %d\n", e->obj->state);
	DebugOut(L"e ableToCheckCollision %d\n", e->obj->ableToCheckCollision);*/
	if (e->obj->type == eTYPE::GOOMBA) {
		CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

		if (e->ny != 0) {
			if (state == STATE_SPIN) {
				goomba->AttackedByShell();
				x += dx;
			}
			else if (state == STATE_DIE) {
				if (e->ny < 0) {
					goomba->SetState(STATE_DIE);
				}
				else {
					goomba->vy = -GOOMBA_JUMP_SPEED_Y;
					AttackedByShell();
				}
			}
			else if (state == STATE_WALKING_SWINGS ||state == STATE_WALKING) {
				if (e->ny < 0) {
					goomba->SetState(STATE_DIE);
				}
				else {
					SetState(STATE_DIE);
					goomba->vy = -GOOMBA_JUMP_SPEED_Y;
				}
			}
		}

		if (e->nx != 0) {
			if (state == STATE_SPIN) {
				goomba->AttackedByShell();
				x += dx;
			}
			else if (state == STATE_DIE) {
				goomba->vx *= -1;
			}
			else if (state == STATE_WALKING_SWINGS || state == STATE_WALKING) {
				if (nx * e->nx > 0) {
					vx *= -1;
					nx *= -1;
					goomba->vx *= -1;
					goomba->nx *= -1;
				}
				else {
					vx *= -1;
					nx *= -1;
				}
			}
		}





		if (state == STATE_SPIN || state == STATE_HOLD) {
			CEnemy* enemy = dynamic_cast<CEnemy*>(e->obj);
			enemy->AttackedByShell();
			x += dx;
		}
		else if (state == STATE_WALKING_SWINGS) {
			vy = -KOOPA_JUMP_SPEED_Y;
			vx = nx * KOOPA_JUMP_SPEED_X;
			goomba->y -= dy;
		}
		else {
			if (e->nx != 0 || e->ny != 0) {
				if (nx * e->nx > 0) {
					vx *= -1;
					nx *= -1;
					goomba->vx *= -1;
					goomba->nx *= -1;
				}
				else {
					goomba->vx *= -1;
					goomba->nx *= -1;
				}
			}
		}
	}
	else if (e->obj->type == eTYPE::KOOPA) {
		CKoopa* koopa = dynamic_cast<CKoopa*>(e->obj);
		if (state == STATE_SPIN || state == STATE_HOLD) {
			CEnemy* enemy = dynamic_cast<CEnemy*>(e->obj);
			enemy->AttackedByShell();
			x += dx;
		}
		else {
			// những state còn lại đụng phải spin hoặc hold
			if (koopa->state == STATE_SPIN || koopa->state == STATE_HOLD) {
				AttackedByShell();
				koopa->x += dx;
			}
			else {
				if (e->nx != 0) {
					vx *= -1;
					koopa->vx *= -1;
				}
			}
		}
	}
}

void CKoopa::AttackedByShell()
{
	SetState(STATE_DIE_BY_WEAPON);
}
