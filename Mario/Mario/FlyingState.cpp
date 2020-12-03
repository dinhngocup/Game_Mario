#include "FlyingState.h"

CFlyingState::CFlyingState(int level)
{
	//OutputDebugString(L"flying\n");
	CMario* mario = CMario::GetInstance();
	mario->vy = -MARIO_JUMP_SPEED_Y;
	this->level = level;
	mario->is_flying = true;
	SetAnimation(level);
}
void CFlyingState::Update(float dt)
{
	CMario* mario = CMario::GetInstance();

	// chuyen trang thai falling
	if (mario->vy == 0) {
		if (level != RACCOON_LEVEL_BIG)
			mario->ChangeState(new CRunningState(level));
		else {
			mario->ChangeState(new CFallingState(level));
		}
	}
}

void CFlyingState::HandleKeyboard()
{
}

void CFlyingState::SetAnimation(int level)
{
	this->level = level;
	CMario* mario = CMario::GetInstance();
	switch (level) {
	case MARIO_LEVEL_BIG:
		ani = MARIO_ANI_BIG_FLYING;
		break;
	case RACCOON_LEVEL_BIG:
		ani = RACCOON_ANI_FLYING;
		break;
	case FIRE_LEVEL:
		ani = FIRE_ANI_FLYING;
		break;
	case MARIO_LEVEL_SMALL:
		ani = MARIO_ANI_SMALL_FLYING;
		break;
	}
}

void CFlyingState::OnKeyDown(int KeyCode)
{
	CMario* mario = CMario::GetInstance();
	switch (KeyCode) {
	case DIK_S:
		if (level == RACCOON_LEVEL_BIG) {
			DWORD time_press_s = GetTickCount64();
			if (time_press_s - mario->time_start_jump <= 4400)
				mario->vy = MARIO_FLYING_SPEED;
			else {
				mario->ChangeState(new CFallingState(level));
			}
		}
		break;

	}
}

void CFlyingState::OnKeyUp(int KeyCode)
{

}

void CFlyingState::KeyState(BYTE* state)
{
	CGame* game = CGame::GetInstance();
	CMario* mario = CMario::GetInstance();

	if (game->IsKeyDown(DIK_S)) {
		DWORD time_press_s = GetTickCount();
		if (time_press_s - mario->time_start_jump <= 400) {
			mario->vy = -MARIO_JUMP_SPEED_Y;
		}
	}
	if (game->IsKeyDown(DIK_RIGHT)) {
		if (level == RACCOON_LEVEL_BIG)
			mario->vx = WALKING_ON_THE_AIR;
		else {
			if (mario->vx < 0)
				mario->vx = 0.2f;
		}
		mario->nx = 1;
	}
	else if (game->IsKeyDown(DIK_LEFT)) {
		if (level == RACCOON_LEVEL_BIG)
			mario->vx = -WALKING_ON_THE_AIR;
		else {
			if (mario->vx > 0)
				mario->vx = -0.2f;
		}
		mario->nx = -1;
	}
}
