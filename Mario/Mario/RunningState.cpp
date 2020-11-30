#include "RunningState.h"
#include "PlayScene.h"
CRunningState::CRunningState(int level)
{
	DebugOut(L"Running\n");
	this->level = level;

	SetAnimation(level);
	CMario* mario = CMario::GetInstance();
	acceleration = MARIO_ACCELERATION;

}

void CRunningState::Update(float dt)
{
	CMario* mario = CMario::GetInstance();
	CGame* game = CGame::GetInstance();
	float speed_x = abs(mario->vx);
	//DebugOut(L"vx khi running ne %f\n", speed_x);

	if (speed_x < 0.7 || (is_max_speed && is_speed_low)) {
		is_max_speed = false;
		mario->vx = (speed_x + acceleration * dt) * mario->nx;
	}
	else {
		is_max_speed = true;
	}
	CPlayScene* scene = (CPlayScene*)game->GetCurrentScene();


	if (speed_x < MARIO_WALKING_SPEED && can_change_to_walking && is_rendered_completely) {
		is_speed_low = false;
		is_max_speed = false;
		mario->is_attacking_by_spinning = false;
		mario->ChangeState(new CWalkingState(level));
		return;
	}
}

void CRunningState::HandleKeyboard()
{

}

void CRunningState::SetAnimation(int level)
{
	CMario* mario = CMario::GetInstance();
	this->level = level;
	switch (level) {
	case MARIO_LEVEL_BIG:
		if (is_max_speed) {
			ani = MARIO_ANI_BIG_RUNNING;
		}
		else if (is_skid)
			ani = MARIO_ANI_BIG_SKID;
		else if (mario->is_crouching)
			ani = MARIO_ANI_BIG_CROUCHING_RIGHT;
		else
			ani = MARIO_ANI_BIG_WALKING_RIGHT;
		break;
	case MARIO_LEVEL_SMALL:
		if (is_max_speed) {
			ani = MARIO_ANI_SMALL_RUNNING;
		}
		else if (is_skid)
			ani = MARIO_ANI_SMALL_SKID;
		else
			ani = MARIO_ANI_SMALL_WALKING_RIGHT;
		break;
	case RACCOON_LEVEL_BIG:
		if (is_max_speed) {
			ani = RACCOON_ANI_RUNNING_BIG;
		}
		else if (mario->is_crouching)
			ani = RACCOON_ANI_BIG_CROUCHING_RIGHT;
		else if (is_skid)
			ani = RACCOON_ANI_SKID_BIG;
		else
			ani = RACCOON_ANI_BIG_WALKING_RIGHT;
		break;
	case FIRE_LEVEL:
		if (is_max_speed) {
			ani = FIRE_MARIO_ANI_RUNNING;
		}
		else if (mario->is_crouching)
			ani = FIRE_MARIO_ANI_CROUCHING_RIGHT;
		else if (is_skid)
			ani = FIRE_MARIO_ANI_SKID;
		else
			ani = FIRE_MARIO_ANI_WALKING_RIGHT;
		break;
	}
}

void CRunningState::OnKeyDown(int KeyCode)
{
	CMario* mario = CMario::GetInstance();
	switch (KeyCode) {
	case DIK_A:
		if (level == RACCOON_LEVEL_BIG && is_rendered_completely) {
			start_ani = GetTickCount64();
			start_count = GetTickCount64();
			ani = RACCOON_ANI_SPINNING_BIG;
			animation->ResetFlagLastFrame();
			CPlayerState::SetAnimation(animation);
		}
		else if (level == FIRE_LEVEL && is_rendered_completely) {
			if (mario->number_attack == 0 || mario->number_attack == 1) {
				mario->time_start_attack = GetTickCount64();
			}
			mario->number_attack++;
			DWORD count = GetTickCount64();

			if (count - mario->time_start_attack <= 1000 && mario->number_attack <= 2) {
				ani = FIRE_MARIO_ANI_THROW;
				animation->ResetFlagLastFrame();
				CPlayerState::SetAnimation(animation);
			}
			else if (count - mario->time_start_attack > 1000) {
				mario->number_attack = 1;
				ani = FIRE_MARIO_ANI_THROW;
				animation->ResetFlagLastFrame();
				CPlayerState::SetAnimation(animation);
			}

		}
		return;
		break;
	case DIK_S:
		if (is_max_speed) {
			mario->time_start_jump = GetTickCount64();
			//DebugOut(L"change to flying\n");
			mario->ChangeState(new CFlyingState(level));
			return;
		}
		break;
	}
	CPlayerState::OnKeyDown(KeyCode);
}

void CRunningState::OnKeyUp(int KeyCode)
{
}

void CRunningState::KeyState(BYTE* state)
{
	CGame* game = CGame::GetInstance();
	CMario* mario = CMario::GetInstance();
	if (ani == RACCOON_ANI_SPINNING_BIG || ani == FIRE_MARIO_ANI_THROW) {
		CheckState();
		if (is_rendered_completely)
			SetAnimation(level);
	}
	if (game->IsKeyDown(DIK_A)) {
		// Bấm A và right left cùng lúc thì giảm gia tốc 
		// Bấm A thả cả 2 nút l r thì giảm tốc
		// ở đây ko reset 2 cờ left right, là vì nút A vẫn còn gi
		if ((game->IsKeyDown(DIK_RIGHT) && game->IsKeyDown(DIK_LEFT)) ||
			!game->IsKeyDown(DIK_LEFT) && !game->IsKeyDown(DIK_RIGHT)) {
			is_speed_low = true;
			can_change_to_walking = true;
			acceleration = -MARIO_ACCELERATION;
			if (game->IsKeyDown(DIK_DOWN)) {
				mario->is_crouching = true;
				if (!already_added) {
					mario->SetY(mario->GetY() + DISPARITIES);
					already_added = true;
				}
				SetAnimation(level);
			}
		}
		else if (game->IsKeyDown(DIK_RIGHT))
		{
			already_added = false;
			// ko là bé chồn hoặc bé chồn đã hoàn thành việc xoay mới running
			if ((level != RACCOON_LEVEL_BIG && level != FIRE_LEVEL) || is_rendered_completely) {
				SetAnimation(level);
				is_right = true;
				// chưa bấm nút left bao giờ, chỉ bấm right lần đầu => tăng tốc về bên phải
				if (!is_left) {
					mario->nx = 1;
					acceleration = MARIO_ACCELERATION;
				}
				// đã bấm nút left trước đó, bấm right để thắng lại và quay đầu => giảm tốc rồi tăng lại
				else
				{
					// bật cờ này để cho mario giảm hết tốc độ rồi quay đầu chứ hong chuyển về walking state
					if (!is_skid && mario->nx < 0) {
						start_skid = GetTickCount64();
						is_skid = true;
						if (!already_minused && mario->is_crouching) {
							mario->SetY(mario->GetY() - DISPARITIES);
							already_minused = true;
						}
						mario->is_crouching = false;
					}

					can_change_to_walking = false;
					is_speed_low = true;
					acceleration = -MARIO_ACCELERATION;
					DWORD stop_skid = GetTickCount64();
					// tăng tốc mario trở lại sau khi đã xả hết tốc độ
					//if (mario->vx >= 0) {
					if (stop_skid - start_skid >= 400) {
						is_skid = false;
						already_minused = false;
						mario->nx = 1;
						is_speed_low = false;
						acceleration = MARIO_ACCELERATION;
					}


					SetAnimation(level);
				}

			}
		}
		else if (game->IsKeyDown(DIK_LEFT)) {
			already_added = false;
			if ((level != RACCOON_LEVEL_BIG && level != FIRE_LEVEL) || is_rendered_completely) {
				is_left = true;
				SetAnimation(level);
				if (!is_right) {
					// right ko an
					mario->nx = -1;
					acceleration = MARIO_ACCELERATION;
				}
				else
				{
					if (!is_skid && mario->nx > 0) {
						start_skid = GetTickCount64();
						is_skid = true;
						if (!already_minused && mario->is_crouching) {
							mario->SetY(mario->GetY() - DISPARITIES);
							already_minused = true;
						}
						mario->is_crouching = false;

					}
					can_change_to_walking = false;
					is_speed_low = true;
					acceleration = -MARIO_ACCELERATION;
					DWORD stop_skid = GetTickCount64();
					//if (mario->vx ><= 0) {
					if (stop_skid - start_skid >= 400) {
						mario->nx = -1;
						is_speed_low = false;
						already_minused = false;
						is_skid = false;
						acceleration = MARIO_ACCELERATION;
					}
					SetAnimation(level);
				}
			}

		}
	}
	else if (!game->IsKeyDown(DIK_A)) {
		//DebugOut(L"bo A\n");
		is_speed_low = true;
		acceleration = -MARIO_ACCELERATION;
		can_change_to_walking = true;
	}
	if (game->IsKeyDown(DIK_X)) {
		mario->time_start_jump = GetTickCount64();
		mario->ChangeState(new CJumpingState(level));
	}
}

void CRunningState::CheckState()
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

