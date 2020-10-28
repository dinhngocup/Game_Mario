#pragma once
#include "PlayerState.h"
#include "Game.h"
#include "Mario.h"
#include "MarioConst.h"
#include "StandingState.h"
#include "Utils.h"
class CFallingState : public CPlayerState
{
	bool is_done = false;
	bool is_rendered_completely = true;
	bool able_to_change = false;
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

