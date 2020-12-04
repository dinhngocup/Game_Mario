#include "FallingState.h"

CFallingState::CFallingState(int level)
{
	//OutputDebugString(L"falling\n");
	CMario* mario = CMario::GetInstance();

	this->level = level;
	mario->collide_with_portal = 0;
	SetAnimation(level);
}

void CFallingState::Update(float dt)
{
	CMario* mario = CMario::GetInstance();
	CGame* game = CGame::GetInstance();
	/*if (is_falling_slowly) {
		mario->vy -= 0.02 * dt;
		DebugOut(L"vy sau khi bam roi cham %f\n", mario->vy);
		is_falling_slowly = false;
	}*/
	// chuyen trang thai standing
	if (mario->vy == 0) {
		if (is_rendered_completely) {
			if (level == RACCOON_LEVEL_BIG)
				mario->is_attacking_by_spinning = false;
			if (mario->is_crouching)
				mario->ChangeState(new CCrouchingState(level));
			else if (abs(mario->vx) > MARIO_WALKING_SPEED) {
				if (level == RACCOON_LEVEL_BIG) {
					mario->vx = 0.4f;
				}
				mario->ChangeState(new CRunningState(level));

			}
			else {
				mario->ChangeState(new CStandingState(level));

			}
		}
	}
	//DebugOut(L"collision spinningggg %d\n", mario->is_attacking_by_spinning);

}

void CFallingState::HandleKeyboard()
{

}

void CFallingState::SetAnimation(int level)
{
	this->level = level;
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
	switch (KeyCode)
	{
	case DIK_S:
	case DIK_X:
		time_start_to_press = GetTickCount64();
		break;

	}
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
	if (level == RACCOON_LEVEL_BIG) {
		if (game->IsKeyDown(DIK_S) || game->IsKeyDown(DIK_X)) {
			DWORD now = GetTickCount64();
			if (mario->vy > 0 && now - time_start_to_press < 300)
				mario->vy += -mario->vy * 0.5f;
		}
	}
}

void CFallingState::CheckState()
{
	CMario* mario = CMario::GetInstance();
	if (level == FIRE_LEVEL) {
		if (GetTickCount64() - start_ani >= 400) {
			is_rendered_completely = true;
			mario->is_attacking = true;
		}
		else
			is_rendered_completely = false;
	}
	else if (level == RACCOON_LEVEL_BIG) {
		if (GetTickCount64() - start_ani >= 600)
			is_rendered_completely = true;
		else {
			is_rendered_completely = false;
			if (GetTickCount64() - start_count >= 120) {
				if (mario->state == MARIO_STATE_ATTACKING)
					mario->SetState(MARIO_STATE_NO_ATTACKING);
				else if (mario->state == MARIO_STATE_NO_ATTACKING)
					mario->SetState(MARIO_STATE_ATTACKING);
				start_count = GetTickCount64();
			}
		}
	}
}
