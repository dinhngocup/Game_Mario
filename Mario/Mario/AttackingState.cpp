#include "AttackingState.h"

CAttackingState::CAttackingState(int level)
{
	DebugOut(L"Attacking\n");
	this->level = level;
	SetAnimation();
}

void CAttackingState::SetAnimation()
{
	ani = FIRE_MARIO_ANI_THROW;
}

void CAttackingState::Update(float dt)
{
	CMario* mario = CMario::GetInstance();
	
}

void CAttackingState::HandleKeyboard()
{

}

void CAttackingState::KeyState(BYTE* state)
{
	CMario* mario = CMario::GetInstance();
	CGame* game = CGame::GetInstance();
	if (!animation->NextIsLastFrame() && animation->IsLastFrame()) {
		mario->is_attacking = true;
		is_done = true;
	}
	if (is_done) {
		
		if (game->IsKeyDown(DIK_A) && (game->IsKeyDown(DIK_LEFT) || game->IsKeyDown(DIK_RIGHT))) {
			mario->ChangeState(new CRunningState(level));
		}
		else
			mario->ChangeState(new CStandingState(level));
	}
}
