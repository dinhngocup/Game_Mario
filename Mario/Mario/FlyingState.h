#pragma once
#include "Mario.h"
#include "PlayerState.h"
#include "Game.h"

class CFlyingState : public CPlayerState
{
public:
	CFlyingState() {};
	CFlyingState(int level);

	~CFlyingState() {};
	void Update(float dt);
	void HandleKeyboard();
	void SetAnimation(int level);

	void OnKeyDown(int KeyCode);
	void OnKeyUp(int KeyCode);
	void KeyState(BYTE* state);
};

