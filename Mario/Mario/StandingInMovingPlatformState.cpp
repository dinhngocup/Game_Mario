﻿#include "StandingInMovingPlatformState.h"

CStandingInMovingPlatformState::CStandingInMovingPlatformState(int level)
{
	CMario* mario = CMario::GetInstance();
	OutputDebugString(L"standing in moving platform\n");
	mario->is_attacking_by_spinning = false;
	this->level = level;

	mario->vx = 0;
	SetAnimation(level);
}

void CStandingInMovingPlatformState::Update(float dt)
{
	CMario* mario = CMario::GetInstance();
	//DebugOut(L"vy %f\n", mario->vy);
		
}

void CStandingInMovingPlatformState::HandleKeyboard()
{
}

void CStandingInMovingPlatformState::SetAnimation(int level)
{
	this->level = level;
	CMario* mario = CMario::GetInstance();
	switch (level) {
	case MARIO_LEVEL_BIG:
		if (!mario->is_crouching)
			ani = MARIO_ANI_BIG_IDLE_RIGHT;
		else
			ani = MARIO_ANI_BIG_CROUCHING_RIGHT;
		break;
	case RACCOON_LEVEL_BIG:
		if (!mario->is_crouching)
			ani = RACCOON_ANI_BIG_IDLE_RIGHT;
		else
			ani = RACCOON_ANI_BIG_CROUCHING_RIGHT;
		break;
	case MARIO_LEVEL_SMALL:
		ani = MARIO_ANI_SMALL_IDLE_RIGHT;
		break;
	case FIRE_LEVEL:
		ani = FIRE_MARIO_ANI_IDLE_RIGHT;
		break;
	}
}

void CStandingInMovingPlatformState::OnKeyDown(int KeyCode)
{
	CPlayerState::OnKeyDown(KeyCode);
	CMario* mario = CMario::GetInstance();
	if (mario->untouchable == 1)
		return;
	switch (KeyCode)
	{
	case DIK_DOWN:
		//DebugOut(L"collide_with_portal %d\n", mario->collide_with_portal);
		if (mario->collide_with_portal == 0) {
			if (level != MARIO_LEVEL_SMALL)
				mario->ChangeState(new CCrouchingState(level));
		}
		else if (mario->collide_with_portal == -1)
			mario->is_underground = true;
		break;

	case DIK_A:
		if (level == RACCOON_LEVEL_BIG) {
			mario->ChangeState(new CSpinningState(level));
		}
		else if (level == FIRE_LEVEL) {
			if (mario->number_attack == 0 || mario->number_attack == 1) {
				mario->time_start_attack = GetTickCount64();
			}
			mario->number_attack++;
			DWORD count = GetTickCount64();

			if (count - mario->time_start_attack <= 800 && mario->number_attack <= 2) {
				mario->ChangeState(new CAttackingState(level));

			}
			else if (count - mario->time_start_attack > 800) {
				mario->number_attack = 1;
				mario->ChangeState(new CAttackingState(level));

			}
		}
		break;
	}
}

void CStandingInMovingPlatformState::OnKeyUp(int KeyCode)
{
}

void CStandingInMovingPlatformState::KeyState(BYTE* states)
{
	CMario* mario = CMario::GetInstance();
	CGame* game = CGame::GetInstance();

	// dùng để lock việc vừa bấm A vừa bấm left right, giảm vận tốc cho đến khi bằng 0 rồi lock vx = 0 luôn
	if (game->IsKeyDown(DIK_RIGHT) && game->IsKeyDown(DIK_LEFT)) {
		mario->vx = 0;
		return;
	}
	if (game->IsKeyDown(DIK_X)) {
		mario->ChangeState(new CJumpingState(level));
	}
	else if (game->IsKeyDown(DIK_RIGHT) || game->IsKeyDown(DIK_LEFT)) {
		mario->ChangeState(new CWalkingState(level));
	}
	else if (game->IsKeyDown(DIK_DOWN) && level != MARIO_LEVEL_SMALL) {
		if (mario->collide_with_portal == 0) {
			mario->ChangeState(new CCrouchingState(level));
		}
		else if (mario->collide_with_portal == -1)
			mario->is_underground = true;

	}
	else if (game->IsKeyDown(DIK_Z)) {
		if (level == RACCOON_LEVEL_BIG)
			mario->ChangeState(new CSpinningState(level));
		else if (level == FIRE_LEVEL) {
			if (GetTickCount64() - mario->start_press_z >= TIME_BLOCK_PRESS_Z) {
				mario->press_z = true;
				mario->ChangeState(new CAttackingState(level));
				mario->start_press_z = GetTickCount64();
			}
		}
	}
}
