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
	if (GetTickCount64() - start_press >= TIME_CONVERT_COIN_TO_BLINGBLING && start_press != 0) {
		int size = 0;
		for (LPGAMEOBJECT coin : scene->items) {
			if (coin->GetType() == eTYPE::STATIC_COIN) {
				scene->grid->AddObjectIntoGrid(eTYPE::BRICK, coin->x, coin->y, BLINGBLING_WIDTH, BLINGBLING_HEIGHT, BLINGBLING_ANI, eTYPE_OBJECT::ITEM);
				coin->SetHealth(false);
			}
		}
		//DebugOut(L"size %d\n", size);
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
		y += BUTTON_P_DISPARITY;
		//DebugOut(L"convert all bling bling brick into coin\n");
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
			scene->grid->AddObjectIntoGrid(eTYPE::STATIC_COIN, blingbling->x, blingbling->y, COIN_WIDTH, COIN_HEIGHT, COIN_ANI, eTYPE_OBJECT::ITEM);
			blingbling->SetHealth(false);
		}
	}
	//DebugOut(L"size %d\n", size);
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
