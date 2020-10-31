#include "FallingState.h"

CFallingState::CFallingState(int level)
{
	OutputDebugString(L"falling\n");
	CMario* mario = CMario::GetInstance();

	this->level = level;

	SetAnimation(level);
}

void CFallingState::Update(float dt)
{
	CMario* mario = CMario::GetInstance();
	CGame* game = CGame::GetInstance();

	// chuyen trang thai standing
	if (mario->vy == 0) {
		if ((mario->is_spinning && is_rendered_completely) || !mario->is_spinning) {
			mario->is_attacking_by_spinning = false;
			if (mario->is_crouching)
				mario->ChangeState(new CCrouchingState(level));
			else if (abs(mario->vx) > MARIO_WALKING_SPEED) {
				DWORD time_end_jump = GetTickCount();
				DWORD dt_from_jump_to_fall = time_end_jump - mario->time_start_jump;
				float speed_x = abs(mario->vx);
				mario->vx = (speed_x - MARIO_ACCELERATION * (dt_from_jump_to_fall/1000) * 2) * mario->nx;
				mario->ChangeState(new CRunningState(level));

			}
			else
				mario->ChangeState(new CStandingState(level));
		}
	}
	//DebugOut(L"collision spinningggg %d\n", mario->is_attacking_by_spinning);

}

void CFallingState::HandleKeyboard()
{

}

void CFallingState::SetAnimation(int level)
{
	CMario* mario = CMario::GetInstance();
	switch (level) {
	case MARIO_LEVEL_BIG:
		if (!mario->is_crouching)
			ani = MARIO_ANI_FALLING_BIG;
		else
			ani = MARIO_ANI_BIG_CROUCHING_RIGHT;
		break;
	case RACCOON_LEVEL_BIG:
		if (!mario->is_crouching)
			ani = RACCOON_ANI_FALLING_BIG;
		else
			ani = RACCOON_ANI_BIG_CROUCHING_RIGHT;
		break;
	case FIRE_LEVEL:
		if (!mario->is_crouching)
			ani = FIRE_MARIO_ANI_FALLING;
		else
			ani = FIRE_MARIO_ANI_CROUCHING_RIGHT;
		break;
	case MARIO_LEVEL_SMALL:
		ani = MARIO_ANI_FALLING_SMALL;
		break;
	}
}

void CFallingState::OnKeyDown(int KeyCode)
{
	CMario* mario = CMario::GetInstance();
	CPlayerState::OnKeyDown(KeyCode);
}

void CFallingState::OnKeyUp(int KeyCode)
{

}

void CFallingState::KeyState(BYTE* state)
{
	CGame* game = CGame::GetInstance();
	CMario* mario = CMario::GetInstance();
	CPlayerState::KeyState(state);
	if (game->IsKeyDown(DIK_RIGHT)) {
		if (abs(mario->vx) <= MARIO_WALKING_SPEED)
			mario->vx = MARIO_WALKING_SPEED;
		mario->nx = 1;
	}
	else if (game->IsKeyDown(DIK_LEFT)) {
		if (abs(mario->vx) <= MARIO_WALKING_SPEED)
			mario->vx = -MARIO_WALKING_SPEED;
		mario->nx = -1;
	}
}
