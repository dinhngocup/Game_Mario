#include "Coin.h"
#include "PlayScene.h"

CCoin::CCoin(float x, float y)
{
	this->x = x;
	this->y = y;
	vy = COIN_VY;
	start_y = y;
}

void CCoin::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	CGame* game = CGame::GetInstance();
	CPlayScene* scene = (CPlayScene*)game->GetCurrentScene();

	vy += COIN_GRAVITY * dt;
	CGameObject::Update(dt, colliable_objects);
	if (y + dy >= start_y - 80 && vy >0) {
		CMario* mario = CMario::GetInstance();
		CPointBonus* point = new CPointBonus(x, y + 14 * 3, STATE_100_POINTS);
		scene->effects.push_back(point);
		mario->AddCoin();
		SetHealth(false);
		ableToCheckCollision = false;
		return;
	}
	y += dy;

}

void CCoin::Render()
{
	animation_set->at(0)->Render(x, y, 255, 1, 0, 0);
}

void CCoin::SetState(int state)
{
}

void CCoin::IsCollisionWithMario(LPCOLLISIONEVENT e)
{
	
}

void CCoin::GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx, int dy)
{
}
