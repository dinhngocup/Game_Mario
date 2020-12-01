#include "StarEffect.h"

CStarEffect::CStarEffect(float x, float y)
{
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(ANI_TRANSFORM);
	SetAnimationSet(ani_set);
	this->x = x;
	this->y = y;
	generate_id++;
	this->id = generate_id;
	DebugOut(L"x%f \n", x);
	DebugOut(L"y%f \n", y);
}

void CStarEffect::Render()
{

	animation_set->at(0)->Render(x, y);
}

void CStarEffect::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (GetTickCount64() - start_ani >= 90) {
		SetHealth(false);
	}
}
