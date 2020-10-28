#include "CrouchingState.h"

CCrouchingState::CCrouchingState(int level)
{
	DebugOut(L"crouching\n");

	this->level = level;
	CMario* mario = CMario::GetInstance();
	mario->is_crouching = true;
	SetAnimation(level);
}

void CCrouchingState::Update(float dt)
{
	this->HandleKeyboard();
}

void CCrouchingState::HandleKeyboard()
{

}

void CCrouchingState::SetAnimation(int level)
{
	switch (level) {
	case RACCOON_LEVEL_BIG:
		ani = RACCOON_ANI_BIG_CROUCHING_RIGHT;
		break;
	case MARIO_LEVEL_BIG:
		ani = MARIO_ANI_BIG_CROUCHING_RIGHT;
		break;
	case FIRE_LEVEL:
		ani = FIRE_MARIO_ANI_CROUCHING_RIGHT;
		break;
	}
}

void CCrouchingState::OnKeyDown(int KeyCode)
{
	
	CPlayerState::OnKeyDown(KeyCode);
	CMario* mario = CMario::GetInstance();

	switch (KeyCode) {
	case DIK_LEFT:
	case DIK_RIGHT:
		mario->is_crouching = false;
		mario->SetY(mario->GetY() - DISPARITIES);

		mario->ChangeState(new CStandingState(level));
		break;
	}
}

void CCrouchingState::OnKeyUp(int KeyCode)
{
	CMario* mario = CMario::GetInstance();

	switch (KeyCode) {
	case DIK_DOWN:
		mario->is_crouching = false;
		
		mario->SetY(mario->GetY() - DISPARITIES);

		mario->ChangeState(new CStandingState(level));
		break;
	}
}

void CCrouchingState::KeyState(BYTE* states)
{
	
	CMario* mario = CMario::GetInstance();
	CGame* game = CGame::GetInstance();
	CPlayerState* player_state = mario->GetState();


	// khi vừa chuyển qua crouching, buông nút down liền, ko bắt kịp sự kiện on key down nên
	// phải có 1 cái keystate check nut down liên tục
	if (!game->IsKeyDown(DIK_DOWN)) {
		mario->is_crouching = false;
		mario->SetY(mario->GetY() - DISPARITIES);
		mario->ChangeState(new CStandingState(level));
	}
	else {
		// standing and walking
		if ((game->IsKeyDown(DIK_LEFT) || game->IsKeyDown(DIK_RIGHT)) && !game->IsKeyDown(DIK_X)) {
			mario->is_crouching = false;
			mario->SetY(mario->GetY() - DISPARITIES);
			mario->ChangeState(new CWalkingState(level));
		}
		// standing and jump and move
		else if (game->IsKeyDown(DIK_X) && (game->IsKeyDown(DIK_LEFT) || game->IsKeyDown(DIK_RIGHT))) {
			mario->is_crouching = false;
			mario->ChangeState(new CJumpingState(level));
		}
		// crouching and jump
		else if (game->IsKeyDown(DIK_X)) {
			mario->ChangeState(new CJumpingState(level));
		}

	}
}
