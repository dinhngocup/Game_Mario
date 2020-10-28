#pragma once
#include "GameObject.h"
#include "PlayerState.h"
#include "JumpingState.h"
#include "HighJumpingState.h"
#include "SpinningState.h"
#include "InvisibleObject.h"
class CMario : public CGameObject
{
	int level;
	int untouchable;
	DWORD untouchable_start;

	float start_x;			// initial position of Mario at scene
	float start_y;
	
	static CMario* __instance;
	

public:
	int ani;
	CPlayerState *player_state;

	//bool is_jumping = false;
	bool is_crouching = false;
	DWORD time_start_jump;
	bool is_press_s = false;
	bool is_attacking = false;
	bool is_attacking_by_spinning = false;
	
	DWORD time_start_attack;

	int number_attack = 0;

	float current_y = 27;

	CMario();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	virtual void RenderBoundingBox();

	
	void SetLevel(int l) { level = l; }
	int GetLevel() { return level; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }

	void Reset();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx, int dy);

	static CMario* GetInstance();

	void SetStartPosition(float start_x, float start_y);
	void SetPosition(float x, float y);

	void OnKeyDown(int KeyCode);
	void OnKeyUp(int KeyCode);
	void KeyState(BYTE* states);
	void ChangeState(CPlayerState* newState);

	CPlayerState* GetState() { return player_state; }

};