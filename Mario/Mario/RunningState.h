#pragma once
#include "PlayerState.h"
#include "Game.h"
#include "Mario.h"

class CRunningState : public CPlayerState
{
private:
	float acceleration = MARIO_ACCELERATION;
	bool is_speed_low = false;
	bool is_max_speed = false;
	bool is_skid = false;
	bool is_right = false;
	bool is_left = false;
	bool can_change_to_walking = true;
public:
	CRunningState() {};
	CRunningState(int level);

	~CRunningState() {};

	void Update(float dt);
	void HandleKeyboard();
	void SetAnimation(int level);

	void OnKeyDown(int KeyCode);
	void OnKeyUp(int KeyCode);
	void KeyState(BYTE* state);
};

