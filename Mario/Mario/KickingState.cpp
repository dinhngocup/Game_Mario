#include "KickingState.h"

CKickingState::CKickingState(int level)
{
	//DebugOut(L"kicking\n");

	this->level = level;
	SetAnimation(level);
	start_ani = GetTickCount64();
}

void CKickingState::Update(float dt)
{
	CMario* mario = CMario::GetInstance();

	CheckState();
	if (is_rendered_completely) {
		mario->ChangeState(new CStandingState(level));
	}
}

void CKickingState::HandleKeyboard()
{
}

void CKickingState::SetAnimation(int level)
{
	this->level = level;
	switch (level) {
	case RACCOON_LEVEL_BIG:
		ani = RACCOON_ANI_KICK;
		break;
	case MARIO_LEVEL_BIG:
		ani = MARIO_ANI_BIG_KICK;
		break;
	case FIRE_LEVEL:
		ani = FIRE_ANI_KICK;
		break;
	case MARIO_LEVEL_SMALL:
		ani = MARIO_ANI_SMALL_KICK;
		break;
	}
}

void CKickingState::OnKeyDown(int KeyCode)
{
}

void CKickingState::OnKeyUp(int KeyCode)
{
}

void CKickingState::KeyState(BYTE* states)
{
	
}

void CKickingState::CheckState()
{
	if (GetTickCount64() - start_ani >= TIME_KICKING)
		is_rendered_completely = true;
	else {
		is_rendered_completely = false;
	}
}
