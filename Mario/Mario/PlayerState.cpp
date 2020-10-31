#include "PlayerState.h"
#include "Mario.h"
#include "Game.h"
void CPlayerState::OnKeyDown(int KeyCode)
{
	CMario* mario = CMario::GetInstance();
	float current_x = mario->GetX();
	//DebugOut(L"key downnn \n");
	//DebugOut(L"keycode %d\n", KeyCode);
	CPlayerState* player_state = mario->GetState();
	switch (KeyCode)
	{
	case DIK_X:
		if (!dynamic_cast<CJumpingState*>(player_state) &&
			!dynamic_cast<CHighJumpingState*>(player_state) &&
			!dynamic_cast<CFallingState*>(player_state)) {
			mario->time_start_jump = GetTickCount();

			mario->ChangeState(new CJumpingState(level));
		}
		break;
	case DIK_S:
		if (!dynamic_cast<CJumpingState*>(player_state) &&
			!dynamic_cast<CHighJumpingState*>(player_state) &&
			!dynamic_cast<CFallingState*>(player_state)) {
			DebugOut(L"change to high jump\n");
			mario->time_start_jump = GetTickCount();
			mario->ChangeState(new CHighJumpingState(level));
		}
		break;
	case DIK_A:
		if (dynamic_cast<CJumpingState*>(player_state) ||
			dynamic_cast<CHighJumpingState*>(player_state) ||
			dynamic_cast<CFallingState*>(player_state)) {
			if (level == RACCOON_LEVEL_BIG) {
				if (is_rendered_completely) {
					ani = RACCOON_ANI_SPINNING_BIG;
					mario->animation_set->at(ani)->ResetFlagLastFrame();
					CPlayerState::SetAnimation(mario->animation_set->at(ani));
				}
				if (ani == RACCOON_ANI_SPINNING_BIG) {
					CheckState();
				}
			}
		}
		break;
	case DIK_1:
		mario->SetPosition(current_x, 450);
		mario->SetSpeed(0, 0);
		mario->SetLevel(MARIO_LEVEL_SMALL);
		mario->ChangeState(new CStandingState(MARIO_LEVEL_SMALL));
		break;
	case DIK_2:
		mario->SetPosition(current_x, 450);
		mario->SetSpeed(0, 0);
		mario->SetLevel(MARIO_LEVEL_BIG);
		mario->ChangeState(new CStandingState(MARIO_LEVEL_BIG));
		break;
	case DIK_3:
		mario->SetPosition(current_x, 450);
		mario->SetLevel(RACCOON_LEVEL_BIG);
		mario->ChangeState(new CStandingState(RACCOON_LEVEL_BIG));
		break;
	case DIK_4:
		mario->SetPosition(current_x, 450);
		mario->SetLevel(FIRE_LEVEL);
		mario->ChangeState(new CStandingState(FIRE_LEVEL));
		break;
	case DIK_R:
		mario->Reset();
		break;

	}
}

void CPlayerState::KeyState(BYTE* states)
{
	CMario* mario = CMario::GetInstance();
	CGame* game = CGame::GetInstance();
	CPlayerState* player_state = mario->GetState();

	if (dynamic_cast<CJumpingState*>(player_state) ||
		dynamic_cast<CHighJumpingState*>(player_state) ||
		dynamic_cast<CFallingState*>(player_state)) {
		if (is_rendered_completely) {
			if (game->IsKeyDown(DIK_Z) && level == RACCOON_LEVEL_BIG) {
				ani = RACCOON_ANI_SPINNING_BIG;
				mario->animation_set->at(ani)->ResetFlagLastFrame();
				CPlayerState::SetAnimation(mario->animation_set->at(ani));
			}
			else if (!game->IsKeyDown(DIK_Z) && level == RACCOON_LEVEL_BIG) {
				if (dynamic_cast<CFallingState*>(player_state))
					ani = RACCOON_ANI_FALLING_BIG;
				else
					ani = RACCOON_ANI_BIG_JUMPING_RIGHT;
			}
		}
		if (ani == RACCOON_ANI_SPINNING_BIG) {
			CheckState();
		}
	}
}

void CPlayerState::CheckState()
{
	int current_frame = animation->GetCurrentFrame();
	CMario* mario = CMario::GetInstance();
	DebugOut(L"current %d\n", current_frame);
	if (current_frame == 0 || current_frame == 4 || current_frame == 2)
		mario->is_attacking_by_spinning = true;
	else
		mario->is_attacking_by_spinning = false;

	// phải render full frame mới được bật cờ render full ani
	//if (!animation->NextIsLastFrame() && animation->IsLastFrame()) {
	if (animation->IsLastFrame()) {
		is_rendered_completely = true;
		mario->is_spinning = false;
	}
	else {
		is_rendered_completely = false;
		mario->is_spinning = true;
	}
	DebugOut(L"attacking %d\n", mario->is_attacking_by_spinning);
}


