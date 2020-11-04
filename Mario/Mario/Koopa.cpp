#include "Koopa.h"

CKoopa::CKoopa()
{
}

CKoopa::CKoopa(int state)
{
	SetState(state);
}

void CKoopa::GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx, int dy)
{
	left = x - KOOPA_BBOX_WIDTH / 2;
	right = left + KOOPA_BBOX_WIDTH;

	if (state == KOOPA_STATE_DIE || state == KOOPA_STATE_DIE_BY_WEAPON) {
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

	if (state != KOOPA_STATE_DIE_BY_WEAPON) {
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
				if (state == KOOPA_STATE_SPIN) {
					KillOtherEnemy(e);
				}
				else {
					IsCollisionWithEnemy(e);
				}
			}
		}
		
		// clean up collision events
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
	
	// còn thiếu đk nó đã chết
	/*if (GetY() > game->GetCamY() + game->GetScreenHeight()) {
		SetHealth(false);
	}*/
}

void CKoopa::Render()
{
	int ani = 0;
	int ny = 1;
	if (state == KOOPA_STATE_DIE)
		ani = KOOPA_ANI_DIE;
	else if (state == KOOPA_STATE_WALKING)
		ani = KOOPA_ANI_WALKING;
	else if (state == KOOPA_STATE_WALKING_SWINGS)
		ani = KOOPA_ANI_WALKING_SWINGS;
	else if (state == KOOPA_STATE_DIE_BY_WEAPON) {
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
	case KOOPA_STATE_WALKING:
		vx = 0;
		vx = KOOPA_WALKING_SPEED;
		break;
	case KOOPA_STATE_DIE:
		y += KOOPA_DISPARITIES;
		vx = 0;
		vy = 0;
		break;
	case KOOPA_STATE_DIE_BY_WEAPON:
		y += KOOPA_DISPARITIES;
		vx = 0.2f;
		vy = -0.5f;
		break;
	case KOOPA_STATE_WALKING_SWINGS:
		vx = -KOOPA_WALKING_SPEED;
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
			SetState(KOOPA_STATE_DIE);
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
		if (state != KOOPA_STATE_DIE)
		{
			if (mario->is_attacking_by_spinning) {
				SetState(KOOPA_STATE_DIE_BY_WEAPON);
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
				if (e->nx == 1) {
					SetSpeed(-0.5f, 0);
				}
				else {
					SetSpeed(0.5f, 0);
				}
			}
			// mai rùa di chuyển
			else {
				if (mario->is_attacking_by_spinning)
					SetState(KOOPA_STATE_DIE_BY_WEAPON);
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

void CKoopa::KillOtherEnemy(LPCOLLISIONEVENT e)
{
	switch (e->obj->type) {
	case eTYPE::GOOMBA:
		e->obj->SetState(GOOMBA_STATE_DIE_BY_WEAPON);
	}
}
