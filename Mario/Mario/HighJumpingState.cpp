#include "HighJumpingState.h"

CHighJumpingState::CHighJumpingState(int level)
{
	DebugOut(L"high jump\n");
	this->level = level;
	CMario* mario = CMario::GetInstance();
	mario->vy = -MARIO_JUMP_SPEED_Y;
	SetAnimation(level);
}

void CHighJumpingState::Update(float dt)
{
	CMario* mario = CMario::GetInstance();
	// phải ở trạng thái được rơi và render hết frame spinning
	// phải có thêm cờ able_to_change là vì, nếu chỉ đơn giản là
	//if (able_to_change && is_rendered_completely) {
	//	// Frame cuối cùng render là frame 0, làm thế cho hiệu ứng đẹp á
	//	// mà frame 0 thì nó tính collision => phải tắt cờ collision by spinning
	//	mario->is_attacking_by_spinning = false;
	//	//DebugOut(L"collision %d\n", mario->is_attacking_by_spinning);
	//	mario->ChangeState(new CFallingState(level));
	//	return;
	//}
	// chuyen trang thai falling
	if (mario->vy >= 0) {
		if (ani == RACCOON_ANI_SPINNING_BIG && is_rendered_completely || ani != RACCOON_ANI_SPINNING_BIG) {
			mario->is_attacking_by_spinning = false;
			mario->ChangeState(new CFallingState(level));
		}
		// đáng lẽ phải rơi nhưng chưa render hết => bật cờ đc rơi đợi render hết mới rơi
		/*else if (ani == RACCOON_ANI_SPINNING_BIG && !is_rendered_completely)
			able_to_change = true;*/
	}
	//DebugOut(L"collision spinningggg %d\n", mario->is_attacking_by_spinning);
}

void CHighJumpingState::HandleKeyboard()
{

}

void CHighJumpingState::SetAnimation(int level)
{
	CMario* mario = CMario::GetInstance();
	switch (level) {
	case MARIO_LEVEL_BIG:
		if (!mario->is_crouching)
			ani = MARIO_ANI_BIG_JUMPING_RIGHT;
		else
			ani = MARIO_ANI_BIG_CROUCHING_RIGHT;
		break;
	case MARIO_LEVEL_SMALL:
		ani = MARIO_ANI_SMALL_JUMPING_RIGHT;
		break;
	case RACCOON_LEVEL_BIG:
		if (!mario->is_crouching)
			ani = RACCOON_ANI_BIG_JUMPING_RIGHT;
		else
			ani = RACCOON_ANI_BIG_CROUCHING_RIGHT;
		break;
	case FIRE_LEVEL:
		if (!mario->is_crouching)
			ani = FIRE_MARIO_ANI_JUMPING_RIGHT;
		else
			ani = FIRE_MARIO_ANI_CROUCHING_RIGHT;
		break;
	}
}

void CHighJumpingState::OnKeyDown(int KeyCode)
{
	CMario* mario = CMario::GetInstance();
	switch (KeyCode) {
	case DIK_A:
		if (level == RACCOON_LEVEL_BIG) {
			if (is_rendered_completely && !able_to_change) {
				ani = RACCOON_ANI_SPINNING_BIG;
				mario->animation_set->at(ani)->ResetFlagLastFrame();
				CPlayerState::SetAnimation(mario->animation_set->at(ani));
			}
			if (ani == RACCOON_ANI_SPINNING_BIG) {
				CheckState();
			}

		}
		break;
	}
}

void CHighJumpingState::OnKeyUp(int KeyCode)
{
	CMario* mario = CMario::GetInstance();
	switch (KeyCode) {

	case DIK_S:
		if (mario->vy >= 0) {
			if (ani == RACCOON_ANI_SPINNING_BIG && is_rendered_completely || ani != RACCOON_ANI_SPINNING_BIG) {
				mario->ChangeState(new CFallingState(level));
			}
			else if (ani == RACCOON_ANI_SPINNING_BIG && !is_rendered_completely)
				able_to_change = true;
		}
		break;
	}
}

void CHighJumpingState::KeyState(BYTE* state)
{
	CGame* game = CGame::GetInstance();
	CMario* mario = CMario::GetInstance();
	if (game->IsKeyDown(DIK_S)) {

		DWORD time_press_s = GetTickCount();
		if (time_press_s - mario->time_start_jump <= 300) {
			mario->vy = -MARIO_JUMP_SPEED_Y;
		}
	}
	if (game->IsKeyDown(DIK_RIGHT)) {
		mario->vx = MARIO_WALKING_SPEED;
		mario->nx = 1;
	}
	else if (game->IsKeyDown(DIK_LEFT)) {
		mario->vx = -MARIO_WALKING_SPEED;
		mario->nx = -1;
	}
	
	// Vẫn giữ nguyên state high jump nhưng chuyển ani
	if (is_rendered_completely) {
		if (game->IsKeyDown(DIK_Z) && level == RACCOON_LEVEL_BIG) {
			ani = RACCOON_ANI_SPINNING_BIG;
			mario->animation_set->at(ani)->ResetFlagLastFrame();
			CPlayerState::SetAnimation(mario->animation_set->at(ani));
		}
		else if (!game->IsKeyDown(DIK_Z) && level == RACCOON_LEVEL_BIG)
			ani = RACCOON_ANI_BIG_JUMPING_RIGHT;
	}
	if (ani == RACCOON_ANI_SPINNING_BIG) {
		CheckState();
	}
}

void CHighJumpingState::CheckState()
{
	int current_frame = animation->GetCurrentFrame();
	CMario* mario = CMario::GetInstance();
	//DebugOut(L"current %d\n", current_frame);
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
