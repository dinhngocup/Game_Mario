#pragma once
#include "PlayerState.h"
#include "Mario.h"
#include "MarioConst.h"
#include "Game.h"
#include "WalkingState.h"
#include "AutoGoState.h"
#include "CrouchingState.h"
#include "AttackingState.h"
class CStandingState : public CPlayerState
{	
public:
	CStandingState();
	CStandingState(int level);

	~CStandingState() {};

	void Update(float dt);
	void HandleKeyboard();
	void SetAnimation(int level);

	void OnKeyDown(int KeyCode);
	void KeyState(BYTE* state);
};

