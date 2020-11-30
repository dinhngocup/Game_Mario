#include "GoDownState.h"

CGoDownState::CGoDownState(int level)
{
	DebugOut(L"go down\n");
	this->level = level;
	SetAnimation(level);
	CMario* mario = CMario::GetInstance();
	mario->vx = 0;
}

void CGoDownState::Update(float dt)
{
	CMario* mario = CMario::GetInstance();
	DebugOut(L"start_y %f\n", start_y);

	if (mario->collide_with_portal == 1) {
		mario->vy = -0.03;
		if (mario->y + mario->h <= start_y) {
			mario->collide_with_portal = 0;
			mario->ChangeState(new CFallingState(level));
		}
	}
	else if (mario->collide_with_portal == -1) {
		mario->vy = 0.03;
		if (mario->y >= start_y) {
			
			mario->collide_with_portal = 0;
			mario->ChangeState(new CFallingState(level));
		}
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

