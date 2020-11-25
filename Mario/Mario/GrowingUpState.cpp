#include "GrowingUpState.h"

CGrowingUpState::CGrowingUpState(int level)
{
	CMario* mario = CMario::GetInstance();
	this->level = level;
	SetAnimation(this->level);
	start_ani1 = GetTickCount64();
	start_ani2 = GetTickCount64();
	if (level == MARIO_LEVEL_SMALL)
		limit = 1100;
	else limit = 700;
}

void CGrowingUpState::Update(float dt)
{
	CMario* mario = CMario::GetInstance();
	if (GetTickCount64() - start_ani1 >= 1800) {
		mario->untouchable = 0;
		int level_mario = mario->GetLevel();
		if (level == MARIO_LEVEL_SMALL) {
			level_mario++;
		}
		else {
			level_mario--;
		}
		mario->SetLevel(level_mario);
		mario->SetState(MARIO_STATE_IDLE);
		if (abs(mario->vx) > MARIO_WALKING_SPEED)
			mario->ChangeState(new CRunningState(level_mario));
		else if (abs(mario->vx) == MARIO_WALKING_SPEED)
			mario->ChangeState(new CWalkingState(level_mario));
		else
			mario->ChangeState(new CStandingState(level_mario));
	}
	else if (GetTickCount64() - start_ani1 <= limit) {
		if (GetTickCount64() - start_ani2 >= 200) {
			start_ani2 = GetTickCount64();
			if (level == MARIO_LEVEL_SMALL) {
				if (mario->state == MARIO_STATE_SMALL_GROW_UP) {
					mario->SetState(MARIO_STATE_BIG_GROW_UP);
					mario->y -= 12;

				}
				else if (mario->state == MARIO_STATE_BIG_GROW_UP) {
					mario->SetState(MARIO_STATE_SMALL_GROW_UP);
					mario->y += 12;
				}

			}
			else {
				if (mario->state == MARIO_STATE_BIG_END_GROW_UP) {
					mario->SetState(MARIO_STATE_BIG_GROW_UP);
					mario->y += 22;

				}
				else if (mario->state == MARIO_STATE_BIG_GROW_UP) {
					mario->SetState(MARIO_STATE_BIG_END_GROW_UP);
					mario->y -= 24;
				}
			}

		}
	}
	else {
		if (GetTickCount64() - start_ani2 >= 200) {
			start_ani2 = GetTickCount64();
			if (level == MARIO_LEVEL_SMALL) {
				if (mario->state == MARIO_STATE_BIG_END_GROW_UP) {
					mario->SetState(MARIO_STATE_BIG_GROW_UP);
					mario->y += 24;

				}
				else if (mario->state == MARIO_STATE_BIG_GROW_UP) {
					mario->SetState(MARIO_STATE_BIG_END_GROW_UP);
					mario->y -= 24;
				}

			}
			else {
				
				if (mario->state == MARIO_STATE_SMALL_GROW_UP) {
					mario->SetState(MARIO_STATE_BIG_GROW_UP);
					mario->y -= 12;

				}
				else if (mario->state == MARIO_STATE_BIG_GROW_UP) {
					mario->SetState(MARIO_STATE_SMALL_GROW_UP);
					mario->y += 12;
				}
			}
		}
	}
}

void CGrowingUpState::SetAnimation(int level)
{
	CMario* mario = CMario::GetInstance();
	if (level == MARIO_LEVEL_SMALL)
		mario->SetState(MARIO_STATE_SMALL_GROW_UP);
	else
		mario->SetState(MARIO_STATE_BIG_END_GROW_UP);

}
