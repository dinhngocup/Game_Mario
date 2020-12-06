#include "Mushroom.h"
#include "PlayScene.h"

CMushroom::CMushroom(float x, float y, int extra)
{
	vy = -0.15f;
	start_y = y;
	start_x = x;
	this->extra = extra;
	SetState(MUSHROOM_STATE_HIDE);
}

void CMushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	CGame* game = CGame::GetInstance();
	CMario* mario = CMario::GetInstance();

	if (y <= start_y - h && state == MUSHROOM_STATE_HIDE) {
		vy += MUSHROOM_GRAVITY * dt;
		SetState(MUSHROOM_STATE_UNHIDE);
	}
	if (state == MUSHROOM_STATE_UNHIDE) {
		vy += MUSHROOM_GRAVITY * dt;
	}
	CGameObject::Update(dt, colliable_objects);
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	CPlayScene* scene = (CPlayScene*)game->GetCurrentScene();

	vector<LPGAMEOBJECT> bricks = scene->ghost_platforms;
	vector<LPGAMEOBJECT> items = scene->items;
	items.push_back(mario);
	coEvents.clear();

	if (state == MUSHROOM_STATE_UNHIDE) {
		CalcPotentialCollisions(&bricks, coEvents);
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
			if (dynamic_cast<CItem*>(e->obj)) {
				if(e->obj->GetType() == eTYPE::BRICK_QUESTION) 
					IsCollisionWithBrick(e);
				else {
					if (e->ny != 0) {
						if (e->ny < 0)
							y += dy;
						else y -= dy;
					}
					if (e->nx != 0) x += dx;
				}

			}
			else if (dynamic_cast<CBrick*>(e->obj)) {
				IsCollisionWithBrick(e);
			}
			else if (dynamic_cast<CInvisibleObject*>(e->obj)) {
				IsCollisionWithGhostPlatform(e);
			}
			else if(dynamic_cast<CMario*>(e->obj)){
				HandleCollisionWithMario(e);
			}

		}

		// clean up collision events
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}

}

void CMushroom::Render()
{
	int ani = 0;
	if (extra == 0)
		ani = ANI_RED_MUSHROOM;
	else ani = ANI_GREEN_MUSHROOM;
	animation_set->at(ani)->Render(x, y, 255, 1, 0, 0);

	CAnimationSets* sets = CAnimationSets::GetInstance();
	sets->Get(2)->at(1)->Render(start_x, start_y);
	RenderBoundingBox();

}

void CMushroom::SetState(int state)
{
	CGameObject::SetState(state);
	CMario* mario = CMario::GetInstance();
	switch (state) {
	case MUSHROOM_STATE_UNHIDE:
		vx = MUSHROOM_VX * mario->nx;
		nx = 1;
		break;
	}
}

void CMushroom::IsCollisionWithMario(LPCOLLISIONEVENT e)
{
	// e is mushroom
	CMario* mario = CMario::GetInstance();
	CGame* game = CGame::GetInstance();
	CPlayScene* scene = (CPlayScene*)game->GetCurrentScene();
	// set tam thời
	mario->vx = 0;
	mario->vy = -MARIO_JUMP_DEFLECT_SPEED;
	SetHealth(false);
	ableToCheckCollision = false;
	if (extra == 0) {
		mario->ChangeState(new CGrowingUpState(mario->GetLevel()));
		CPointBonus* point = new CPointBonus(x, y + h, STATE_1000_POINTS);
		scene->effects.push_back(point);
	}
	else {
		CPointBonus* point = new CPointBonus(x, y + h, STATE_1UP);
		scene->effects.push_back(point);
	}
}

void CMushroom::IsCollisionWithBrick(LPCOLLISIONEVENT e)
{
	if (e->nx != 0) {
		vx *= -1;
		nx *= -1;
	}
	if (e->ny != 0) vy = 0;
}

void CMushroom::HandleCollisionWithMario(LPCOLLISIONEVENT e)
{
	// e is mario
	CMario* mario = CMario::GetInstance();
	CGame* game = CGame::GetInstance();
	CPlayScene* scene = (CPlayScene*)game->GetCurrentScene();
	// set tam thời
	mario->vx = 0;
	mario->vy = -MARIO_JUMP_DEFLECT_SPEED;
	SetHealth(false);
	ableToCheckCollision = false;

	if (extra == 0) {
		mario->ChangeState(new CGrowingUpState(mario->GetLevel()));
		CPointBonus* point = new CPointBonus(x, y + h, STATE_1000_POINTS);
		scene->effects.push_back(point);
	}
	else {
		CPointBonus* point = new CPointBonus(x, y + h, STATE_1UP);
		scene->effects.push_back(point);
	}
}

void CMushroom::GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx, int dy)
{
	left = x;
	top = y;
	right = left + w;
	bottom = y + h;
}
