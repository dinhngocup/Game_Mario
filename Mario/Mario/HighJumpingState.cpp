#include "HighJumpingState.h"

CHighJumpingState::CHighJumpingState(int level)
{
	DebugOut(L"high jump\n");
	this->level = level;
	CMario* mario = CMario::GetInstance();
	mario->vy = -MARIO_JUMP_SPEED_Y;
	SetAnimation(level);

}

void CHighJumpingState::Update(float dt)
{
	CMario* mario = CMario::GetInstance();

	// chuyen trang thai falling
	if (mario->vy >= 0) {
		// đáng lẽ phải rơi nhưng chưa render hết => bật cờ đc rơi đợi render hết mới chuyển qua state rơi
		//if (mario->is_spinning && is_rendered_completely || !mario->is_spinning) {
		if (is_rendered_completely) {
			if (level == RACCOON_LEVEL_BIG)
				mario->is_attacking_by_spinning = false;
			mario->ChangeState(new CFallingState(level));
		}

	}
	//DebugOut(L"collision spinningggg %d\n", mario->is_attacking_by_spinning);
}

void CHighJumpingState::HandleKeyboard()
{

}

void CHighJumpingState::SetAnimation(int level)
{
	this->level = level;
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

void CHighJumpingState::OnKeyDown(int KeyCode)
{
	CMario* mario = CMario::GetInstance();
	CPlayerState::OnKeyDown(KeyCode);

}

void CHighJumpingState::OnKeyUp(int KeyCode)
{
	CMario* mario = CMario::GetInstance();
	switch (KeyCode) {

	case DIK_S:
		if (mario->vy >= 0) {
			if ((ani == RACCOON_ANI_SPINNING_BIG || ani == FIRE_ANI_FLYING_THROW) && is_rendered_completely
				|| (ani != RACCOON_ANI_SPINNING_BIG && ani != FIRE_ANI_FLYING_THROW)) {
				mario->ChangeState(new CFallingState(level));
			}
		}
		break;
	}
}

void CHighJumpingState::KeyState(BYTE* state)
{
	CGame* game = CGame::GetInstance();
	CMario* mario = CMario::GetInstance();

	if (game->IsKeyDown(DIK_S)) {
		if (mario->collide_with_portal == 1) {
			mario->vx = 0;
			mario->is_underground = true;
			return;
		}
		DWORD time_press_s = GetTickCount64();
		if (time_press_s - mario->time_start_jump <= 300) {
			mario->vy = -MARIO_JUMP_SPEED_Y;
		}
	}
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

	CPlayerState::KeyState(state);
	//DebugOut(L"is render %d\n", is_rendered_completely);
}

