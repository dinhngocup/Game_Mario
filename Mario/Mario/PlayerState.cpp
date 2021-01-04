#include "PlayerState.h"
#include "Mario.h"
#include "GrowingUpState.h"
#include "Game.h"
void CPlayerState::OnKeyDown(int KeyCode)
{
	CMario* mario = CMario::GetInstance();
	float current_x = mario->GetX();
	float current_y = mario->GetY();
	CPlayerState* player_state = mario->GetState();
	CGame* game = CGame::GetInstance();
	switch (KeyCode)
	{
	case DIK_L:
		game->SetCamYPos(145.0f);
		mario->x = 6770.0f;
		mario->y = 150.0f;
		game->SetCamXPos(mario->x - game->GetScreenWidth() / 2);
		break;
	case DIK_K:
		game->SetCamYPos(145.0f);
		mario->x = 3940.0f;
		mario->y = 150.0f;
		mario->is_flying = true;
		game->SetCamXPos(mario->x - game->GetScreenWidth() / 2);
		break;
	case DIK_M:
		game->SetCamYPos(750.0f);
		mario->x = 6960.0f;
		mario->y = 1000.0f;
		game->SetCamXPos(mario->x - game->GetScreenWidth() / 2);
		break;
	case DIK_N:
		game->SetCamYPos(750.0f);
		mario->x = 5856.0f;
		mario->y = 1000.0f;
		game->SetCamXPos(mario->x - game->GetScreenWidth() / 2);
		break;
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
			//DebugOut(L"vx truoc khi nhay %f\n", mario->vx);
			mario->time_start_jump = GetTickCount();
			
			mario->ChangeState(new CHighJumpingState(level));
		}
		break;
	case DIK_A:
		if (dynamic_cast<CJumpingState*>(player_state) ||
			dynamic_cast<CHighJumpingState*>(player_state) ||
			dynamic_cast<CFallingState*>(player_state)) {
			player_state->start_ani = GetTickCount64();
			player_state->start_count = GetTickCount64();
			if (level == RACCOON_LEVEL_BIG) {

				if (player_state->is_rendered_completely) {
					ani = RACCOON_ANI_SPINNING_BIG;
					mario->animation_set->at(ani)->ResetFlagLastFrame();
					CPlayerState::SetAnimation(mario->animation_set->at(ani));
				}
				if (ani == RACCOON_ANI_SPINNING_BIG) {
					player_state->CheckState();
				}
			}
			else if (level == FIRE_LEVEL) {
				if (player_state->is_rendered_completely) {
					ani = FIRE_ANI_FLYING_THROW;
					mario->animation_set->at(ani)->ResetFlagLastFrame();
					CPlayerState::SetAnimation(mario->animation_set->at(ani));

				}
				if (ani == FIRE_ANI_FLYING_THROW) {
					player_state->CheckState();
				}
			}
		}
		break;
	case DIK_1:
		mario->SetPosition(current_x, 0);
		mario->SetSpeed(0, 0);
		mario->SetLevel(MARIO_LEVEL_SMALL);
		mario->SetState(0);
		mario->ChangeState(new CStandingState(MARIO_LEVEL_SMALL));
		break;
	case DIK_2:
		mario->SetPosition(current_x, 0);
		mario->SetSpeed(0, 0);
		mario->SetState(0);
		mario->SetLevel(MARIO_LEVEL_BIG);
		mario->ChangeState(new CStandingState(MARIO_LEVEL_BIG));
		break;
	case DIK_3:
		mario->SetPosition(current_x, 0);
		mario->SetLevel(RACCOON_LEVEL_BIG);
		mario->SetState(0);
		mario->ChangeState(new CStandingState(RACCOON_LEVEL_BIG));
		break;
	case DIK_4:
		mario->SetPosition(current_x, 0);
		mario->SetLevel(FIRE_LEVEL);
		mario->SetState(0);
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
			player_state->start_ani = GetTickCount64();
			player_state->start_count = GetTickCount64();
			if (game->IsKeyDown(DIK_Z)) {
				if (level == RACCOON_LEVEL_BIG) {
					ani = RACCOON_ANI_SPINNING_BIG;
				}
				else if (level == FIRE_LEVEL) {
					ani = FIRE_ANI_FLYING_THROW;
					mario->press_z = true;
					mario->start_press_z = GetTickCount64();
				}
				mario->animation_set->at(ani)->ResetFlagLastFrame();
				CPlayerState::SetAnimation(mario->animation_set->at(ani));
			}
			else if (!game->IsKeyDown(DIK_Z)) {
				if (level == RACCOON_LEVEL_BIG) {
					if (dynamic_cast<CFallingState*>(player_state))
						ani = RACCOON_ANI_FALLING_BIG;
					else
						ani = RACCOON_ANI_BIG_JUMPING_RIGHT;
				}
				else if (level == FIRE_LEVEL) {
					if (dynamic_cast<CFallingState*>(player_state))
						ani = FIRE_MARIO_ANI_FALLING;
					else
						ani = FIRE_MARIO_ANI_JUMPING_RIGHT;
				}
			}
		}
		if (ani == RACCOON_ANI_SPINNING_BIG || ani == FIRE_ANI_FLYING_THROW) {
			player_state->CheckState();
		}
	}
}

//
//void CPlayerState::CheckState()
//{
//	int current_frame = animation->GetCurrentFrame();
//	CMario* mario = CMario::GetInstance();
//	DebugOut(L"frame %d\n", current_frame);
//	CPlayerState* player_state = mario->GetState();
//	if (dynamic_cast<CKickingState*>(player_state)) {
//		if (animation->NextIsLastFrame()) {
//			is_rendered_completely = true;
//		}
//		else
//			is_rendered_completely = false;
//		return;
//	}
//	if (level == RACCOON_LEVEL_BIG) {
//		if (current_frame == 0 || current_frame == 4 || current_frame == 2)
//			mario->is_attacking_by_spinning = true;
//		else
//			mario->is_attacking_by_spinning = false;
//
//	}
//	else if (level == FIRE_LEVEL) {
//		if (current_frame == 1 && !is_attacked) {
//			mario->is_attacking = true;
//			// allow throw only 1 fire ball
//			is_attacked = true;
//		}
//	}
//	
//
//	 phải render full frame mới được bật cờ render full ani
//	if (!animation->NextIsLastFrame() && animation->IsLastFrame()) {
//	/*if (animation->IsLastFrame()) {
//		is_rendered_completely = true;
//		if (level == FIRE_LEVEL) {
//			is_attacked = false;
//		}
//	}
//	else {
//		is_rendered_completely = false;
//	}*/
//	/*DebugOut(L"attacking %d\n", mario->is_attacking);
//	DebugOut(L"is_attacked %d\n", is_attacked);*/
//}


