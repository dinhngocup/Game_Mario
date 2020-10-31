#pragma once
#include "PlayerState.h"
#include "MarioConst.h"
#include "Mario.h"
#include "Game.h"



class CCrouchingState : public CPlayerState
{
public:
	CCrouchingState() {};
	CCrouchingState(int level);

	~CCrouchingState() {};
	void Update(float dt);
	void HandleKeyboard();
	void SetAnimation(int level);

	void OnKeyDown(int KeyCode);
	void OnKeyUp(int KeyCode);
	void KeyState(BYTE* states);
};

