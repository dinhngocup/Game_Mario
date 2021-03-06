#pragma once
#include "PlayerState.h"
#include "Mario.h"
#include "Game.h"
#include "GameObject.h"
class CHoldingState : public CPlayerState
{
private:
	float acceleration = MARIO_ACCELERATION;
	bool is_speed_low = false;
	bool is_max_speed = false;
	//bool is_skid = false;
	bool is_right = false;
	bool is_left = false;
	bool can_change_to_walking = false;
	DWORD start_skid;
public:
	CHoldingState() {};
	CHoldingState(int level);

	~CHoldingState() {};
	void Update(float dt);
	void HandleKeyboard();
	void SetAnimation(int level);

	void OnKeyDown(int KeyCode);
	void OnKeyUp(int KeyCode);
	void KeyState(BYTE* states);

};

