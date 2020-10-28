#include "PlayerState.h"
#include "Mario.h"
#include "Game.h"
void CPlayerState::OnKeyDown(int KeyCode)
{
	CMario* mario = CMario::GetInstance();
	float current_x = mario->GetX();

	CPlayerState* player_state = mario->GetState();
	switch (KeyCode)
	{
	case DIK_X:
		if(!dynamic_cast<CJumpingState*>(player_state) && 
			!dynamic_cast<CHighJumpingState*>(player_state) &&
			!dynamic_cast<CFallingState*>(player_state)) {
			mario->ChangeState(new CJumpingState(level));
		}
		break;
	case DIK_S:
		if (!dynamic_cast<CJumpingState*>(player_state) &&
			!dynamic_cast<CHighJumpingState*>(player_state) &&
			!dynamic_cast<CFallingState*>(player_state)) {
			mario->time_start_jump = GetTickCount();
			mario->ChangeState(new CHighJumpingState(level));
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


