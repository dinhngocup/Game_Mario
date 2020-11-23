#pragma once
#include "Game.h"
#include "Font.h"
#include "Sprites.h"
#include "GameConst.h"

#define MUSHROOM_CARD 1
#define FIRE_FLOWER_CARD 2
#define STAR_MAN_CARD 3


#define MUSHROOM_CARD_SPRITE_ID 40041
#define FIRE_FLOWER_CARD_SPRITE_ID 40042
#define STAR_MAN_CARD_SPRITE_ID 40043
class CHub
{
	vector<CFont*> letters;
	static CHub* __instance;
	int time_limit = 300;
	float card_posX;
	float hub_posX, hub_posY;
public:
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

