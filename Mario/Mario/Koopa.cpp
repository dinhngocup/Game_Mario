#include "Koopa.h"

CKoopa::CKoopa()
{
}

CKoopa::CKoopa(int state)
{
	SetState(state);
	//SetState(STATE_SPIN);
}

void CKoopa::GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx, int dy)
{
	left = x - KOOPA_BBOX_WIDTH / 2;
	right = left + KOOPA_BBOX_WIDTH;

	if (state == STATE_DIE || state == STATE_DIE_BY_WEAPON || state == STATE_SPIN) {
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
	vy += KOOPA_GRAVITY * dt;

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

			if (dynamic_cast<CBrick*>(e->obj)) {
				IsCollisionWithBrick(e);
			}
			else if (dynamic_cast<CInvisibleObject*>(e->obj)) {
				IsCollisionWithGhostPlatform(e);
			}
			else if (dynamic_cast<CEnemy*>(e->obj)) {
				DebugOut(L"rua\n");
				IsCollisionWithEnemy(e);

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
	if (state == STATE_DIE)
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
		vx = KOOPA_WALKING_SPEED;
		break;
	case STATE_DIE:
		y += KOOPA_DISPARITIES;
		vx = 0;
		vy = 0;
		break;
	case STATE_DIE_BY_WEAPON:
		y += KOOPA_DISPARITIES;
		vx = 0.2f * nx;
		vy = -0.5f;
		break;
	case STATE_WALKING_SWINGS:
		vx = -KOOPA_WALKING_SPEED;
		break;

	case STATE_SPIN:
		//y -= 10;
		vx = nx * KOOPA_SPIN_SPEED;
		vy = 0;
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
		mario->vx = 0;

		// đụng ngang koopas còn đang sống
		if (state != STATE_DIE)
		{
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
					// set trạng thái mario kick
					SetState(STATE_SPIN);
				}
				else {
					nx = 1;
					SetState(STATE_SPIN);
				}
			}
			// mai rùa di chuyển
			else {
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
	if (state == STATE_SPIN) {
		CEnemy* enemy = dynamic_cast<CEnemy*>(e->obj);
		if (enemy->state != STATE_DIE_BY_WEAPON) {
			DebugOut(L"hi\n");
			enemy->AttackedByShell();
		}
		x += dx;
	}
	else if(state == STATE_DIE){
		e->obj->vx *= -1;
	}
	else {
		if (e->nx != 0) {
			vx *= -1;
			e->obj->vx *= -1;
		}
	}
}

