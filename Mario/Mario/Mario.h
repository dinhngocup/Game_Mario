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
#include "GoDownState.h"

class CMario : public CGameObject
{
	int level = 0;
	int time_flicker;

	float start_x;			// initial position of Mario at scene
	float start_y;
	
	static CMario* __instance;
	int score = 0;
	int coin_number = 0;
	vector <int> cards;
	int lives = MARIO_DEFAULT_LIVE;
public:
	int current_map_portal_id = 1;
	bool up_scene = false;
	DWORD untouchable_start;
	DWORD start_press_z = 0;
	bool press_z;
	int untouchable;
	int ani;
	CPlayerState *player_state;

	
	DWORD unhide_start;
	bool is_crouching = false;
	DWORD time_start_jump;
	bool is_press_s = false;
	bool is_attacking = false;
	bool is_attacking_by_spinning = false;
	bool is_skid = false;
	bool is_holding = true;
	bool is_flying = false;
	DWORD time_start_attack;
	int collide_with_portal = 0;
	bool is_underground = false;

	int number_attack = 0;
	bool is_auto_go_in_map = false;

	CMario();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	
	void UpdateInMapScene(DWORD dt);
	void RenderInMapScene();

	void SetLevel(int l) { level = l; }
	int GetLevel() { return level; }
	void StartUntouchable();
	void UpLevel();
	int GetScore() { return score; }
	int GetCoin() { return coin_number; }
	int GetLives() { return lives; }
	void AddLive() { this->lives++; }
	void ResetLive() { this->lives = 4; }
	void MinusLive() { this->lives--; }
	void AddScore(int score) { this->score += score; }
	void AddCoin() { this->coin_number++; }
	void Reset();
	void AddCard(int card_id) { cards.push_back(card_id); }
	vector<int> GetCards() { return cards; }
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx, int dy);
	void GetHeightMario();
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
	void IsCollisionWithBlingBlingBrick(LPCOLLISIONEVENT e);

	void MarioAutoGo();
	void MarioAutoGoInMap(float x, float y);
};