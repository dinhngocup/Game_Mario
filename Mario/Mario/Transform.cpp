#include "Transform.h"

CTransform::CTransform(float x, float y)
{
	start_ani = GetTickCount64();
	DebugOut(L"new transform\n");
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(ANI_TRANSFORM);
	SetAnimationSet(ani_set);
	w = 12;
	h = 12;
	SetPosition(x, y);
	generate_id++;
	this->id = generate_id;
}


void CTransform::Render()
{
	animation_set->at(0)->Render(x, y);
}

void CTransform::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CMario* mario = CMario::GetInstance();
	if (GetTickCount64() - start_ani >= 600) {
		SetHealth(false);
		mario->SetState(MARIO_STATE_UNHIDE_UNTOUCHABLE);
		mario->unhide_start = GetTickCount64();
	}
}

