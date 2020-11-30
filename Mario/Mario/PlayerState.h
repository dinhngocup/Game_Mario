#pragma once
#include "DefineLibs.h"
#include "Animation.h"
class CPlayerState {
protected:
	int ani;
	int level;
	CAnimation* animation;
	bool is_rendered_completely = true;
	bool is_attacked = false;
	DWORD start_ani, start_count;
public:
	CPlayerState() {};
	CPlayerState(int level) {};

	int GetAnimation() { return this->ani; };
	void SetAnimation(CAnimation* animation) { this->animation = animation; }
	void SetAniId(int aniId) { this->ani = aniId; }
	void SetLevel(int level) { this->level = level; }

	virtual void Update(float dt) = 0;
	virtual void HandleKeyboard() = 0;
	virtual void SetAnimation(int level) {};

	virtual void OnKeyUp(int KeyCode) {};
	virtual void OnKeyDown(int KeyCode);
	virtual void KeyState(BYTE* states);

	virtual void CheckState() {};

};
