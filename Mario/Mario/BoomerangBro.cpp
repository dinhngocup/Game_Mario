#include "BoomerangBro.h"
#include "PlayScene.h"

CBoomerangBro::CBoomerangBro()
{
	generate_id++;
	this->id = generate_id;
	nx = -1;
	SetState(BOOMERANG_BRO_STATE_WALKING_RIGHT);
}

void CBoomerangBro::GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx, int dy)
{
	left = x;
	top = y;

	right = left + w;
	bottom = top + h;
}

void CBoomerangBro::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGame* game = CGame::GetInstance();
	CPlayScene* scene = (CPlayScene*)game->GetCurrentScene();

	if (state != BOOMERANG_BRO_STATE_DIE) {
		if (attacking) {
			if (GetTickCount64() - start_throwing_weapon > 500 && weapon_quantity < 2) {
				start_throwing_weapon = GetTickCount64();
				scene->grid->AddObjectIntoGrid(eTYPE::BOOMERANG, x, y, 0, 0, 30, eTYPE_OBJECT::ENEMY);
				weapon_quantity++;
			}
			if (weapon_quantity == 2) {
				SetState(BOOMERANG_BRO_STATE_IDLE);
			}
			/*else if (GetTickCount64() - start_throwing_weapon > 4000) {
				start_throwing_weapon = GetTickCount64();
				weapon_quantity = 0;
			}*/
			if (GetTickCount64() - start_throwing_weapon > 3000 && weapon_quantity == 2) {
				attacking = false;
				SetState(BOOMERANG_BRO_STATE_WALKING_RIGHT);
			}

		}
		else {
			if (state != BOOMERANG_BRO_STATE_DIE) {
				if (x - start_x > 200) {
					SetState(BOOMERANG_BRO_STATE_WALKING_LEFT);
				}
				if (x < start_x) {
					SetState(BOOMERANG_BRO_STATE_ATTACKING);
				}
			}
		}
	}
	vy += BOOMERANG_BRO_GRAVITY * dt;
	CGameObject::Update(dt);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;



	vector<LPGAMEOBJECT> bricks = scene->ghost_platforms;
	vector<LPGAMEOBJECT> items = scene->items;

	coEvents.clear();

	if (state != BOOMERANG_BRO_STATE_DIE) {
		CalcPotentialCollisions(&items, coEvents);
		CalcPotentialCollisions(&bricks, coEvents);
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
			if (state != BOOMERANG_BRO_STATE_DIE) {
				if (dynamic_cast<CBrickQuestion*>(e->obj)) {
					IsCollisionWithBrick(e);
				}
				else if (dynamic_cast<CInvisibleObject*>(e->obj)) {
					IsCollisionWithGhostPlatform(e);
				}
				else if (dynamic_cast<CBrick*>(e->obj)) {
					IsCollisionWithBrick(e);
				}
			}
		}

		// clean up collision events
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
	if (state == BOOMERANG_BRO_STATE_DIE) {
		if (y > game->GetCamY() + game->GetScreenHeight() ||
			x > game->GetCamX() + game->GetScreenWidth() ||
			x < game->GetCamX()) {
			SetHealth(false);
		}
	}
}

void CBoomerangBro::Render()
{
	int ani = 0;
	int ny = 1;
	if (state == BOOMERANG_BRO_STATE_DIE) {
		ani = BOOMERANG_BRO_ANI_DIE;
		ny = -1;
	}
	else if (state == BOOMERANG_BRO_STATE_WALKING_LEFT ||
		state == BOOMERANG_BRO_STATE_WALKING_RIGHT ||
		state == BOOMERANG_BRO_STATE_IDLE)
		ani = BOOMERANG_BRO_ANI_WALKING;
	else if (state == BOOMERANG_BRO_STATE_ATTACKING)
		ani = BOOMERANG_BRO_ANI_ATTACKING;

	//if (vx >= 0)
	animation_set->at(ani)->Render(x, y, 255, -1, 0, 0, ny);
	//else
		//animation_set->at(ani)->Render(x, y, 255, 1, 0, 0, ny);

	RenderBoundingBox();
}

void CBoomerangBro::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BOOMERANG_BRO_STATE_WALKING_RIGHT:
		vx = BOOMERANG_BRO_WALKING_SPEED;
		break;
	case BOOMERANG_BRO_STATE_WALKING_LEFT:
		vx = -BOOMERANG_BRO_WALKING_SPEED;
		break;
	case BOOMERANG_BRO_STATE_DIE:
		vx = 0;
		vy = -GOOMBA_SPEED_Y_DIE_BY_WEAPON;
		ableToCheckCollision = false;
		break;
	case BOOMERANG_BRO_STATE_ATTACKING:
		vx = 0;
		attacking = true;
		start_throwing_weapon = GetTickCount64();
		weapon_quantity = 0;
		break;

	}
}

void CBoomerangBro::IsCollisionWithMario(LPCOLLISIONEVENT e)
{
	CMario* mario = CMario::GetInstance();
	CGame* game = CGame::GetInstance();
	CPlayScene* scene = (CPlayScene*)game->GetCurrentScene();

	if (e->ny != 0) {
		if (e->ny < 0)
		{
			CPointBonus* point = new CPointBonus(x, y, STATE_100_POINTS);
			scene->effects.push_back(point);
			SetState(BOOMERANG_BRO_STATE_DIE);
			mario->vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
		else {
			mario->y -= dy;
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
	// đụng bên hông 
	if (e->nx != 0)
	{
		// đụng ngang nấm còn đang sống
		if (state != BOOMERANG_BRO_STATE_DIE)
		{
			//DebugOut(L"cham x\n");
			if (mario->is_attacking_by_spinning) {
				mario->vx = 0;
				// đụng bên phải
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

				CPointBonus* point = new CPointBonus(x, y, STATE_100_POINTS);
				scene->effects.push_back(point);

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
