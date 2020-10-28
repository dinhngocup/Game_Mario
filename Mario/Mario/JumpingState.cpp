#include "JumpingState.h"

CJumpingState::CJumpingState()
{
}

CJumpingState::CJumpingState(int level)
{
	OutputDebugString(L"jump\n");

	this->level = level;

	CMario* mario = CMario::GetInstance();
	mario->vy = -MARIO_JUMP_SPEED_Y;

	SetAnimation(level);
}

void CJumpingState::Update(float dt)
{
	CMario* mario = CMario::GetInstance();

	if (mario->vy > 0) mario->ChangeState(new CFallingState(level));
}

void CJumpingState::HandleKeyboard()
{


}

void CJumpingState::SetAnimation(int level)
{
	CMario* mario = CMario::GetInstance();
	switch (level) {
	case MARIO_LEVEL_BIG:
		if (!mario->is_crouching)
			ani = MARIO_ANI_BIG_JUMPING_RIGHT;
		else
			ani = MARIO_ANI_BIG_CROUCHING_RIGHT;
		break;
	case MARIO_LEVEL_SMALL:
		ani = MARIO_ANI_SMALL_JUMPING_RIGHT;
		break;
	case RACCOON_LEVEL_BIG:
		if (!mario->is_crouching)
			ani = RACCOON_ANI_BIG_JUMPING_RIGHT;
		else
			ani = RACCOON_ANI_BIG_CROUCHING_RIGHT;
		break;
	case FIRE_LEVEL:
		if (!mario->is_crouching)
			ani = FIRE_MARIO_ANI_JUMPING_RIGHT;
		else
			ani = FIRE_MARIO_ANI_CROUCHING_RIGHT;
		break;
	}
}

void CJumpingState::OnKeyDown(int KeyCode)
{
	CMario* mario = CMario::GetInstance();
	switch (KeyCode) {
	case DIK_A:
		if (level == RACCOON_LEVEL_BIG)
			mario->ChangeState(new CSpinningState(RACCOON_LEVEL_BIG));
		break;

	}
}

void CJumpingState::KeyState(BYTE* state)
{
	CGame* game = CGame::GetInstance();
	CMario* mario = CMario::GetInstance();
	
	if (game->IsKeyDown(DIK_X)) {
		if (game->IsKeyDown(DIK_RIGHT)) {
			mario->vx = MARIO_WALKING_SPEED;
			mario->nx = 1;
		}
		else if (game->IsKeyDown(DIK_LEFT)) {
			mario->vx = -MARIO_WALKING_SPEED;
			mario->nx = -1;
		}
	}
	else if (game->IsKeyDown(DIK_Z) && level == RACCOON_LEVEL_BIG) {
		mario->ChangeState(new CSpinningState(level));
	}
}

