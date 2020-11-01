#include "WalkingState.h"


CWalkingState::CWalkingState(int level)
{
	OutputDebugString(L"walking\n");
	this->level = level;

	// tra ra animation
	SetAnimation(level);
}

void CWalkingState::Update(float dt)
{
	this->HandleKeyboard();
}

void CWalkingState::HandleKeyboard()
{

}

void CWalkingState::SetAnimation(int level)
{
	
	switch (level) {
	case RACCOON_LEVEL_BIG:
		ani = RACCOON_ANI_BIG_WALKING_RIGHT;
		break;
	case MARIO_LEVEL_BIG:
		ani = MARIO_ANI_BIG_WALKING_RIGHT;
		break;
	case MARIO_LEVEL_SMALL:
		ani = MARIO_ANI_SMALL_WALKING_RIGHT;
		break;
	case FIRE_LEVEL:
		ani = FIRE_MARIO_ANI_WALKING_RIGHT;
		break;
	}
}

void CWalkingState::OnKeyDown(int KeyCode)
{
	CPlayerState::OnKeyDown(KeyCode);
}

void CWalkingState::OnKeyUp(int KeyCode)
{
	CMario* mario = CMario::GetInstance();

	switch (KeyCode) {
	case DIK_RIGHT:
		mario->ChangeState(new CStandingState(level));
		break;
	case DIK_LEFT:
		mario->ChangeState(new CStandingState(level));
		break;
	}
}

void CWalkingState::KeyState(BYTE* state)
{
	CMario* mario = CMario::GetInstance();
	CGame* game = CGame::GetInstance();

	// xử lí việc bấm phím a right left cùng lúc thì chuyển mario về trạng thái đứng
	
	if (game->IsKeyDown(DIK_RIGHT) && game->IsKeyDown(DIK_LEFT)) {
		mario->ChangeState(new CStandingState(level));
		return;
	}
	if (game->IsKeyDown(DIK_A) && game->IsKeyDown(DIK_RIGHT) && game->IsKeyDown(DIK_LEFT)) {
		mario->ChangeState(new CStandingState(level));
		return;
	}
	else if (game->IsKeyDown(DIK_A)) {
		if (game->IsKeyDown(DIK_RIGHT)) {
			mario->nx = 1;
		}
		else if (game->IsKeyDown(DIK_LEFT)) {
			mario->nx = -1;
		}
		mario->ChangeState(new CRunningState(level));
		return;
	}
	else if (game->IsKeyDown(DIK_RIGHT))
	{
		mario->vx = MARIO_WALKING_SPEED;
		mario->nx = 1;
		return;
	}
	else if (game->IsKeyDown(DIK_LEFT)) {
		
		mario->vx = -MARIO_WALKING_SPEED;
		mario->nx = -1;
		return;
	} 
	else {
		mario->ChangeState(new CStandingState(level));
		return;
	}
	
	if (game->IsKeyDown(DIK_Z) && level == RACCOON_LEVEL_BIG) {
		mario->ChangeState(new CSpinningState(level));
		return;
	}


}
