#include "Koopa.h"

CKoopa::CKoopa()
{
	SetState(KOOPA_STATE_WALKING);
}

CKoopa::CKoopa(int state)
{
	SetState(state);
}

void CKoopa::GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx, int dy)
{
	left = x - KOOPA_BBOX_WIDTH / 2;
	top = y - KOOPA_BBOX_HEIGHT / 2;
	right = left + KOOPA_BBOX_WIDTH;

	if (state == KOOPA_STATE_DIE)
		bottom = top + KOOPA_BBOX_HEIGHT_DIE;
	else
		bottom = top + KOOPA_BBOX_HEIGHT;
}

void CKoopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//CGameObject::Update(dt);
	//// Simple fall down
	//vy += KOOPA_GRAVITY * dt;
	//
	//vector<LPCOLLISIONEVENT> coEvents;
	//vector<LPCOLLISIONEVENT> coEventsResult;

	//coEvents.clear();

	//CalcPotentialCollisions(coObjects, coEvents);

	//if (coEvents.size() == 0)
	//{
	//	x += dx;
	//	y += dy;
	//}
	//else
	//{
	//	float min_tx, min_ty, nx = 0, ny;
	//	float rdx = 0;
	//	float rdy = 0;
	//	
	//	FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy); 

	//	// block every object first!
	//	x += min_tx * dx + nx * 0.4f;
	//	y += min_ty * dy + ny * 0.4f;

	//	for (UINT i = 0; i < coEventsResult.size(); i++)
	//	{
	//		LPCOLLISIONEVENT e = coEventsResult[i];
	//		if (dynamic_cast<CBrick*>(e->obj) || dynamic_cast<CMario*>(e->obj)) {
	//			//CBrick* brick = dynamic_cast<CBrick*>(e->obj);
	//			if (e->nx != 0) {
	//				vx *= -1;
	//			}
	//		}
	//		else {
	//			if (e->nx != 0) vx = 0;
	//			if (e->ny != 0)	vy = 0;
	//		}
	//	}
	//	// clean up collision events
	//	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	//}
}

void CKoopa::Render()
{
	int ani = 0; 
	if (state == KOOPA_STATE_DIE)
		ani = KOOPA_ANI_DIE;
	else if(state == KOOPA_STATE_WALKING)
		ani = KOOPA_ANI_WALKING;
	else if (state == KOOPA_STATE_WALKING_SWINGS)
		ani = KOOPA_ANI_WALKING_SWINGS;
	if(vx >= 0)
		animation_set->at(ani)->Render(x, y, 255, -1);
	else
		animation_set->at(ani)->Render(x, y);

	RenderBoundingBox();
}

void CKoopa::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPA_STATE_WALKING:
		vx = -KOOPA_WALKING_SPEED;
		vx = 0;
		break;
	case KOOPA_STATE_DIE:
		y += KOOPA_BBOX_HEIGHT - KOOPA_BBOX_HEIGHT_DIE;
		vx = 0;
		vy = 0;
		break;
	case KOOPA_STATE_WALKING_SWINGS:
		vx = -KOOPA_WALKING_SPEED;
		break;
	}

}