﻿#include "StandingState.h"

CStandingState::CStandingState()
{
	SetAnimation(level);
}

CStandingState::CStandingState(int level)
{
	CMario* mario = CMario::GetInstance();
	OutputDebugString(L"standing\n");

	this->level = level;

	mario->vx = 0;

	SetAnimation(level);


}

void CStandingState::Update(float dt)
{
	this->HandleKeyboard();
}

void CStandingState::HandleKeyboard()
{
}

void CStandingState::SetAnimation(int level)
{

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

void CStandingState::OnKeyDown(int KeyCode)
{
	CPlayerState::OnKeyDown(KeyCode);
	CMario* mario = CMario::GetInstance();
	switch (KeyCode)
	{
	case DIK_DOWN:
		if (level != MARIO_LEVEL_SMALL)
			mario->ChangeState(new CCrouchingState(level));
		break;
	case DIK_A:
		if (level == RACCOON_LEVEL_BIG) {
			mario->ChangeState(new CSpinningState(level));
		}
		else if (level == FIRE_LEVEL) {
			if (mario->number_attack == 0 || mario->number_attack == 1) {
				mario->time_start_attack = GetTickCount();
			}
			mario->number_attack++;
			DWORD count = GetTickCount();
			
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

void CStandingState::KeyState(BYTE* state)
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
		mario->ChangeState(new CCrouchingState(level));
	}
	else if (game->IsKeyDown(DIK_Z) && level == RACCOON_LEVEL_BIG) {
		mario->ChangeState(new CSpinningState(level));
	}
	
}