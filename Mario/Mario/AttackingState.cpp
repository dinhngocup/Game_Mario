#include "AttackingState.h"

CAttackingState::CAttackingState(int level)
{
	//DebugOut(L"Attacking\n");
	this->level = level;
	SetAnimation();
	start_ani = GetTickCount64();
}

void CAttackingState::SetAnimation()
{
	ani = FIRE_MARIO_ANI_THROW;
}

void CAttackingState::Update(float dt)
{
	CMario* mario = CMario::GetInstance();
	CheckState();
	/*int current_frame = animation->GetCurrentFrame();

	DebugOut(L"frame %d\n", current_frame);
	DebugOut(L"is_attacking %d\n", mario->is_attacking);*/

	if (is_rendered_completely) {
		mario->is_attacking = true;
	}
}

void CAttackingState::HandleKeyboard()
{

}

void CAttackingState::KeyState(BYTE* state)
{
	CMario* mario = CMario::GetInstance();
	CGame* game = CGame::GetInstance();
	CheckState();
	if (is_rendered_completely) {
		mario->is_attacking = true;
		if (game->IsKeyDown(DIK_A) && (game->IsKeyDown(DIK_LEFT) || game->IsKeyDown(DIK_RIGHT))) {
			mario->ChangeState(new CRunningState(level));
		}
		else if (game->IsKeyDown(DIK_LEFT) || game->IsKeyDown(DIK_RIGHT))
			mario->ChangeState(new CWalkingState(level));
		else
			mario->ChangeState(new CStandingState(level));
	}
}

void CAttackingState::CheckState()
{
	CMario* mario = CMario::GetInstance();
	if (GetTickCount64() - start_ani >= 400)
		is_rendered_completely = true;
	else
		is_rendered_completely = false;
}


