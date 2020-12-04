#include "GrowingUpState.h"
#include "PlayScene.h"

CGrowingUpState::CGrowingUpState(int level)
{
	CMario* mario = CMario::GetInstance();
	CGame* game = CGame::GetInstance();
	this->level = level;
	SetAnimation(this->level);
	start_ani1 = GetTickCount64();
	start_ani2 = GetTickCount64();
	if (level == MARIO_LEVEL_SMALL)
		limit = TIME_LIMIT_FROM_SMALL_TO_BIG;
	else limit = TIME_LIMIT_FROM_BIG_TO_SMALL;
	CPlayScene* scene = (CPlayScene*)game->GetCurrentScene();
	scene->time_scale = 0;
	mario->unhide_start = 0;
}

void CGrowingUpState::Update(float dt)
{
	CMario* mario = CMario::GetInstance();
	CGame* game = CGame::GetInstance();
	CPlayScene* scene = (CPlayScene*)game->GetCurrentScene();

	if (GetTickCount64() - start_ani1 >= TIME_GROWING_UP) {
		
		int level_mario = mario->GetLevel();
		if (mario->unhide_start == 0) {
			scene->time_scale = 1;
			if (level == MARIO_LEVEL_SMALL) {
				level_mario++;
			}
			else {
				level_mario--;
			}
			mario->SetLevel(level_mario);
			if (level_mario == MARIO_LEVEL_BIG) {
				mario->untouchable = 0;
				mario->SetState(MARIO_STATE_IDLE);
				mario->ChangeState(new CStandingState(level_mario));
				return;
			}
			mario->unhide_start = GetTickCount64();
			mario->untouchable_start = GetTickCount64();
			mario->SetState(MARIO_END_GROWING);
			mario->ChangeState(new CStandingState(level_mario));

		}
		/*if (GetTickCount64() - mario->untouchable_start <= 2000) {

		if (GetTickCount64() - mario->unhide_start >= 100 && mario->unhide_start !=0) {
			if (mario->state == MARIO_STATE_HIDE_UNTOUCHABLE)
				mario->SetState(MARIO_STATE_UNHIDE_UNTOUCHABLE);
			else
				mario->SetState(MARIO_STATE_HIDE_UNTOUCHABLE);
			mario->unhide_start = GetTickCount64();
		}
		
		}
		else {
			mario->untouchable = 0;
			mario->ChangeState(new CStandingState(level_mario));
		}*/

	}
	else if (GetTickCount64() - start_ani1 <= limit) {
		if (GetTickCount64() - start_ani2 >= TIME_CHANGE_STATE_WITHIN_GROWING_UP) {
			start_ani2 = GetTickCount64();
			if (level == MARIO_LEVEL_SMALL) {
				if (mario->state == MARIO_STATE_SMALL_GROW_UP) {
					mario->SetState(MARIO_STATE_BIG_GROW_UP);
					mario->y -= DISPARITY_SMALL_TO_MIDDLE_MARIO;

				}
				else if (mario->state == MARIO_STATE_BIG_GROW_UP) {
					mario->SetState(MARIO_STATE_SMALL_GROW_UP);
					mario->y += DISPARITY_SMALL_TO_MIDDLE_MARIO;
				}

			}
			else {
				if (mario->state == MARIO_STATE_BIG_END_GROW_UP) {
					mario->SetState(MARIO_STATE_BIG_GROW_UP);
					mario->y += DISPARITY_BIG_TO_MIDDLE_MARIO - 2;

				}
				else if (mario->state == MARIO_STATE_BIG_GROW_UP) {
					mario->SetState(MARIO_STATE_BIG_END_GROW_UP);
					mario->y -= DISPARITY_BIG_TO_MIDDLE_MARIO;
				}
			}

		}
	}
	else {
		if (GetTickCount64() - start_ani2 >= TIME_CHANGE_STATE_WITHIN_GROWING_UP) {
			start_ani2 = GetTickCount64();
			if (level == MARIO_LEVEL_SMALL) {
				if (mario->state == MARIO_STATE_BIG_END_GROW_UP) {
					mario->SetState(MARIO_STATE_BIG_GROW_UP);
					mario->y += DISPARITY_BIG_TO_MIDDLE_MARIO;

				}
				else if (mario->state == MARIO_STATE_BIG_GROW_UP) {
					mario->SetState(MARIO_STATE_BIG_END_GROW_UP);
					mario->y -= DISPARITY_BIG_TO_MIDDLE_MARIO;
				}

			}
			else {
				
				if (mario->state == MARIO_STATE_SMALL_GROW_UP) {
					mario->SetState(MARIO_STATE_BIG_GROW_UP);
					mario->y -= DISPARITY_SMALL_TO_MIDDLE_MARIO;

				}
				else if (mario->state == MARIO_STATE_BIG_GROW_UP) {
					mario->SetState(MARIO_STATE_SMALL_GROW_UP);
					mario->y += DISPARITY_SMALL_TO_MIDDLE_MARIO;
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
