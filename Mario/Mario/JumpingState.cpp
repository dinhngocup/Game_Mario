#include "JumpingState.h"

CJumpingState::CJumpingState()
{
}

CJumpingState::CJumpingState(int level)
{
	OutputDebugString(L"jump\n");

	this->level = level;

	CMario* mario = CMario::GetInstance();
	mario->vy = -MARIO_JUMP_SPEED_Y;

	SetAnimation(level);
}

void CJumpingState::Update(float dt)
{
	CMario* mario = CMario::GetInstance();

	if (mario->vy >= 0) {
		//if ((mario->is_spinning && is_rendered_completely) || !mario->is_spinning) {
		if (is_rendered_completely) {
			if (level == RACCOON_LEVEL_BIG)
				mario->is_attacking_by_spinning = false;
			//DebugOut(L"vx truoc khi nhay %f\n", mario->vx);
			mario->ChangeState(new CFallingState(level));
		}
	}
}

void CJumpingState::HandleKeyboard()
{


}

void CJumpingState::SetAnimation(int level)
{
	this->level = level;
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

void CJumpingState::OnKeyDown(int KeyCode)
{
	CMario* mario = CMario::GetInstance();
	CPlayerState::OnKeyDown(KeyCode);

}

void CJumpingState::KeyState(BYTE* state)
{
	CGame* game = CGame::GetInstance();
	CMario* mario = CMario::GetInstance();

	CPlayerState::KeyState(state);

	if (game->IsKeyDown(DIK_X)) {
	
		if (mario->collide_with_portal == 1) {
			mario->is_underground = true;
			mario->vx = 0;
			return;
		}
		if (game->IsKeyDown(DIK_RIGHT)) {
			if (abs(mario->vx) <= MARIO_WALKING_SPEED)
				mario->vx = MARIO_WALKING_SPEED;
			mario->nx = 1;
		}
		else if (game->IsKeyDown(DIK_LEFT)) {
			if (abs(mario->vx) <= MARIO_WALKING_SPEED)
				mario->vx = -MARIO_WALKING_SPEED;
			mario->nx = -1;
		}
	}


}

