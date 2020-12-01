#include "WalkingState.h"


CWalkingState::CWalkingState(int level)
{
	OutputDebugString(L"walking\n");
	this->level = level;

	SetAnimation(level);
}

void CWalkingState::Update(float dt)
{
	CMario* mario = CMario::GetInstance();
	if (mario->vy > MARIO_GRAVITY && !mario->is_crouching) {
		mario->ChangeState(new CFallingState(level));
		return;
	}

	if (is_slowly) {
		float speed = abs(mario->vx);
		mario->vx = (speed - 0.0008 * dt) * mario->nx;
		if (!is_skid) {
			if (mario->nx > 0 && mario->vx <= 0 || mario->nx < 0 && mario->vx >= 0) {
				if (mario->is_crouching) {
					mario->vx = 0;
					mario->ChangeState(new CCrouchingState(level));
				}
				else {
					mario->ChangeState(new CStandingState(level));
				}
				return;
			}
		}
	}
}

void CWalkingState::HandleKeyboard()
{

}

void CWalkingState::SetAnimation(int level)
{
	this->level = level;
	CMario* mario = CMario::GetInstance();
	switch (level) {
	case RACCOON_LEVEL_BIG:
		if (is_skid)
			ani = RACCOON_ANI_SKID_BIG;
		else if (mario->is_crouching)
			ani = RACCOON_ANI_BIG_CROUCHING_RIGHT;
		else
			ani = RACCOON_ANI_BIG_WALKING_RIGHT;
		break;
	case MARIO_LEVEL_BIG:
		if (is_skid)
			ani = MARIO_ANI_BIG_SKID;
		else if (mario->is_crouching)
			ani = MARIO_ANI_BIG_CROUCHING_RIGHT;
		else
			ani = MARIO_ANI_BIG_WALKING_RIGHT;
		break;
	case MARIO_LEVEL_SMALL:
		if (is_skid)
			ani = MARIO_ANI_SMALL_SKID;
		else
			ani = MARIO_ANI_SMALL_WALKING_RIGHT;
		break;
	case FIRE_LEVEL:
		if (is_skid)
			ani = FIRE_MARIO_ANI_SKID;
		else if (mario->is_crouching)
			ani = FIRE_MARIO_ANI_CROUCHING_RIGHT;
		else
			ani = FIRE_MARIO_ANI_WALKING_RIGHT;
		break;
	}
}

void CWalkingState::OnKeyDown(int KeyCode)
{
	CPlayerState::OnKeyDown(KeyCode);
}

void CWalkingState::OnKeyUp(int KeyCode)
{
	CMario* mario = CMario::GetInstance();

	/*switch (KeyCode) {
	case DIK_RIGHT:
		is_slowly = true;
		break;
	case DIK_LEFT:
		is_slowly = true;
		break;
	}*/
}

void CWalkingState::KeyState(BYTE* state)
{
	CMario* mario = CMario::GetInstance();
	CGame* game = CGame::GetInstance();

	if (game->IsKeyDown(DIK_Z)) {
		if(level == RACCOON_LEVEL_BIG)
			mario->ChangeState(new CSpinningState(level));
		else if (level == FIRE_LEVEL) {
			if (GetTickCount64() - mario->start_press_z >= 1000 || mario->start_press_z == 0) {
				mario->press_z = true;
				mario->ChangeState(new CAttackingState(level));
				mario->start_press_z = GetTickCount64();
			}
		}
		return;
	}
	if (game->IsKeyDown(DIK_RIGHT) && game->IsKeyDown(DIK_LEFT)) {
		is_slowly = true;
		return;
	}
	// xử lí việc bấm phím a right left cùng lúc thì chuyển mario về trạng thái đứng
	if (game->IsKeyDown(DIK_A) && game->IsKeyDown(DIK_RIGHT) && game->IsKeyDown(DIK_LEFT)) {
		is_slowly = true;
		return;
	}
	else if (game->IsKeyDown(DIK_A)) {
		if (!game->IsKeyDown(DIK_RIGHT) && !game->IsKeyDown(DIK_LEFT)) {
			is_slowly = true;
			return;
		}
		if (game->IsKeyDown(DIK_RIGHT)) {
			mario->nx = 1;
		}
		else if (game->IsKeyDown(DIK_LEFT)) {
			mario->nx = -1;
		}

		if (level == FIRE_LEVEL) {
			if (mario->number_attack == 0 || mario->number_attack == 1) {
				mario->time_start_attack = GetTickCount();
			}
			mario->number_attack++;
			DWORD count = GetTickCount();

			if (count - mario->time_start_attack <= 1000 && mario->number_attack <= 2) {
				mario->ChangeState(new CAttackingState(level));

			}
			else if (count - mario->time_start_attack > 1000) {
				mario->number_attack = 1;
				mario->ChangeState(new CAttackingState(level));

			}
			/*else {
				DebugOut(L"ignore\n");
			}*/
		}
		else if (level == RACCOON_LEVEL_BIG) {
			mario->ChangeState(new CSpinningState(level));
		}
		else
			mario->ChangeState(new CRunningState(level));
	}
	else if (game->IsKeyDown(DIK_RIGHT))
	{
		is_right = true;
		is_slowly = false;
		if (!is_left) {
			mario->vx = MARIO_WALKING_SPEED;
			mario->nx = 1;
		}
		else {
			is_slowly = true;
			
				is_skid = true;
			if (mario->vx >= 0) {
				is_skid = false;
				mario->nx = 1;
				mario->vx = MARIO_WALKING_SPEED;
				is_slowly = false;
			}
		}
		SetAnimation(level);
	}
	else if (game->IsKeyDown(DIK_LEFT)) {
		is_left = true;
		is_slowly = false;
		if (!is_right) {
			mario->vx = -MARIO_WALKING_SPEED;
			mario->nx = -1;
		}
		else {
			is_slowly = true;
				is_skid = true;
			
			if (mario->vx <= 0) {
				is_skid = false;
				mario->nx = -1;
				mario->vx = -MARIO_WALKING_SPEED;
				is_slowly = false;
			}
		}
		SetAnimation(level);
	}
	else if (game->IsKeyDown(DIK_DOWN) && !game->IsKeyDown(DIK_RIGHT) && !game->IsKeyDown(DIK_LEFT)) {
		//DebugOut(L"hiii %d\n", mario->change_bounding_box_crouching);
		if (level != MARIO_LEVEL_SMALL) {
			is_slowly = true;
			mario->is_crouching = true;
			if (!already_added) {
				mario->SetY(mario->GetY() + DISPARITIES);
				already_added = true;
			}
			SetAnimation(level);

		}
		else {
			is_slowly = true;
		}
	}
	else {
		is_slowly = true;
		if (is_skid) {
			is_skid = false;
			mario->nx *= -1;
			SetAnimation(level);

		}

	}




}
