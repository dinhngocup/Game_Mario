#pragma once
#include "PlayerState.h"
#include "Mario.h"
#include "Game.h"
class CSpinningState : public CPlayerState
{
public:
	CSpinningState() {};
	CSpinningState(int level);

	~CSpinningState() {};
	void Update(float dt);
	void HandleKeyboard();
	void SetAnimation();

	void OnKeyDown(int KeyCode);
	void OnKeyUp(int KeyCode) {};
	void KeyState(BYTE* state);

	
};

