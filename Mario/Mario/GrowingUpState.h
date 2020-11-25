#pragma once
#include "Mario.h"
#include "PlayerState.h"
#include "Game.h"
#define STATE_SMALL	0
#define STATE_BIG 1
class CGrowingUpState : public CPlayerState
{
private: 
	DWORD start_ani1;
	DWORD start_ani2;
	int limit;
	int state = STATE_SMALL;
	float prev_vx;
public:
	CGrowingUpState() {};
	CGrowingUpState(int level);

	~CGrowingUpState() {};
	void Update(float dt);
	void HandleKeyboard() {};
	void SetAnimation(int level);

	void OnKeyDown(int KeyCode) {};
	void OnKeyUp(int KeyCode) {};
	void KeyState(BYTE* state) {};
};

