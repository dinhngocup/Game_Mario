#pragma once
#include "Game.h"
#include "Font.h"
#include "Sprites.h"
#include "GameConst.h"
class CHub
{
	vector<CFont*> letters;
	static CHub* __instance;
public:
	static CHub* GetInstance();
	void AddFont(CFont* letter);
	void Render(float cam_x, float cam_y);
	CFont* GetFont(int id) { return letters[id]; }
};

