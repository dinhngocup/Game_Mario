#include "SpinningState.h"

CSpinningState::CSpinningState(int level)
{
	CMario* mario = CMario::GetInstance();

	this->level = level;
	SetAnimation();
	start_ani = GetTickCount64();
	DebugOut(L"spinning\n");
	start_count = GetTickCount64();
	mario->SetState(MARIO_STATE_ATTACKING);
}

void CSpinningState::Update(float dt)
{
	CMario* mario = CMario::GetInstance();
	CheckState();
	/*int current_frame = animation->GetCurrentFrame();
	
	DebugOut(L"frame %d\n", current_frame);
	DebugOut(L"is_attacking %d\n", mario->is_attacking_by_spinning);*/

	if (is_rendered_completely) {
		mario->is_attacking_by_spinning = false;
		mario->ChangeState(new CStandingState(level));
	}

}

void CSpinningState::HandleKeyboard()
{

}

void CSpinningState::SetAnimation()
{
	ani = RACCOON_ANI_SPINNING_BIG;
}

void CSpinningState::OnKeyDown(int KeyCode)
{
	CPlayerState::OnKeyDown(KeyCode);
}

void CSpinningState::KeyState(BYTE* state)
{
	CMario* mario = CMario::GetInstance();
	CGame* game = CGame::GetInstance();

	CheckState();
	if (is_rendered_completely) {
		start_ani = GetTickCount64();
		// Đè nút A và phím qua trái phải thì chạy nhanh
		if (game->IsKeyDown(DIK_A) && (game->IsKeyDown(DIK_RIGHT) || game->IsKeyDown(DIK_LEFT))) {
			mario->is_attacking_by_spinning = false;
			mario->ChangeState(new CRunningState(level));
		}
		else if (game->IsKeyDown(DIK_Z)) {
			// Phải reset flag khi render completely
			animation->ResetFlagLastFrame();
			CPlayerState::SetAnimation(mario->animation_set->at(ani));
			if (game->IsKeyDown(DIK_RIGHT)) {
				mario->vx = MARIO_WALKING_SPEED;
				mario->nx = 1;
			}
			else if (game->IsKeyDown(DIK_LEFT)) {
				mario->vx = -MARIO_WALKING_SPEED;
				mario->nx = -1;
			}
			else if (!game->IsKeyDown(DIK_LEFT) && !game->IsKeyDown(DIK_RIGHT)) {
				mario->is_attacking_by_spinning = false;
				mario->vx = 0;
				mario->ChangeState(new CStandingState(level));
			}
		}
		else if (game->IsKeyDown(DIK_LEFT) || game->IsKeyDown(DIK_RIGHT)) {
			mario->is_attacking_by_spinning = false;
			mario->ChangeState(new CWalkingState(level));
		}
		else {
			// chuyển về standing
			mario->is_attacking_by_spinning = false;
			mario->ChangeState(new CStandingState(level));
		}

	}
	

}

void CSpinningState::CheckState()
{
	CMario* mario = CMario::GetInstance();
	if (GetTickCount64() - start_ani >= TIME_SPINNING)
		is_rendered_completely = true;
	else {
		is_rendered_completely = false;
		if (GetTickCount64() - start_count >= TIME_CONVERT_SPINNING_STATE) {
			if (mario->state == MARIO_STATE_ATTACKING)
				mario->SetState(MARIO_STATE_NO_ATTACKING);
			else if(mario->state == MARIO_STATE_NO_ATTACKING)
				mario->SetState(MARIO_STATE_ATTACKING);
			start_count = GetTickCount64();
		}
	}

}


