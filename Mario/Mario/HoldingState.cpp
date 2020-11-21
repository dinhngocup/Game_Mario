#include "HoldingState.h"
#include "PlayScene.h"
CHoldingState::CHoldingState(int level)
{
	//DebugOut(L"holding\n");

	this->level = level;
	SetAnimation(level);
}

void CHoldingState::Update(float dt)
{
	CMario* mario = CMario::GetInstance();

	float speed_x = abs(mario->vx);

	if (speed_x < 0.7 || (is_max_speed && is_speed_low)) {
		is_max_speed = false;
		mario->vx = (speed_x + acceleration * dt) * mario->nx;
	}
	else {
		is_max_speed = true;
	}
	//DebugOut(L"vx %f\n", mario->vx);


	if (speed_x < MARIO_WALKING_SPEED && can_change_to_walking) {
		//DebugOut(L"change to walking\n");
		is_speed_low = false;
		is_max_speed = false;
		mario->vx = 0;
		SetAnimation(level);

	}
	CGame* game = CGame::GetInstance();

	CPlayScene* scene = (CPlayScene*)game->GetCurrentScene();
	vector<LPGAMEOBJECT> enemies = scene->enemies;

	for (int i = 0; i < enemies.size(); i++) {
		if (dynamic_cast<CKoopa*>(enemies.at(i)) && enemies.at(i)->state == STATE_HOLD) {

			enemies.at(i)->nx = mario->nx;

			enemies.at(i)->x += (mario->vx - enemies.at(i)->vx) * dt;

			break;
		}
	}
}

void CHoldingState::HandleKeyboard()
{
}

void CHoldingState::SetAnimation(int level)
{
	this->level = level;
	CMario* mario = CMario::GetInstance();
	switch (level) {
	case RACCOON_LEVEL_BIG:
		if (mario->vx == 0) {
			ani = RACCOON_ANI_STAND_HOLD;
		}
		else
			ani = RACCOON_ANI_HOLDING;
		break;
	case MARIO_LEVEL_BIG:
		if (mario->vx == 0) {
			ani = MARIO_ANI_BIG_STAND_HOLD;
		}
		else
			ani = MARIO_ANI_BIG_HOLDING;
		break;
	case FIRE_LEVEL:
		if (mario->vx == 0) {
			ani = FIRE_ANI_STAND_HOLD;
		}
		else
			ani = FIRE_ANI_HOLDING;
		break;
	case MARIO_LEVEL_SMALL:
		if (mario->vx == 0) {
			ani = MARIO_ANI_SMALL_STAND_HOLD;
		}
		else
			ani = MARIO_ANI_SMALL_HOLDING;
		break;
	}
}

void CHoldingState::OnKeyDown(int KeyCode)
{

}

void CHoldingState::OnKeyUp(int KeyCode)
{
	CMario* mario = CMario::GetInstance();
	switch (KeyCode) {
	case DIK_A:

		break;
	}
}

void CHoldingState::KeyState(BYTE* states)
{
	CMario* mario = CMario::GetInstance();

	CGame* game = CGame::GetInstance();
	if (game->IsKeyDown(DIK_A)) {
		if ((game->IsKeyDown(DIK_RIGHT) && game->IsKeyDown(DIK_LEFT)) ||
			!game->IsKeyDown(DIK_LEFT) && !game->IsKeyDown(DIK_RIGHT)) {
			is_speed_low = true;
			can_change_to_walking = true;
			acceleration = -MARIO_ACCELERATION;
		}
		else if (game->IsKeyDown(DIK_RIGHT))
		{
			if (mario->nx < 0) {
				mario->is_skid = true;
				start_skid = GetTickCount64();
				//DebugOut(L"skid right\n");
			}
			SetAnimation(level);
			can_change_to_walking = false;
			is_right = true;
			if (!is_left) {
				mario->nx = 1;
				acceleration = MARIO_ACCELERATION;
			}
			else
			{
				is_speed_low = true;
				acceleration = -MARIO_ACCELERATION;
				mario->nx = 1;
				DWORD stop_skid = GetTickCount64();
				if (stop_skid - start_skid >= 400) {
					is_speed_low = false;
					acceleration = MARIO_ACCELERATION;
				}
			}
		}
		else if (game->IsKeyDown(DIK_LEFT)) {
			if (mario->nx > 0) {
				mario->is_skid = true;
				start_skid = GetTickCount64();
				//DebugOut(L"skid left\n");
			}
			SetAnimation(level);
			is_left = true;
			can_change_to_walking = false;
			if (!is_right) {
				// right ko an
				mario->nx = -1;
				acceleration = MARIO_ACCELERATION;
			}
			else
			{
				is_speed_low = true;
				acceleration = -MARIO_ACCELERATION;
				mario->nx = -1;
				DWORD stop_skid = GetTickCount64();
				if (stop_skid - start_skid >= 400) {
					mario->nx = -1;
					is_speed_low = false;
					acceleration = MARIO_ACCELERATION;
				}
				//SetAnimation(level);
			}
			//DebugOut(L"nx left%d\n", mario->nx);

		}
	}
	else if (!game->IsKeyDown(DIK_A)) {
		is_speed_low = true;
		acceleration = -MARIO_ACCELERATION * 3;
		if (mario->vx <= 0 && mario->nx > 0 || mario->vx >= 0 && mario->nx < 0) {
			mario->is_holding = false;
			mario->is_attacking_by_spinning = false;
			mario->ChangeState(new CKickingState(level));
		}
	}
}
