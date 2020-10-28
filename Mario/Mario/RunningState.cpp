#include "RunningState.h"

CRunningState::CRunningState(int level)
{
	DebugOut(L"Running\n");
	this->level = level;
	if (level == RACCOON_LEVEL_BIG) {
		ani = RACCOON_ANI_SPINNING_BIG;
	}
	else
		SetAnimation(level);

	acceleration = MARIO_ACCELERATION;
}

void CRunningState::Update(float dt)
{
	CMario* mario = CMario::GetInstance();

	float speed_x = abs(mario->vx);
	// is speed low dùng để xác định lúc nào là lúc giảm tốc
	/*
	TH1: tốc độ chưa đạt tối đa thì rơi vô câu if(speed_x < 0.7)
	để tăng tốc or giảm tốc tùy theo acceleration
	TH2: đã đạt đến tốc độ tối đa thì rơi vô else để ko tăng tốc nữa
	or rơi vô (is_max_speed && is_speed_low) để biết là đang max speed và có ý định giảm tốc
	bởi cờ is speed low
	*/
	if (speed_x < 0.7 || (is_max_speed && is_speed_low)) {
		is_max_speed = false;
		mario->vx = (speed_x + acceleration * dt) * mario->nx;
	}
	else {
		is_max_speed = true;
	}

	/*if (mario->vx > 0) mario->nx = 1;
	else {
		mario->nx = -1;
	}*/

	if (level != RACCOON_LEVEL_BIG || (!animation->NextIsLastFrame() && animation->IsLastFrame())) {
		SetAnimation(level);
	}
	else
		ani = RACCOON_ANI_SPINNING_BIG;


	if (speed_x < MARIO_WALKING_SPEED && can_change_to_walking) {
		DebugOut(L"change to walking\n");

		is_speed_low = false;
		is_max_speed = false;

		mario->ChangeState(new CWalkingState(level));
		return;

	}

}

void CRunningState::HandleKeyboard()
{
	
}

void CRunningState::SetAnimation(int level)
{
	switch (level) {
	case MARIO_LEVEL_BIG:
		if (is_max_speed) {
			ani = MARIO_ANI_BIG_RUNNING;
		}
		else if (is_skid)
			ani = MARIO_ANI_BIG_SKID;
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
		else if (is_skid)
			ani = RACCOON_ANI_SKID_BIG;
		else
			ani = RACCOON_ANI_BIG_WALKING_RIGHT;
		break;
	case FIRE_LEVEL:
		if (is_max_speed) {
			ani = FIRE_MARIO_ANI_RUNNING;
		}
		else if (is_skid)
			ani = FIRE_MARIO_ANI_SKID;
		else
			ani = FIRE_MARIO_ANI_WALKING_RIGHT;
		break;
	}
}

void CRunningState::OnKeyDown(int KeyCode)
{
	CPlayerState::OnKeyDown(KeyCode);
	switch (KeyCode) {
	case DIK_A:
		if (level == RACCOON_LEVEL_BIG) {
			ani = RACCOON_ANI_SPINNING_BIG;
			animation->ResetFlagLastFrame();
		}
		break;
	}
}

void CRunningState::OnKeyUp(int KeyCode)
{
	CGame* game = CGame::GetInstance();
	CMario* mario = CMario::GetInstance();
	switch (KeyCode) {
	case DIK_A:
		//Nếu bỏ nút A thì ko xét hai nút trái phải, vì lúc này giảm tốc dần rồi chuyển qua walking
		//DebugOut(L"stop A\n");
		is_speed_low = true;
		acceleration = -MARIO_ACCELERATION;
		can_change_to_walking = true;
		// bỏ nút A ra, nó chưa kịp chuyển về walking, bấm nút A tiếp,
		// thì lúc này phải áp dụng logic code quay đầu chứ lúc này đâu phải
		// state vừa bắt đầu chạy nhanh đâu
		/*is_left = false;
		is_right = false;*/

		break;
	}
}

void CRunningState::KeyState(BYTE* state)
{

	CGame* game = CGame::GetInstance();
	CMario* mario = CMario::GetInstance();
	if (game->IsKeyDown(DIK_A)) {
		// Bấm A và right left cùng lúc thì giảm gia tốc 
		// Bấm A thả cả 2 nút l r thì giảm tốc
		// ở đây ko reset 2 cờ left right, là vì nút A vẫn còn gi
		if ((game->IsKeyDown(DIK_RIGHT) && game->IsKeyDown(DIK_LEFT)) ||
			!game->IsKeyDown(DIK_LEFT) && !game->IsKeyDown(DIK_RIGHT)) {
			//DebugOut(L"stopppp\n");
			is_speed_low = true;
			can_change_to_walking = true;
			acceleration = -MARIO_ACCELERATION;
		}
		else if (game->IsKeyDown(DIK_RIGHT))
		{
			// ko là bé chồn hoặc bé chồn đã hoàn thành việc xoay mới running
			if (level != RACCOON_LEVEL_BIG || (!animation->NextIsLastFrame() && animation->IsLastFrame())) {
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
					can_change_to_walking = false;
					is_speed_low = true;
					acceleration = -MARIO_ACCELERATION ;
					is_skid = true;
					// tăng tốc mario trở lại sau khi đã xả hết tốc độ
					if (mario->vx >= 0) {
						is_skid = false;
						mario->nx = 1;
						is_speed_low = false;
						acceleration = MARIO_ACCELERATION;
					}
					SetAnimation(level);
				}

			}
		}
		else if (game->IsKeyDown(DIK_LEFT)) {
			if (level != RACCOON_LEVEL_BIG || (!animation->NextIsLastFrame() && animation->IsLastFrame())) {
				is_left = true;
				if (!is_right) {
					// right ko an
					mario->nx = -1;
					acceleration = MARIO_ACCELERATION;
				}
				else
				{
					can_change_to_walking = false;
					is_speed_low = true;
					acceleration = -MARIO_ACCELERATION ;
					is_skid = true;
					if (mario->vx <= 0) {
						mario->nx = -1;
						is_speed_low = false;
						is_skid = false;
						acceleration = MARIO_ACCELERATION;
					}
					SetAnimation(level);
				}
			}

		}
	}
}

