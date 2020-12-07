#pragma once
#include "GameObject.h"
#include "Mario.h"
#include "Game.h"

#define STATE_START 0
#define STATE_1 1
#define STATE_2 2
#define STATE_3 3
#define STATE_4 4
#define STATE_5 5
#define STATE_6 6
#define STATE_CARD 7
#define STATE_EMPTY 8

#define ANI_START 0
#define ANI_1 1
#define ANI_2 2
#define ANI_3 3
#define ANI_4 4
#define ANI_5 5
#define ANI_6 6
#define ANI_CARD 7
#define ANI_EMPTY 8


class CMapPortal : public CGameObject
{
public:
	int id_portal;

	int is_portal;
	int t, l, r, b;

	CMapPortal(int id, float x, float y, int state, bool is_portal, int l, int t, int r, int b);
	~CMapPortal() {};

	void GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx = 0, int dy = 0);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void Render();

};

