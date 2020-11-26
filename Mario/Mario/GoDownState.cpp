#include "GoDownState.h"

CGoDownState::CGoDownState(int level)
{
	DebugOut(L"go down\n");
	this->level = level;
	SetAnimation(level);
}

void CGoDownState::Update(float dt)
{
	CMario* mario = CMario::GetInstance();
	if (mario->collide_with_portal == 1)
		mario->vy = -0.03;
	else if (mario->collide_with_portal == -1) {
		mario->vy = 0.03;
	}
}

void CGoDownState::SetAnimation(int level)
{
	this->level = level;
	CMario* mario = CMario::GetInstance();
	switch (level) {
	case MARIO_LEVEL_BIG:
		ani = MARIO_ANI_BIG_GO_DOWN;
		break;
	case RACCOON_LEVEL_BIG:
		ani = RACCOON_ANI_GO_DOWN;
		break;
	case FIRE_LEVEL:
		ani = FIRE_ANI_GO_DOWN;
		break;
	case MARIO_LEVEL_SMALL:
		ani = MARIO_ANI_SMALL_GO_DOWN;
		break;
	}
}

