#pragma once
#include "PlayerState.h"
#include "Game.h"
#include "Mario.h"

class CHighJumpingState : public CPlayerState
{
	float acceleration;
	//bool is_spinning = true;
public:
	CHighJumpingState() {};
	CHighJumpingState(int level);

	~CHighJumpingState() {};

	void Update(float dt);
	void HandleKeyboard();
	void SetAnimation(int level);

	void OnKeyDown(int KeyCode);
	void OnKeyUp(int KeyCode);
	void KeyState(BYTE* state);
	void CheckState();
};

