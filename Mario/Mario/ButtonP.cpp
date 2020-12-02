#include "ButtonP.h"
#include "PlayScene.h"

CButtonP::CButtonP(float x, float y)
{
	this->x = x;
	this->y = y;
	this->state = STATE_NOT_PRESSED_YET;
}

void CButtonP::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	CGame* game = CGame::GetInstance();
	CPlayScene* scene = (CPlayScene*)game->GetCurrentScene();
	if (GetTickCount64() - start_press >= 3000 && start_press != 0) {
		int size = 0;
		for (LPGAMEOBJECT coin : scene->items) {
			if (coin->GetType() == eTYPE::STATIC_COIN) {
				scene->grid->AddObjectIntoGrid(eTYPE::BRICK, coin->x, coin->y, 48, 48, 3, eTYPE_OBJECT::ITEM);
				coin->SetHealth(false);
			}
		}
		DebugOut(L"size %d\n", size);
		start_press = 0;
	}

}

void CButtonP::Render()
{
	int ani = 0;
	if (state == STATE_NOT_PRESSED_YET)
		ani = ANI_NOT_PRESSED_YET;
	else
		ani = ANI_PRESSED;

	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}

void CButtonP::SetState(int state)
{
	CGameObject::SetState(state);
	if (state == STATE_PRESSED) {
		// convert all bling bling brick into coin
		y += 27;
		DebugOut(L"convert all bling bling brick into coin\n");
		CreateBonus();
	}
}

void CButtonP::CreateBonus()
{
	CGame* game = CGame::GetInstance();
	CPlayScene* scene = (CPlayScene*)game->GetCurrentScene();
	int size = 0;
	for (LPGAMEOBJECT blingbling : scene->items) {
		if (blingbling->GetType() == eTYPE::BRICK && blingbling->state == BLING_BLING_BRICK) {
			scene->grid->AddObjectIntoGrid(eTYPE::STATIC_COIN, blingbling->x, blingbling->y, 14, 16, 14, eTYPE_OBJECT::ITEM);
			blingbling->SetHealth(false);
		}
	}
	DebugOut(L"size %d\n", size);
}

void CButtonP::IsCollisionWithMario(LPCOLLISIONEVENT e)
{
	CMario* mario = CMario::GetInstance();
	if (e->ny != 0) {
		if (e->ny < 0 && state == STATE_NOT_PRESSED_YET) {
			start_press = GetTickCount64();
			SetState(STATE_PRESSED);
		}
		mario->vy = 0;
	}
	if (e->nx != 0) mario->vx = 0;
}

void CButtonP::GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx, int dy)
{
	top = y;
	left = x;
	right = left + w;
	if (state == STATE_NOT_PRESSED_YET) {
		bottom = top + h;
	}
	else {
		bottom = top + BUTTON_PRESSED_HEIGHT;
	}
}
