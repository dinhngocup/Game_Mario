#pragma once
#include "PlayerState.h"
#include "Game.h"
#include "Mario.h"
#include "MarioConst.h"
#include "Utils.h"
class CGoDownState : public CPlayerState
{
	
public:
	CGoDownState() {};
	CGoDownState(int level);

	~CGoDownState() {};
	void Update(float dt);
	void HandleKeyboard() {};
	void SetAnimation(int level);

	void OnKeyDown(int KeyCode) {};
	void OnKeyUp(int KeyCode) {};
	void KeyState(BYTE* state) {};
};

