#pragma once
#include "PlayerState.h"
#include "MarioConst.h"
#include "Game.h"
#include "Mario.h"
#include "FallingState.h"
class CJumpingState : public CPlayerState
{
public:
	CJumpingState();
	CJumpingState(int level);

	~CJumpingState() {};

	void Update(float dt);
	void HandleKeyboard();

	void SetAnimation(int level);

	void OnKeyDown(int KeyCode);
	void OnKeyUp(int KeyCode) {};
	void KeyState(BYTE* state);
};

