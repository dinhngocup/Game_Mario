#pragma once
#include "PlayerState.h"
#include "Mario.h"

class CKickingState : public CPlayerState
{
public:
	CKickingState() {};
	CKickingState(int level);

	~CKickingState() {};
	void Update(float dt);
	void HandleKeyboard();
	void SetAnimation(int level);

	void OnKeyDown(int KeyCode);
	void OnKeyUp(int KeyCode);
	void KeyState(BYTE* states);

	void CheckState();
};

