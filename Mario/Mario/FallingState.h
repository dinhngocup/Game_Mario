#pragma once
#include "PlayerState.h"
#include "Game.h"
#include "Mario.h"
#include "MarioConst.h"
#include "StandingState.h"
#include "Utils.h"
class CFallingState : public CPlayerState
{
	bool is_falling_slowly = false;
	DWORD time_start_to_press;
public:
	CFallingState() {};
	CFallingState(int level);

	~CFallingState() {};
	void Update(float dt);
	void HandleKeyboard();
	void SetAnimation(int level); 
	
	void OnKeyDown(int KeyCode);
	void OnKeyUp(int KeyCode);
	void KeyState(BYTE* state);

	void CheckState();
};

