#pragma once
#include "GameObject.h"
#include "PlayerState.h"
#include "JumpingState.h"
#include "HighJumpingState.h"
#include "SpinningState.h"
#include "InvisibleObject.h"
#include "Brick.h"
#include "BrickQuestion.h"
#include "MarioConst.h"
#include "Koopa.h"
#include "Enemy.h"
#include "Item.h"
#include "KickingState.h"
class CMario : public CGameObject
{
	int level;
	int untouchable;
	DWORD untouchable_start;

	float start_x;			// initial position of Mario at scene
	float start_y;
	
	static CMario* __instance;
	int score = 0;
	int coin_number = 0;
	vector <int> cards;
public:
	int ani;
	CPlayerState *player_state;

	
	DWORD unhide_start;
	bool is_crouching = false;
	DWORD time_start_jump;
	bool is_press_s = false;
	bool is_attacking = false;
	bool is_attacking_by_spinning = false;
	//bool is_spinning = false;
	//bool is_running = false;
	bool is_skid = false;
	bool is_holding = true;
	bool is_flying = false;
	DWORD time_start_attack;

	int number_attack = 0;


	CMario();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	

	
	void SetLevel(int l) { level = l; }
	int GetLevel() { return level; }
	void StartUntouchable();
	int GetScore() { return score; }
	int GetCoin() { return coin_number; }
	void AddScore(int score) { this->score += score; }
	void AddCoin() { this->coin_number++; }
	void Reset();
	void AddCard(int card_id) { cards.push_back(card_id); }
	vector<int> GetCards() { return cards; }
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx, int dy);

	static CMario* GetInstance();

	void SetStartPosition(float start_x, float start_y);
	void SetPosition(float x, float y);

	void OnKeyDown(int KeyCode);
	void OnKeyUp(int KeyCode);
	void KeyState(BYTE* states);
	void ChangeState(CPlayerState* newState);
	void SetState(int state);

	CPlayerState* GetState() { return player_state; }
	void IsCollisionWithBrick(LPCOLLISIONEVENT e);
};