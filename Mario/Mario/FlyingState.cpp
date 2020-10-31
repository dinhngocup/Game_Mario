#include "FlyingState.h"

CFlyingState::CFlyingState(int level)
{
	OutputDebugString(L"flying\n");
	CMario* mario = CMario::GetInstance();
	mario->vy = -MARIO_JUMP_SPEED_Y;
	this->level = level;

	SetAnimation(level);
}
void CFlyingState::Update(float dt)
{
	CMario* mario = CMario::GetInstance();

	// chuyen trang thai falling
	if (mario->vy == 0) {
		mario->ChangeState(new CRunningState(level));
	}
}

void CFlyingState::HandleKeyboard()
{
}

void CFlyingState::SetAnimation(int level)
{
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
			mario->vy = -MARIO_JUMP_SPEED_Y;
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
		if (time_press_s - mario->time_start_jump <= 300) {
			DebugOut(L"< hon 300ms\n");
			mario->vy = -MARIO_JUMP_SPEED_Y;
		}
		else {
			DebugOut(L"lon hon 300ms\n");
			
		}
	}
	if (game->IsKeyDown(DIK_RIGHT)) {
		if (mario->vx < 0)
			mario->vx = 0.2f;
		mario->nx = 1;
	}
	else if (game->IsKeyDown(DIK_LEFT)) {
		if (mario->vx > 0)
			mario->vx = -0.2f;
		mario->nx = -1;
	}
}
