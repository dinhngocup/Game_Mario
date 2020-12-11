#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Mario.h"
#include "Game.h"
#include "PlayScene.h"
#include "MapScene.h"

CMario* CMario::__instance = NULL;
CMario::CMario() : CGameObject()
{
	untouchable = 0;
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGame* game = CGame::GetInstance();
	CPlayScene* scene = (CPlayScene*)game->GetCurrentScene();
	if ((state == MARIO_STATE_DIE && y > game->GetCamY() + 560 ||
		y > game->GetCamY() + 560) && !scene->time_up) {
		DebugOut(L"handle mario die\n");
		if (lives > 0)
			MinusLive();
		else
			ResetLive();
		scene->mario_die = true;
		//level = MARIO_LEVEL_SMALL;
		return;
	}
	if (state == MARIO_AUTO_GO && x > game->GetCamX() + game->GetScreenWidth()) {
		//DebugOut(L"stop mario\n");
		vx = 0;
		return;
	}
	if (level == FIRE_LEVEL && start_press_z == 0)
		start_press_z = GetTickCount64();
	if (state != MARIO_STATE_DIE && state != MARIO_AUTO_GO)
		player_state->Update(dt);
	if (!dynamic_cast<CGoDownState*>(player_state))
		vy += MARIO_GRAVITY * dt;

	scene->UpdateSpeedBar(abs(vx));
	CGameObject::Update(dt);

	vector<LPGAMEOBJECT> enemies = scene->enemies;
	vector<LPGAMEOBJECT> bricks = scene->ghost_platforms;
	vector<LPGAMEOBJECT> items = scene->items;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != MARIO_STATE_DIE && !dynamic_cast<CGoDownState*>(player_state)) {
		CalcPotentialCollisions(&bricks, coEvents);
		CalcPotentialCollisions(&enemies, coEvents);
		CalcPotentialCollisions(&items, coEvents);
	}

	if (untouchable == 1) {
		if (!dynamic_cast<CGrowingUpState*>(player_state)) {

			if (GetTickCount64() - untouchable_start >= time_flicker
				&& GetTickCount64() - untouchable_start < MARIO_UNTOUCHABLE_TIME) {
				if (GetTickCount64() - unhide_start >= 100) {
					if (state == MARIO_STATE_HIDE_UNTOUCHABLE)
						SetState(MARIO_STATE_UNHIDE_UNTOUCHABLE);
					else
						SetState(MARIO_STATE_HIDE_UNTOUCHABLE);
					unhide_start = GetTickCount64();
				}
			}
			else if (GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME) {
				untouchable_start = 0;
				untouchable = 0;
				SetState(0);
			}
		}
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
			if (state != MARIO_STATE_DIE && state != MARIO_AUTO_GO) {
				if (dynamic_cast<CPortal*>(e->obj)) {

					CPortal* portal = dynamic_cast<CPortal*>(e->obj);
					if (e->ny != 0) {
						vy = 0;
						collide_with_portal = portal->direction_collision;

						if (e->ny == portal->direction_collision && e->ny == -1) {
							if (is_underground) {
								ChangeState(new CGoDownState(level));
								CGoDownState* go_down_state = dynamic_cast<CGoDownState*>(player_state);
								x = portal->x;
								go_down_state->start_y = portal->y_change;
								portal->is_activated = true;

								is_underground = false;
							}

						}
						else if (e->ny == portal->direction_collision && e->ny == 1) {
							ChangeState(new CGoDownState(level));
							CGoDownState* go_down_state = dynamic_cast<CGoDownState*>(player_state);
							portal->is_activated = true;
							go_down_state->start_y = portal->y_change;
							x = portal->x;
							is_underground = false;

						}
					}


				}
				else if (dynamic_cast<CInvisibleObject*>(e->obj)) {
					IsCollisionWithGhostPlatform(e);
				}
				else if (dynamic_cast<CEnemy*>(e->obj)) {
					if (false) {
						if (e->nx != 0)
							x += dx;
						if (e->ny != 0) {
							if (e->ny < 0)
								y += dy;
							else y -= dy;
						}
					}
					else {
						if (is_holding && e->obj->state == KOOPA_STATE_HOLD) {

						}
						else if (e->obj->GetType() == eTYPE::FIRE_FLOWER_WEAPON) {
							if (e->nx != 0) {
								x += dx;
							}
							if (e->ny != 0) {
								if (e->ny < 0)
									y += dy;
								else y -= dy;
							}
							e->obj->IsCollisionWithMario(e);

						}
						else if (e->obj->GetType() == eTYPE::FIRE_FLOWER) {

							e->obj->IsCollisionWithMario(e);

							if (e->nx != 0)
								x += dx;
							if (e->ny != 0) {
								if (e->ny < 0)
									y += dy;
								else y -= dy;
							}
						}
						else {
							e->obj->IsCollisionWithMario(e);
							//DebugOut(L"hiii\n");
						}
					}

				}
				else if (dynamic_cast<CItem*>(e->obj)) {
					e->obj->IsCollisionWithMario(e);
				}
				else if (dynamic_cast<CBrick*>(e->obj)) {
					CBrick* brick = dynamic_cast<CBrick*>(e->obj);
					if (brick->state == BLING_BLING_BRICK) {
						brick->IsCollisionWithMario(e);
					}
					else
						IsCollisionWithBrick(e);
				}
			}
			else if (state == MARIO_AUTO_GO) {
				if (dynamic_cast<CBrick*>(e->obj)) {
					if (e->ny != 0)	vy = 0;
					if (e->nx != 0) x += dx;
				}
			}
		}
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}

}

void CMario::Render()
{
	int offset = 0;
	bool spinningFlag = false;
	int alpha = 255;
	if (state == MARIO_STATE_DIE)
		ani = MARIO_ANI_DIE;
	else if (state == MARIO_AUTO_GO) {
		switch (level) {
		case RACCOON_LEVEL_BIG:
			ani = RACCOON_ANI_BIG_WALKING_RIGHT;
			break;
		case MARIO_LEVEL_BIG:
			ani = MARIO_ANI_BIG_WALKING_RIGHT;
			break;
		case MARIO_LEVEL_SMALL:
			ani = MARIO_ANI_SMALL_WALKING_RIGHT;
			break;
		case FIRE_LEVEL:
			ani = FIRE_MARIO_ANI_WALKING_RIGHT;
			break;
		}
		nx = 1;
	}
	else {

		if (dynamic_cast<CRunningState*>(player_state) && level != RACCOON_LEVEL_BIG && level != FIRE_LEVEL)
			player_state->SetAnimation(level);
		if (!dynamic_cast<CGrowingUpState*>(player_state))
			this->ani = player_state->GetAnimation();

		if (level == RACCOON_LEVEL_BIG) {
			if (nx > 0)
				offset = OFFSET_RACCOON;
		}

		if (ani == RACCOON_ANI_SPINNING_BIG) {
			spinningFlag = true;
			if (nx < 0)
				offset = -OFFSET_RACCOON;
			else offset = OFFSET_RACCOON;

		}

		if (state == MARIO_STATE_HIDE_UNTOUCHABLE || state == MARIO_STATE_HIDE) alpha = 0;
		else if (state == MARIO_STATE_UNHIDE_UNTOUCHABLE) alpha = 255;

		if (state == MARIO_STATE_SMALL_GROW_UP)
			ani = MARIO_ANI_SMALL_GROW_UP;
		else if (state == MARIO_STATE_BIG_GROW_UP)
			ani = MARIO_ANI_BIG_GROW_UP;
		else if (state == MARIO_STATE_BIG_END_GROW_UP)
			ani = MARIO_ANI_BIG_IDLE_RIGHT;

		if (state == MARIO_END_GROWING) {
			if (level == MARIO_LEVEL_SMALL)
				ani = MARIO_ANI_SMALL_IDLE_RIGHT;
			else
				ani = MARIO_ANI_BIG_IDLE_RIGHT;

		}
	}
	//DebugOut(L"ani%d", ani);
	animation_set->at(ani)->Render(x, y, alpha, nx, offset, spinningFlag);

	RenderBoundingBox();
}

void CMario::UpdateInMapScene(DWORD dt)
{
	CGame* game = CGame::GetInstance();
	CMapScene* scene = (CMapScene*)game->GetCurrentScene();

	if (is_auto_go_in_map) {
		CGameObject::Update(dt);
		x += dx;
		y += dy;
		if (vx != 0) {
			if (x <= scene->current_portal->x && vx < 0 || x >= scene->current_portal->x && vx > 0) {
				x = scene->current_portal->x;
				is_auto_go_in_map = false;
				vx = 0;

			}
		} else if (vy != 0) {
			if (y <= scene->current_portal->y && vy < 0 || y >= scene->current_portal->y && vy > 0) {
				y = scene->current_portal->y;
				is_auto_go_in_map = false;
				vy = 0;

			}
		}

	}
}

void CMario::RenderInMapScene()
{
	int ani = 0;
	switch (level) {
	case MARIO_LEVEL_SMALL:
		ani = MARIO_ANI_SMALL_IN_MAP;
		break;
	case MARIO_LEVEL_BIG:
		ani = MARIO_ANI_SMALL_IN_MAP;
		break;
	case RACCOON_LEVEL_BIG:
		ani = RACCOON_ANI_IN_MAP;
		break;
	case FIRE_LEVEL:
		ani = FIRE_ANI_IN_MAP;
		break;
	}

	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}

void CMario::GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx = 0, int dy = 0)
{
	CGame* game = CGame::GetInstance();
	if (level == MARIO_LEVEL_BIG && !dynamic_cast<CGrowingUpState*>(player_state)
		|| level == FIRE_LEVEL || level == RACCOON_LEVEL_BIG)
	{
		left = x;
		right = left + MARIO_BIG_BBOX_WIDTH;
		if (is_crouching) {
			top = y;
			bottom = top + MARIO_CROUCH_BBOX_HEIGHT;
		}
		else if (is_attacking_by_spinning) {
			left = x;
			top = y;
			bottom = top + MARIO_BIG_BBOX_HEIGHT;
			right = left + MARIO_BIG_BBOX_WIDTH;
		}
		else {
			top = y;
			bottom = top + MARIO_BIG_BBOX_HEIGHT;
		}
	}
	else
	{
		left = x;
		top = y;
		if (state == MARIO_STATE_BIG_GROW_UP) {
			right = left + 42;
			bottom = top + 57;
		}
		else if (state == MARIO_STATE_BIG_END_GROW_UP) {
			right = left + MARIO_BIG_BBOX_WIDTH;
			bottom = top + MARIO_BIG_BBOX_HEIGHT;
		}
		else {
			right = left + MARIO_SMALL_BBOX_WIDTH;
			bottom = top + MARIO_SMALL_BBOX_HEIGHT;
		}

	}
}

void CMario::GetHeightMario()
{
	switch (level) {
	case MARIO_LEVEL_SMALL:
		this->h = MARIO_SMALL_BBOX_HEIGHT;
		this->w = MARIO_SMALL_BBOX_WIDTH;
		break;
	case MARIO_LEVEL_BIG:
	case RACCOON_LEVEL_BIG:
	case FIRE_LEVEL:
		if (is_crouching) {
			this->h = MARIO_CROUCH_BBOX_HEIGHT;

		}
		else
			this->h = MARIO_BIG_BBOX_HEIGHT;
		this->w = MARIO_BIG_BBOX_WIDTH;
		break;

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
	if (player_state != NULL)
		player_state->OnKeyDown(KeyCode);
}

void CMario::OnKeyUp(int KeyCode)
{
	if (player_state != NULL)
		player_state->OnKeyUp(KeyCode);
}

void CMario::KeyState(BYTE* states)
{
	if (player_state != NULL)
		player_state->KeyState(states);
}

void CMario::ChangeState(CPlayerState* newState)
{
	
	delete player_state;
	player_state = NULL;
	player_state = newState;

	if (((dynamic_cast<CSpinningState*>(player_state) ||
		dynamic_cast<CRunningState*>(player_state))) && level == RACCOON_LEVEL_BIG) {
		ani = RACCOON_ANI_SPINNING_BIG;
		animation_set->at(ani)->ResetFlagLastFrame();
		player_state->SetAnimation(animation_set->at(ani));
	}
	else if (level == FIRE_LEVEL && (dynamic_cast<CAttackingState*>(player_state) ||
		dynamic_cast<CRunningState*>(player_state))) {
		ani = FIRE_MARIO_ANI_THROW;
		animation_set->at(ani)->ResetFlagLastFrame();
		player_state->SetAnimation(animation_set->at(ani));
	}
	else if (dynamic_cast<CKickingState*>(player_state)) {
		ani = player_state->GetAnimation();
		animation_set->at(ani)->ResetFlagLastFrame();
		player_state->SetAnimation(animation_set->at(ani));
	}
}

void CMario::StartUntouchable()
{
	vx = 0;
	CGame* game = CGame::GetInstance();
	if (level == MARIO_LEVEL_SMALL) {
		SetState(MARIO_STATE_DIE);
	}
	else if (level == MARIO_LEVEL_BIG) {
		untouchable = 1;
		ChangeState(new CGrowingUpState(level));
	}
	else {
		untouchable = 1;
		untouchable_start = GetTickCount64();
		// xu ly bang cuc lua no
		time_flicker = TIME_TRANSFORM_EFFECT;
		SetState(MARIO_STATE_HIDE_UNTOUCHABLE);

		CPlayScene* scene = (CPlayScene*)game->GetCurrentScene();

		float effect_x, effect_y;

		if (level == MARIO_LEVEL_SMALL) {
			effect_x = x + MARIO_SMALL_BBOX_WIDTH / 2;
			effect_y = y + MARIO_SMALL_BBOX_HEIGHT / 2;
		}
		else {
			effect_x = x;
			effect_y = y + MARIO_BIG_BBOX_HEIGHT / 4;
		}

		scene->time_scale = 0;
		CTransform* transform = new CTransform(effect_x, effect_y);
		scene->effects.push_back(transform);
		level--;
		ChangeState(new CStandingState(level));

	}
}

void CMario::UpLevel()
{
	CGame* game = CGame::GetInstance();
	CPlayScene* scene = (CPlayScene*)game->GetCurrentScene();
	SetState(MARIO_STATE_HIDE);
	float effect_x, effect_y;
	scene->time_scale = 0;
	if (level == MARIO_LEVEL_SMALL) {
		effect_x = x + MARIO_SMALL_BBOX_WIDTH / 2;
		effect_y = y + MARIO_SMALL_BBOX_HEIGHT / 2;
	}
	else {
		effect_x = x;
		effect_y = y + MARIO_BIG_BBOX_HEIGHT / 4;
	}
	CTransform* transform = new CTransform(effect_x, effect_y);
	scene->effects.push_back(transform);


}

/*
	Reset Mario status to the beginning state of a scene
*/
void CMario::Reset()
{
	SetPosition(GetX(), GetY());
	SetSpeed(0, 0);
	ChangeState(new CStandingState(this->level));
}

void CMario::SetState(int state) {
	CGameObject::SetState(state);
	CGame* game = CGame::GetInstance();
	CPlayScene* scene = (CPlayScene*)game->GetCurrentScene();

	switch (state)
	{
	case MARIO_STATE_DIE:
		player_state = NULL;
		vy = -MARIO_DIE_DEFLECT_SPEED;
		vx = 0;
		scene->time_scale = 0;
		break;
	case MARIO_STATE_HIDE_UNTOUCHABLE:
		vx = 0;
		vy = 0;
		break;
	case MARIO_STATE_ATTACKING:
		is_attacking_by_spinning = true;
		break;
	case MARIO_STATE_NO_ATTACKING:
		is_attacking_by_spinning = false;
		break;
	case MARIO_AUTO_GO:
		vx = 0.2f;
		break;
	default:
		break;
	}
}

void CMario::IsCollisionWithBrick(LPCOLLISIONEVENT e)
{
	if (!dynamic_cast<CRunningState*>(player_state))
		if (e->nx != 0) vx = 0;
	if (e->ny != 0)	vy = 0;
}

void CMario::IsCollisionWithBlingBlingBrick(LPCOLLISIONEVENT e)
{
}

void CMario::MarioAutoGo()
{
	player_state = NULL;
		SetState(MARIO_AUTO_GO);
	/*if (state != MARIO_AUTO_GO) {
		state = MARIO_STATE_IDLE;
	} else if()

	if (dynamic_cast<CStandingState*>(player_state)) {
	}*/
}

void CMario::MarioAutoGoInMap(float x, float y)
{
}


