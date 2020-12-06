#include "PointBonus.h"

CPointBonus::CPointBonus(float x, float y, int state)
{
	//DebugOut(L"new\n");
	start_ani = GetTickCount64();
	SetState(state);
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(ANI_POINT_BONUS);
	SetAnimationSet(ani_set);
	this->x = x;
	this->y = y;
}

void CPointBonus::SetState(int state)
{
	CGameObject::SetState(state);
	CMario* mario = CMario::GetInstance();
	switch (state) {
	case STATE_100_POINTS:
		mario->AddScore(POINT_100);
		break;
	case STATE_200_POINTS:
		mario->AddScore(POINT_200);
		break;
	case STATE_1000_POINTS:
		mario->AddScore(POINT_1000);
		break;
	case STATE_1UP:
		mario->AddLive();
		break;
	}
}

void CPointBonus::Render()
{
	int ani = 0;
	switch (state) {
	case STATE_100_POINTS:
		ani = ANI_100_POINTS;
		break;
	case STATE_200_POINTS:
		ani = ANI_200_POINTS;
		break;
	case STATE_1000_POINTS:
		ani = ANI_1000_POINTS;
		break;
	case STATE_1UP:
		ani = ANI_1UP;
		break;
	}
	animation_set->at(ani)->Render(x, y);

}

void CPointBonus::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (GetTickCount64() - start_ani >= TIME_POINT_EFFECT) {
		SetHealth(false);
	}
	else {
		vy = VY_POINT_BONUS;
		CGameObject::Update(dt);
		y += dy;
	}
}
