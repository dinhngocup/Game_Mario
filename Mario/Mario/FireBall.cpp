#include "FireBall.h"
#include "Game.h"
#include "PlayScene.h"

CFireBall::CFireBall()
{

}

CFireBall::CFireBall(float start_x, float start_y, int nx)
{
	//OutputDebugString(L"new fire ball\n");
	generate_id++;
	this->id = generate_id;
	w = 24;
	h = 24;
	vx = 0.9f * nx;
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(ANI_FIRE_BALL);
	SetAnimationSet(ani_set);
	SetState(FIRE_BALL_STATE_FLY);
	SetPosition(start_x, start_y);
}

void CFireBall::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	//OutputDebugString(L"update fire ball\n");
	CGame* game = CGame::GetInstance();
	if (GetTickCount64() - start_ani >= EXPLOSIVE_TIME && state == FIRE_BALL_STATE_EXPLODE) {
		SetHealth(false);
		return;
	}
	if (state == FIRE_BALL_STATE_FLY) {
		vy += FIRE_BALL_GRAVITY * dt;
		CGameObject::Update(dt, colliable_objects);

		CPlayScene* scene = (CPlayScene*)game->GetCurrentScene();
		vector<LPGAMEOBJECT> enemies = scene->enemies;
		vector<LPGAMEOBJECT> bricks = scene->ghost_platforms;

		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;
		coEvents.clear();

		// Fireball ra khoi cam còn thiếu cam y 
		if (GetX() >= game->GetCamX() + game->GetScreenWidth() ||
			GetX() <= game->GetCamX() ||
			GetY() <= game->GetCamY() ||
			GetY() >= game->GetCamY() + game->GetScreenHeight()) {
			SetHealth(false);
			return;
		}

		CalcPotentialCollisions(&bricks, coEvents);
		CalcPotentialCollisions(&enemies, coEvents);

		// No collision occured, proceed normally
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


			// TODO: This is a very ugly designed function!!!!
			FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

			// block every object first!
			x += min_tx * dx + nx * 0.4f;
			y += min_ty * dy + ny * 0.4f;



			//
			// Collision logic with other objects
			//
			for (UINT i = 0; i < coEventsResult.size(); i++)
			{
				LPCOLLISIONEVENT e = coEventsResult[i];
				if (dynamic_cast<CBrick*>(e->obj) || dynamic_cast<CBrickQuestion*>(e->obj)) {
					IsCollisionWithBrick(e);
				}
				else if (dynamic_cast<CInvisibleObject*>(e->obj)) {
					IsCollisionWithGhostPlatform(e);
				}
				else if (dynamic_cast<CEnemy*>(e->obj)) {
					IsCollisionWithEnemy(e);
				}

			}

			// clean up collision events
			for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
		}
	}


}

void CFireBall::Render()
{
	int ani = 0;
	if (state == FIRE_BALL_STATE_FLY) ani = FIRE_BALL_ANI_FLY;
	else ani = FIRE_BALL_ANI_EXPLODE;
	animation_set->at(ani)->Render(x, y);
}

void CFireBall::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state) {
	case FIRE_BALL_STATE_EXPLODE:
		start_ani = GetTickCount64();
		break;
	}
}



void CFireBall::GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx, int dy)
{
	left = x;
	top = y;
	right = left + 24;
	bottom = top + 24;
}

void CFireBall::IsCollisionWithBrick(LPCOLLISIONEVENT e)
{
	if (e->ny < 0) {
		vy = -0.4f;
	}
	if (e->nx != 0) {
		SetState(FIRE_BALL_STATE_EXPLODE);
		vx = 0;
	}
}

void CFireBall::IsCollisionWithEnemy(LPCOLLISIONEVENT e)
{
	SetState(FIRE_BALL_STATE_EXPLODE);
	if (e->nx > 0) {
		e->obj->nx = -1;
	}
	else
		e->obj->nx = 1;
	switch (e->obj->type) {
		case eTYPE::KOOPA: {
			CKoopa* koopa = dynamic_cast<CKoopa*>(e->obj);
			koopa->SetState(KOOPA_STATE_DIE_BY_WEAPON);
			break;
		}
		case eTYPE::GOOMBA: {
			CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
			goomba->SetState(GOOMBA_STATE_DIE_BY_WEAPON);
			break;
		}
		case eTYPE::FLOWER: 
		case eTYPE::FIRE_FLOWER: {
			e->obj->SetHealth(false);
			break;
		}
	}
}
