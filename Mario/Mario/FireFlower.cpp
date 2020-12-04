#include "FireFlower.h"
#include "PlayScene.h"

CFireFlower::CFireFlower(int flower_color, float x, float y, int w, int h)
{
	generate_id++;
	this->id = generate_id;
	this->flower_color = flower_color;
	start_x = x;
	start_y = y;
	boundaryY = start_y - h;
	start_state = STATE_MOVING_UP;
	SetState(STATE_MOVING_UP);
}

void CFireFlower::GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx, int dy)
{
	left = x;
	top = y; 

	right = left + w;
	bottom = top + h;
}

void CFireFlower::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CMario* mario = CMario::GetInstance();
	if (state == STATE_MOVING_UP) {
		if (y <= start_y - h) {
			y = start_y - h;
			if (mario->y <= boundaryY) {
				ny = -1;
				SetState(STATE_ATTACKING_UP);
			}
			else {
				ny = 1;
				SetState(STATE_ATTACKING_DOWN);
			}
		}
		else y += dy;
	}
	else if (state == STATE_MOVING_DOWN) {
		if (y >= start_y)
			SetState(STATE_HIDE);
		else y += dy;
	}
	else if (state == STATE_ATTACKING_UP || state == STATE_ATTACKING_DOWN) {
		if (GetTickCount64() - start_count >= FF_TIME_ATTACKING) {
			SetState(STATE_MOVING_DOWN);
		}
	}
	else {
		// state là hide
		if (mario->x >= start_x - FF_MIN_BOUNDARY && mario->x <= start_x + FF_MAX_BOUNDARY) {

		}
		else {
			if (GetTickCount64() - start_count >= FF_TIME_HIDE) {
				SetState(STATE_MOVING_UP);
			}
		}
	}
	CGameObject::Update(dt);
	x += dx;
	y += dy;
	
}

void CFireFlower::Render()
{
	CMario* mario = CMario::GetInstance();
	int ani;
	if (flower_color == RED_FLOWER) {
		switch (state) {
		case STATE_HIDE:
			ani = RED_FF_ANI_ATTACKING_UP;
			break;
		case STATE_ATTACKING_UP:
			ani = RED_FF_ANI_ATTACKING_UP;
			break;
		case STATE_ATTACKING_DOWN:
			ani = RED_FF_ANI_ATTACKING_DOWN;
			break;
		case STATE_MOVING_UP:
		case STATE_MOVING_DOWN:
			if (ny < 0)
				ani = RED_FF_ANI_UP;
			else
				ani = RED_FF_ANI_DOWN;
			break;
		}
	}
	else {
		switch (state) {
		case STATE_HIDE:
			ani = GREEN_FF_ANI_ATTACKING_UP;
			break;
		case STATE_ATTACKING_UP:
			ani = GREEN_FF_ANI_ATTACKING_UP;
			break;
		case STATE_ATTACKING_DOWN:
			ani = GREEN_FF_ANI_ATTACKING_DOWN;
			break;
		case STATE_MOVING_UP:
		case STATE_MOVING_DOWN:
			if (ny < 0)
				ani = GREEN_FF_ANI_UP;
			else
				ani = GREEN_FF_ANI_DOWN;
			break;
		}
	}

	animation_set->at(ani)->Render(x, y, 255, nx);
	RenderBoundingBox();
}

void CFireFlower::SetState(int state)
{
	CGameObject::SetState(state);
	CMario* mario = CMario::GetInstance();
	CGame* game = CGame::GetInstance();
	switch (state) {
	case STATE_MOVING_UP:
		if (mario->y <= boundaryY) {
			ny = -1;
		}
		else
			ny = 1;
		if (mario->x <= start_x)
			nx = 1;
		else nx = -1;
		vy = -FF_SPEED;
		break;
	case STATE_MOVING_DOWN:
		if (mario->y <= boundaryY) {
			ny = -1;
		}
		else
			ny = 1;
		vy = FF_SPEED;
		break;
	case STATE_ATTACKING_UP:
	case STATE_ATTACKING_DOWN: {
		//bắn lửa
		CreateWeapon();
		start_count = GetTickCount64();
		vy = 0;
		break;
	}
	case STATE_HIDE:
		start_count = GetTickCount64();
		vy = 0;
		break;
	}
}

void CFireFlower::IsCollisionWithMario(LPCOLLISIONEVENT e)
{
	CMario* mario = CMario::GetInstance();
	if (e->nx != 0) {
		if (mario->is_attacking_by_spinning) {
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
			
			CPointBonus* point = new CPointBonus(x, y, STATE_100_POINTS);
			scene->effects.push_back(point);

			SetHealth(false);
			return;
		}
	}
	mario->StartUntouchable();
}

void CFireFlower::CreateWeapon()
{
	CGame* game = CGame::GetInstance();
	CPlayScene* scene = (CPlayScene*)game->GetCurrentScene();
	CMario* mario = CMario::GetInstance();
	int angle = 0;
	if (mario->x >= start_x - 24 + BOUNDARY_ANGLE * nx) {
		angle = 1;
	}
	scene->grid->AddObjectIntoGrid(eTYPE::FIRE_FLOWER_WEAPON, x + 10 * nx * -1, y + 10, 24, 24, ANI_FF_WEAPON, eTYPE_OBJECT::ITEM, ny, nx, angle);

}

void CFireFlower::IsCollisionWithEnemy(LPCOLLISIONEVENT e)
{
}
