#pragma once
#include "Sprites.h"
#include "GameConst.h"
#include "Font.h"

#define MUSHROOM_CARD 1
#define STAR_MAN_CARD 2
#define FIRE_FLOWER_CARD 3


#define MUSHROOM_CARD_SPRITE_ID 40041
#define FIRE_FLOWER_CARD_SPRITE_ID 40042
#define STAR_MAN_CARD_SPRITE_ID 40043

struct Number
{
	float x, y;
	int id;
};

class CHub
{
	CHub() {}
	vector<CFont*> letters;
	static CHub* __instance;
	int time_limit = 300;
	float card_posX;
	float hub_posX, hub_posY;
public:
	vector<Number> numbers;
	vector<Number> end_scene_letters_1;
	vector<Number> end_scene_letters_2;
	vector<Number> time_up_title;
	float card_in_title_X, card_in_title_Y;
	static CHub* GetInstance();
	int GetTimeLimit() { return time_limit; }
	void AddFont(CFont* letter);
	void Render();
	CFont* GetFont(int id) { return letters[id]; }
	int GetCardId(int id);
	void GetCardPosX(float& x) { x = card_posX;}
	void SetCardPosX(float x) { card_posX = x; }
	void SetHubPos(float cam_x, float cam_y);
	void GetHubPos(float& x, float& y) { x = hub_posX; y = hub_posY; }
	void RenderBackgroundHub(float hub_posX, float hub_posY);
};

