#include "Goomba.h"
#include "PlayScene.h"

CGoomba::CGoomba(int state)
{
	generate_id++;
	this->id = generate_id;
	start_state = state;
	nx = -1;
	SetState(state);
}

void CGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx, int dy)
{
	left = x;
	top = y;

	right = left + w;
	bottom = top + h;
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CMario* mario = CMario::GetInstance();
	/*if (x < mario->x)
		smart_goomba = true;
	if (smart_goomba) {
		boundary_left = mario->x - 250;
		boundary_right = mario->x + mario->w + 250;

		DebugOut(L"l %f\n", boundary_left);
		DebugOut(L"mario x %f\n", mario->x);
		DebugOut(L"x %f\n", x);
		DebugOut(L"w %d\n", w);
		DebugOut(L"r %f\n", boundary_right);


		if (x <= boundary_right && x >= boundary_left) {
			if (x + w >= boundary_right || x - w <= boundary_left) {
				if (!changed)
					change = true;
			}
		}
	}*/

	if (start_walking == 0) {
		start_walking = GetTickCount64();
	}
	if (state == GOOMBA_STATE_WALKING_SWINGS_CLOSE) {
		/*if (change) {
			DebugOut(L"hi\n");
			vx *= -1;
			nx *= -1;
			change = false;
			changed = true;
		}*/
		if (GetTickCount64() - start_walking >= GOOMBA_TIME_WALKING) {
			SetState(GOOMBA_STATE_JUMPING_SWINGS_CLOSE);
		}
	}
	else if (state == GOOMBA_STATE_JUMPING_SWINGS_CLOSE) {
		if (vy > 0) {
			index++;
			SetState(GOOMBA_STATE_WALKING_SWINGS_OPEN);
		}
	}

	vy += GOOMBA_GRAVITY * dt;
	CGameObject::Update(dt);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	CGame* game = CGame::GetInstance();
	CPlayScene* scene = (CPlayScene*)game->GetCurrentScene();
	vector<LPGAMEOBJECT> enemies = scene->enemies;
	vector<LPGAMEOBJECT> bricks = scene->ghost_platforms;
	vector<LPGAMEOBJECT> items = scene->items;

	coEvents.clear();

	if (state != GOOMBA_STATE_DIE_BY_WEAPON) {
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

		//DebugOut(L"size %d\n", coEventsResult.size());
		//DebugOut(L"size in gooomba %d\n", coEventsResult.size());
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			// nấm vc rùa và viên gạch, sau khi vc với rùa
			// biến thành die by weapon, xong nó xét vc tiếp với gạch
			// làm reset vy của nấm => khi thành die by weapon thì mấy e tiếp theo pass luôn
			if (state != GOOMBA_STATE_DIE_BY_WEAPON) {
				if (dynamic_cast<CBrickQuestion*>(e->obj)) {
					IsCollisionWithBrick(e);
				}
				else if (dynamic_cast<CInvisibleObject*>(e->obj)) {
					if (state == GOOMBA_STATE_WALKING_SWINGS_OPEN || state == GOOMBA_STATE_WALKING_SWINGS)
						IsCollisionWithGhostPlatformSpecially(e);
					else
						IsCollisionWithGhostPlatform(e);
				}
				else if (dynamic_cast<CEnemy*>(e->obj)) {
					IsCollisionWithEnemy(e);
				}
				else if (dynamic_cast<CBrick*>(e->obj)) {
					if (state == GOOMBA_STATE_WALKING_SWINGS_OPEN || state == GOOMBA_STATE_WALKING_SWINGS) {
						IsCollisionWithBrickSpecially(e);
					}
					else
						IsCollisionWithBrick(e);
				}
			}
		}

		// clean up collision events
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}

	if (state == GOOMBA_STATE_DIE_BY_WEAPON || state == GOOMBA_STATE_DIE) {
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
	if (state == GOOMBA_STATE_DIE)
		ani = GOOMBA_ANI_DIE;
	else if (state == GOOMBA_STATE_WALKING)
		ani = GOOMBA_ANI_WALKING;
	else if (state == GOOMBA_STATE_WALKING_SWINGS)
		ani = GOOMBA_ANI_WALKING_SWINGS;
	else if (state == GOOMBA_STATE_WALKING_SWINGS_OPEN)
		ani = GOOMBA_ANI_WALKING_SWINGS_OPEN;
	else if (state == GOOMBA_STATE_WALKING_SWINGS_CLOSE || state == GOOMBA_STATE_JUMPING_SWINGS_CLOSE)
		ani = GOOMBA_ANI_WALKING_SWINGS_CLOSE;
	else if (state == GOOMBA_STATE_DIE_BY_WEAPON) {
		ani = GOOMBA_ANI_DIE_BY_WEAPON;
		ny = -1;
	}
	if (vx >= 0)
		animation_set->at(ani)->Render(x, y, 255, -1, 0, 0, ny);
	else
		animation_set->at(ani)->Render(x, y, 255, 1, 0, 0, ny);

	//RenderBoundingBox();
}

void CGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case GOOMBA_STATE_WALKING:
		vx = 0;
		vx = -GOOMBA_WALKING_SPEED;
		break;
	case GOOMBA_STATE_DIE:
		y += GOOMBA_DISPARITIES;
		vx = 0;
		vy = 0;
		break;
	case GOOMBA_STATE_DIE_BY_WEAPON:
		y += GOOMBA_DISPARITIES;
		vx = GOOMBA_SPEED_X_DIE_BY_WEAPON * nx;
		vy = -GOOMBA_SPEED_Y_DIE_BY_WEAPON;
		ableToCheckCollision = false;
		break;
	case GOOMBA_STATE_WALKING_SWINGS:
		vy = -GOOMBA_JUMP_SPEED_Y;
		vx = this->nx * GOOMBA_JUMP_SPEED_X;
		if (changed)
			changed = false;
		break;
	case GOOMBA_STATE_WALKING_SWINGS_CLOSE:
		vx = nx * GOOMBA_WALKING_SPEED;
		break;
	case GOOMBA_STATE_JUMPING_SWINGS_CLOSE:
		vy = -GOOMBA_JUMP_SPEED_Y_LOW;
		vx = this->nx * GOOMBA_JUMP_SPEED_X_LOW;
		break;
	}
	UpdateWidthHeightGoomba();
}

void CGoomba::IsCollisionWithMario(LPCOLLISIONEVENT e)
{

	CMario* mario = CMario::GetInstance();
	// nhảy lên đầu nấm
	if (e->ny < 0)
	{
		if (state != GOOMBA_STATE_DIE)
		{
			if (state == GOOMBA_STATE_WALKING_SWINGS) {
				SetState(GOOMBA_STATE_WALKING);
			}
			else
				SetState(GOOMBA_STATE_DIE);
			mario->vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
		else {
			mario->vy = 0;
		}
	}
	// đụng bên hông nấm
	else if (e->nx != 0)
	{
		// đụng ngang nấm còn đang sống
		if (state != GOOMBA_STATE_DIE)
		{
			if (mario->is_attacking_by_spinning) {
				mario->vx = 0;
				// đụng bên phải
				UpdateWidthHeightGoomba();
				CStarEffect* effect;
				if (e->nx > 0) {
					e->obj->nx = -1;
					effect = new CStarEffect(x + w, y + h / 2);
				}
				else {
					effect = new CStarEffect(x, y + h / 2);
					e->obj->nx = 1;
				}
				effect->start_ani = GetTickCount64();
				CGame* game = CGame::GetInstance();
				CPlayScene* scene = (CPlayScene*)game->GetCurrentScene();
				scene->effects.push_back(effect);
				SetState(GOOMBA_STATE_DIE_BY_WEAPON);
			}
			else {
				if (mario->untouchable == 0) {
					mario->vx = 0;
					if (mario->GetLevel() != MARIO_LEVEL_SMALL)
					{
						mario->StartUntouchable();
					}
					else
						mario->SetState(MARIO_STATE_DIE);

				}
			}
		}
	}
}

void CGoomba::IsCollisionWithBrickSpecially(LPCOLLISIONEVENT e)
{
	if (e->ny != 0) {
		if (e->ny < 0) {
			if (state == GOOMBA_STATE_WALKING_SWINGS_OPEN) {
				if (index < MAX_JUMP_QUANTITY)
					SetState(GOOMBA_STATE_JUMPING_SWINGS_CLOSE);
				else {
					index = 0;
					SetState(GOOMBA_STATE_WALKING_SWINGS);
				}

			}
			else {
				SetState(GOOMBA_STATE_WALKING_SWINGS_CLOSE);
				start_walking = 0;
			}
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

void CGoomba::IsCollisionWithGhostPlatformSpecially(LPCOLLISIONEVENT e)
{
	if (e->ny != 0) {
		if (e->ny < 0) {
			SetState(GOOMBA_STATE_JUMPING_SWINGS_CLOSE);
		}
		else {
			y += dy;
		}
	}
	if (e->nx != 0) x += dx;
}

void CGoomba::AttackedByShell()
{
	SetState(GOOMBA_STATE_DIE_BY_WEAPON);
}

void CGoomba::IsCollisionWithEnemy(LPCOLLISIONEVENT e)
{
	if (e->obj->type == eTYPE::GOOMBA) {
		CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
		if (e->nx != 0) {
			vx *= -1;
			goomba->vx *= -1;
		}
		if (e->ny != 0) {
			if (e->ny < 0)
				y += dy;
			else y -= dy;
		}
	}
	else if (e->obj->type == eTYPE::KOOPA) {
		CKoopa* koopa = dynamic_cast<CKoopa*>(e->obj);
		if (e->ny != 0) {
			if (koopa->state == KOOPA_STATE_SPIN) {
				AttackedByShell();
				koopa->x += dx;
			}
			else if (koopa->state == KOOPA_STATE_DIE) {
				if (e->ny > 0)
					SetState(GOOMBA_STATE_DIE);
				else {
					vy = -GOOMBA_JUMP_SPEED_Y;
					koopa->AttackedByShell();
				}
			}
			else if (koopa->state == KOOPA_STATE_WALKING_SWINGS || koopa->state == KOOPA_STATE_WALKING) {
				if (e->ny > 0) {
					SetState(GOOMBA_STATE_DIE);
				}
				else {
					koopa->SetState(KOOPA_STATE_DIE);
					vy = -GOOMBA_JUMP_SPEED_Y;
				}
			}
		}

		if (e->nx != 0) {
			if (koopa->state == KOOPA_STATE_SPIN) {
				AttackedByShell();
				koopa->x += dx;
			}
			else if (koopa->state == KOOPA_STATE_DIE) {
				vx *= -1;
			}
			else if (koopa->state == KOOPA_STATE_WALKING_SWINGS || koopa->state == KOOPA_STATE_WALKING) {
				if (nx * e->nx > 0) {
					vx *= -1;
					nx *= -1;
					koopa->vx *= -1;
					koopa->nx *= -1;
				}
				else {
					koopa->vx *= -1;
					koopa->nx *= -1;
				}
			}
		}


	}
}

void CGoomba::UpdateWidthHeightGoomba()
{
	switch (state) {
	case GOOMBA_STATE_WALKING:
	case GOOMBA_STATE_DIE:
		w = GOOMBA_BBOX_WIDTH_WALKING;
		h = GOOMBA_BBOX_WIDTH_WALKING;
		break;
	case GOOMBA_STATE_WALKING_SWINGS_CLOSE:
		w = GOOMBA_BBOX_WIDTH_WALKING_SWINGS_CLOSE;
		h = GOOMBA_BBOX_HEIGHT_WALKING_SWINGS_CLOSE;
		break;
	case GOOMBA_STATE_WALKING_SWINGS_OPEN:
	case GOOMBA_STATE_WALKING_SWINGS:
		w = GOOMBA_BBOX_WIDTH_WALKING_SWINGS;
		h = GOOMBA_BBOX_HEIGHT_WALKING_SWINGS;
		break;

	}
}

