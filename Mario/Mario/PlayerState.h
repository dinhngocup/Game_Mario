#pragma once
#include "DefineLibs.h"
#include "Animation.h"
class CPlayerState {
protected:
	int ani;
	int level;
	CAnimation* animation;
public:
	CPlayerState() {};
	CPlayerState(int level) {};

	int GetAnimation() { return this->ani; };
	void SetAnimation(CAnimation* animation) { this->animation = animation; }
	void SetAniId(int aniId) { this->ani = aniId; }
	virtual void Update(float dt) = 0;
	virtual void HandleKeyboard() = 0;

	virtual void OnKeyUp(int KeyCode) {};
	virtual void OnKeyDown(int KeyCode);
	virtual void KeyState(BYTE* states) {};

};
