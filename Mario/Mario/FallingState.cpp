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
	
	
	/*if (able_to_change && is_rendered_completely) {
		mario->is_attacking_by_spinning = false;
		if (mario->is_crouching)
			mario->ChangeState(new CCrouchingState(level));
		else
			mario->ChangeState(new CStandingState(level));
		return;
	}*/
	// chuyen trang thai standing
	if (mario->vy == 0) {
		if (ani == RACCOON_ANI_SPINNING_BIG && is_rendered_completely || ani != RACCOON_ANI_SPINNING_BIG) {
			mario->is_attacking_by_spinning = false;
			if (mario->is_crouching)
				mario->ChangeState(new CCrouchingState(level));
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
	/*CMario* mario = CMario::GetInstance();
	switch (KeyCode) {
	case DIK_A:
		if (level == RACCOON_LEVEL_BIG)
			mario->ChangeState(new CSpinningState(RACCOON_LEVEL_BIG));
		break;

	}*/
}

void CFallingState::OnKeyUp(int KeyCode)
{

}

void CFallingState::KeyState(BYTE* state)
{
	CGame* game = CGame::GetInstance();
	CMario* mario = CMario::GetInstance();

	// Vẫn giữ nguyên state high jump nhưng chuyển ani
	if (is_rendered_completely) {
		if (game->IsKeyDown(DIK_Z) && level == RACCOON_LEVEL_BIG) {
			ani = RACCOON_ANI_SPINNING_BIG;
			mario->animation_set->at(ani)->ResetFlagLastFrame();
			CPlayerState::SetAnimation(mario->animation_set->at(ani));
		}
		else if (!game->IsKeyDown(DIK_Z) && level == RACCOON_LEVEL_BIG)
			ani = RACCOON_ANI_FALLING_BIG;
	}
	if (ani == RACCOON_ANI_SPINNING_BIG) {
		CheckState();
	}
}

void CFallingState::CheckState()
{
	int current_frame = animation->GetCurrentFrame();
	CMario* mario = CMario::GetInstance();
	DebugOut(L"current %d\n", current_frame);
	if (current_frame == 0 || current_frame == 4 || current_frame == 2)
		mario->is_attacking_by_spinning = true;
	else
		mario->is_attacking_by_spinning = false;

	// phải render full frame mới được bật cờ is done
	if (!animation->NextIsLastFrame() && animation->IsLastFrame()) {
		is_rendered_completely = true;
	}
	else is_rendered_completely = false;
}
