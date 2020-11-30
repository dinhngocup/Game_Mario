#pragma once
#include "PlayerState.h"
#include "Mario.h"
class CAttackingState : public CPlayerState
{
public:
	bool is_done = false;
	bool is_attacked = false;
	CAttackingState() {};
	CAttackingState(int level);

	~CAttackingState() {};

	void SetAnimation();
	void Update(float dt);
	void HandleKeyboard();
	
	void OnKeyUp(int KeyCode) {};
	void OnKeyDown(int KeyCode) {};
	void KeyState(BYTE* state);

	void CheckState();
};

