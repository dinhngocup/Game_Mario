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
	// Tổng có 3 frame, khi render đến frame cuối mà change state liền thì hong đẹp
	// phải cho render frame cuối xong, quay về frame đầu, render rồi đổi luôn trạng thái
	// lúc xét va chạm thì tới khúc animation.nextislastframe = true xxets va chạm là dc
	// còn render gì kệ nó

	// nếu render frame đầu xong mới đổi state thì current frame ở lần tiếp theo là 1, nó bị hụt 1 frame 0
	// do đó phải reset frame về -1 như mới thì nó mới render frame 0
	// vậy  nên hong?

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

	if (is_done) {
		// Đè nút A và phím qua trái phải thì chạy nhanh
		if (game->IsKeyDown(DIK_A) && (game->IsKeyDown(DIK_RIGHT) || game->IsKeyDown(DIK_LEFT))) {
			mario->ChangeState(new CRunningState(level));
		}
		else if (game->IsKeyDown(DIK_Z)) {
			if (game->IsKeyDown(DIK_RIGHT)) {
				mario->vx = MARIO_WALKING_SPEED;
				mario->nx = 1;
			}
			else if (game->IsKeyDown(DIK_LEFT)) {
				mario->vx = -MARIO_WALKING_SPEED;
				mario->nx = -1;
			}
		}
		else {
			// chuyển về standing
			mario->is_attacking_by_spinning = false;
			mario->ChangeState(new CStandingState(level));
		}

	}
	// bấm phím Z thì xoay liên tục
	/*if (game->IsKeyDown(DIK_Z) && (game->IsKeyDown(DIK_RIGHT) || game->IsKeyDown(DIK_LEFT))) {
		SetAnimation();
	}*/
	DebugOut(L"spinning%d\n", mario->is_attacking_by_spinning);

}

void CSpinningState::CheckState()
{
	int current_frame = animation->GetCurrentFrame();
	//DebugOut(L"crr %d\n", current_frame);
	CMario* mario = CMario::GetInstance();
	if (current_frame == 0 || current_frame == 4 || current_frame == 2)
		mario->is_attacking_by_spinning = true;
	else
		mario->is_attacking_by_spinning = false;
	if (!animation->NextIsLastFrame() && animation->IsLastFrame()) {
		is_done = true;
	}

}

