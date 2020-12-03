#include "Transform.h"
#include "PlayScene.h"

void CTransform::GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx, int dy)
{
	left = x;
	top = y;
	right = left + w;
	bottom = top + h;
}

CTransform::CTransform(float x, float y)
{
	start_ani = GetTickCount64();
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(ANI_TRANSFORM);
	SetAnimationSet(ani_set);
	w = TRANSFORM_EFFECT_W;
	h = TRANSFORM_EFFECT_H;
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
	CGame* game = CGame::GetInstance();
	CPlayScene* scene = (CPlayScene*)game->GetCurrentScene();
	if (GetTickCount64() - start_ani >= TIME_TRANSFORM_EFFECT) {
		SetHealth(false);
		scene->time_scale = 1;
		mario->SetState(MARIO_STATE_UNHIDE_UNTOUCHABLE);
		mario->unhide_start = GetTickCount64();
	}
}

