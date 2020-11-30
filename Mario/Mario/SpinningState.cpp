#include "SpinningState.h"

CSpinningState::CSpinningState(int level)
{
	this->level = level;
	SetAnimation();

	DebugOut(L"spinning\n");
}

void CSpinningState::Update(float dt)
{
	CMario* mario = CMario::GetInstance();
	

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

	//CheckState();
	CPlayerState::CheckState();
	if (is_rendered_completely) {
		// Đè nút A và phím qua trái phải thì chạy nhanh
		if (game->IsKeyDown(DIK_A) && (game->IsKeyDown(DIK_RIGHT) || game->IsKeyDown(DIK_LEFT))) {
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


