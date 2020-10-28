#pragma once
#include "PlayerState.h"
#include "Mario.h"
#include "MarioConst.h"
#include "Game.h"
#include "StandingState.h"
#include "RunningState.h"
class CWalkingState : public CPlayerState
{
public:
	CWalkingState() {};
	CWalkingState(int level);

	~CWalkingState() {};

	void Update(float dt);
	void HandleKeyboard();

	void SetAnimation(int level);

	void OnKeyDown(int KeyCode);
	void OnKeyUp(int KeyCode);
	void KeyState(BYTE* state);

};

