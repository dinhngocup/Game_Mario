#include "Koopa.h"
#include "PlayScene.h"

CKoopa::CKoopa()
{
}

CKoopa::CKoopa(int state)
{
	nx = -1;
	generate_id++;
	this->id = generate_id;
	start_state = state;
	SetState(state);
}

void CKoopa::GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx, int dy)
{
	left = x;
	right = left + KOOPA_BBOX_WIDTH;

	if (state == KOOPA_STATE_DIE || state == KOOPA_STATE_DIE_BY_WEAPON ||
		state == KOOPA_STATE_SPIN || state == KOOPA_STATE_HOLD ||
		state == KOOPA_STATE_DIE_BY_TAIL) {
		top = y;
		bottom = top + KOOPA_BBOX_HEIGHT_DIE;
	}
	else {
		top = y;
		bottom = top + KOOPA_BBOX_HEIGHT;
	}
}

void CKoopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (revival) {
		if (GetTickCount64() - start_revival >= REVIVAL_TIME){
			CMario* mario = CMario::GetInstance();
			y -= 30;
			SetState(KOOPA_STATE_WALKING);
			nx = mario->nx;
			revival = false;
		}

	}
	CMario* mario = CMario::GetInstance();
	vy += KOOPA_GRAVITY * dt;
	if (state == KOOPA_STATE_HOLD) {
		if (!mario->is_holding) {
			SetState(KOOPA_STATE_UNHOLD);
			SetState(KOOPA_STATE_SPIN);
		}
		else {
			if (mario->is_skid || vx == 0) {
				x = mario->x + 27.0f * mario->nx;
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
	CPlayScene* scene = (CPlayScene*)game->GetCurrentScene();
	vector<LPGAMEOBJECT> enemies = scene->enemies;
	vector<LPGAMEOBJECT> bricks = scene->ghost_platforms;
	vector<LPGAMEOBJECT> items = scene->items;



	coEvents.clear();

	if (state != KOOPA_STATE_DIE_BY_WEAPON) {
		CalcPotentialCollisions(&bricks, coEvents);
		CalcPotentialCollisions(&enemies, coEvents);
		CalcPotentialCollisions(&items, coEvents);
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
			if (state != KOOPA_STATE_DIE_BY_WEAPON) {
				if (dynamic_cast<CBrick*>(e->obj)) {
					if (state == KOOPA_STATE_HOLD || state == KOOPA_STATE_WALKING_SWINGS) {
						IsCollisionWithBrickSpecially(e);
					}
					else
						IsCollisionWithBrick(e);
				}
				else if (dynamic_cast<CInvisibleObject*>(e->obj)) {
					if (state == KOOPA_STATE_WALKING_SWINGS)
						IsCollisionWithGhostPlatformSpecially(e);
					else
						IsCollisionWithGhostPlatform(e);
				}
				else if (dynamic_cast<CEnemy*>(e->obj)) {
					IsCollisionWithEnemy(e);
				}
				else if (dynamic_cast<CBrickQuestion*>(e->obj)) {
					CBrickQuestion* brick = dynamic_cast<CBrickQuestion*>(e->obj);
					if (state == KOOPA_STATE_SPIN) {
						if (e->nx != 0) {
							brick->SetState(STATE_EMPTY);
							vx *= -1;
						}
						if (e->ny != 0) vy = 0;
					}
					else
						IsCollisionWithBrick(e);
				}

			}
		}

		// clean up collision events
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}


	if (state == KOOPA_STATE_DIE_BY_WEAPON) {
		if (y > game->GetCamY() + game->GetScreenHeight() ||
			x > game->GetCamX() + game->GetScreenWidth() ||
			x < game->GetCamX())
			SetHealth(false);
	}
}

void CKoopa::Render()
{
	int ani = 0;
	if (state == KOOPA_STATE_DIE || state == KOOPA_STATE_HOLD)
		ani = KOOPA_ANI_DIE;
	else if (state == KOOPA_STATE_WALKING)
		ani = KOOPA_ANI_WALKING;
	else if (state == KOOPA_STATE_WALKING_SWINGS)
		ani = KOOPA_ANI_WALKING_SWINGS;
	else if (state == KOOPA_STATE_SPIN)
		ani = KOOPA_ANI_SPIN;
	else if (state == KOOPA_STATE_DIE_BY_WEAPON || state == KOOPA_STATE_DIE_BY_TAIL) {
		ani = KOOPA_ANI_DIE;
	}
	if (vx >= 0)
		animation_set->at(ani)->Render(x, y, 255, -1, 0, 0, koopa_ny);
	else
		animation_set->at(ani)->Render(x, y, 255, 1, 0, 0, koopa_ny);

	RenderBoundingBox();
}

void CKoopa::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPA_STATE_WALKING:
		vx = 0;
		vx = nx * KOOPA_WALKING_SPEED;
		koopa_ny = 1;
		break;
	case KOOPA_STATE_DIE:
		start_revival = GetTickCount64();
		//y += 28;
		revival = true;
		vx = 0;
		vy = 0;
		koopa_ny = 1;
		break;
	case KOOPA_STATE_DIE_BY_WEAPON:
		y -= KOOPA_DISPARITIES;
		vx = 0.2f * nx;
		koopa_ny = -1;
		vy = -0.5f;
		ableToCheckCollision = false;
		break;
	case KOOPA_STATE_DIE_BY_TAIL:
		start_revival = GetTickCount64();
		revival = true;
		//y += KOOPA_DISPARITIES;
		vx = 0.4f * nx;
		vy = -0.5f;
		koopa_ny = -1;
		break;
	case KOOPA_STATE_WALKING_SWINGS:
		vx = nx * -KOOPA_WALKING_SPEED;
		vx = 0;
		koopa_ny = 1;
		break;

	case KOOPA_STATE_SPIN:
		vx = nx * KOOPA_SPIN_SPEED;
		revival = false;
		vy = 0;
		break;
	case KOOPA_STATE_HOLD:
		y -= 10;
		x -= 10 * nx;
		break;
	case KOOPA_STATE_UNHOLD:
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
		if (state != KOOPA_STATE_DIE)
		{
			// rùa có cánh
			if (state == KOOPA_STATE_WALKING_SWINGS) {
				SetState(KOOPA_STATE_WALKING);
			}
			// rùa ko cánh, rùa spin
			else {
				if (koopa_ny == 1)
					SetState(KOOPA_STATE_DIE);
				else {
					SetState(KOOPA_STATE_DIE);
					koopa_ny = -1;
				}
			}
				mario->vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
		else {
			mario->vy = -MARIO_JUMP_DEFLECT_SPEED;
			if (mario->x <= x) {
				nx = 1;
			}
			else {
				nx = -1;
			}
			SetState(KOOPA_STATE_SPIN);
		}
	}
	// đụng bên hông koopa
	else if (e->nx != 0)
	{
		// đụng ngang koopas còn đang sống
		if (state != KOOPA_STATE_DIE && state != KOOPA_STATE_DIE_BY_TAIL)
		{
			if (mario->is_attacking_by_spinning) {
				mario->vx = 0;
				nx = e->nx * -1;

				CStarEffect* effect;
				if (nx > 0) {
					effect = new CStarEffect(x + w, y + h / 2);
				}
				else {
					effect = new CStarEffect(x, y + h / 2);
				}
				effect->start_ani = GetTickCount64();
				CGame* game = CGame::GetInstance();
				CPlayScene* scene = (CPlayScene*)game->GetCurrentScene();
				scene->effects.push_back(effect);

				SetState(KOOPA_STATE_DIE_BY_TAIL);
			}
			else {
				if (mario->untouchable == 0) {
					mario->vx = 0;
					if (mario->GetLevel() > MARIO_LEVEL_SMALL)
					{
						mario->StartUntouchable();
					}
					// mario chết
					else
						mario->SetState(MARIO_STATE_DIE);

				}
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
						SetState(KOOPA_STATE_HOLD);
						mario->is_holding = true;
						mario->ChangeState(new CHoldingState(mario->GetLevel()));
					}
					else {
						mario->vx = 0;
						mario->ChangeState(new CKickingState(mario->GetLevel()));
						SetState(KOOPA_STATE_SPIN);
					}
				}
				else {
					nx = 1;
					// set trạng thái mario kick
					if (dynamic_cast<CRunningState*>(mario->player_state)) {
						SetState(KOOPA_STATE_HOLD);
						mario->is_holding = true;
						mario->ChangeState(new CHoldingState(mario->GetLevel()));
					}
					else {
						mario->vx = 0;
						mario->ChangeState(new CKickingState(mario->GetLevel()));
						SetState(KOOPA_STATE_SPIN);
					}
				}
			}
			// mai rùa di chuyển
			else {
				if (mario->is_attacking_by_spinning) {
					mario->vx = 0;
					// đụng bên phải
					if (e->nx > 0) {
						e->obj->nx = -1;
					}
					else
						e->obj->nx = 1;
					SetState(KOOPA_STATE_DIE_BY_WEAPON);
				}
				else {
					if (mario->untouchable == 0) {
						mario->vx = 0;
						if (mario->GetLevel() > MARIO_LEVEL_SMALL)
						{
							mario->StartUntouchable();
						}
						// mario chết
						else
							mario->SetState(MARIO_STATE_DIE);

					}
				}
			}
		}
	}
}

void CKoopa::HandleCollisionWithMario(LPCOLLISIONEVENT e)
{
}

void CKoopa::IsCollisionWithEnemy(LPCOLLISIONEVENT e)
{
	if (e->obj->type == eTYPE::GOOMBA) {
		CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

		if (e->ny != 0) {
			if (state == KOOPA_STATE_SPIN) {
				goomba->AttackedByShell();
				x += dx;
			}
			else if (state == KOOPA_STATE_DIE) {
				if (e->ny < 0) {
					goomba->SetState(GOOMBA_STATE_DIE);
				}
				else {
					goomba->vy = -GOOMBA_JUMP_SPEED_Y;
					AttackedByShell();
				}
			}
			else if (state == KOOPA_STATE_WALKING_SWINGS || state == KOOPA_STATE_WALKING) {
				if (e->ny < 0) {
					goomba->SetState(GOOMBA_STATE_DIE);
				}
				else {
					SetState(KOOPA_STATE_DIE);
					goomba->vy = -GOOMBA_JUMP_SPEED_Y;
				}
			}
		}

		if (e->nx != 0) {
			if (state == KOOPA_STATE_SPIN) {
				goomba->AttackedByShell();
				x += dx;
			}
			else if (state == KOOPA_STATE_DIE) {
				goomba->vx *= -1;
			}
			else if (state == KOOPA_STATE_WALKING_SWINGS || state == KOOPA_STATE_WALKING) {
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





		if (state == KOOPA_STATE_SPIN || state == KOOPA_STATE_HOLD) {
			CEnemy* enemy = dynamic_cast<CEnemy*>(e->obj);
			enemy->AttackedByShell();
			x += dx;
		}
		else if (state == KOOPA_STATE_WALKING_SWINGS) {
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
		if (state == KOOPA_STATE_SPIN || state == KOOPA_STATE_HOLD) {
			CEnemy* enemy = dynamic_cast<CEnemy*>(e->obj);
			enemy->AttackedByShell();
			x += dx;
		}
		else {
			// những state còn lại đụng phải spin hoặc hold
			if (koopa->state == KOOPA_STATE_SPIN || koopa->state == KOOPA_STATE_HOLD) {
				AttackedByShell();
				koopa->x += dx;
			}
			else {
				if (e->nx != 0) {
					vx *= -1;
					koopa->vx *= -1;
				} 
				if (e->ny != 0) {
					y += dy;
				}
			}
		}
	}
}

void CKoopa::IsCollisionWithBrickSpecially(LPCOLLISIONEVENT e)
{
	if (state == KOOPA_STATE_HOLD) {
		if (e->nx != 0) x += dx;
		if (e->ny != 0) y += dy;
	}
	else if (state == KOOPA_STATE_WALKING_SWINGS) {
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

}

void CKoopa::IsCollisionWithGhostPlatformSpecially(LPCOLLISIONEVENT e)
{
	if (e->ny != 0) {
		if (e->ny < 0) {
			vy = -KOOPA_JUMP_SPEED_Y;
			vx = this->nx * KOOPA_JUMP_SPEED_X;
		}
		else {
			y += dy;
		}
	}
	if (e->nx != 0) x += dx;
}

void CKoopa::IsCollisionWithBrick(LPCOLLISIONEVENT e)
{
	if (e->ny != 0) {
		vy = 0;
		if (state == KOOPA_STATE_DIE_BY_TAIL)
			vx = 0;
	}
	// đụng hộp ngang quay đầu hoặc đi hết chiều dài của viên gạch
	if (state != KOOPA_STATE_SPIN) {
		if (e->nx != 0 || x + w >= e->obj->x + e->obj->w || x <= e->obj->x) {
			vx *= -1;
			nx *= -1;
		}
	}
	else {
		if (e->nx != 0) {
			vx *= -1;
			nx *= -1;
		}
	}
}

void CKoopa::IsCollisionWithGhostPlatform(LPCOLLISIONEVENT e)
{
	if (e->nx != 0) x += dx;
	if (e->ny < 0) {
		vy = 0;
		if (state == KOOPA_STATE_DIE_BY_TAIL)
			vx = 0;
	}
	else if (e->ny > 0)
		y += dy;
	if (state == KOOPA_STATE_WALKING)
		if (x + w >= e->obj->x + e->obj->w || x <= e->obj->x) {
			vx *= -1;
			nx *= -1;
		}
}

void CKoopa::AttackedByShell()
{
	SetState(KOOPA_STATE_DIE_BY_WEAPON);
}
