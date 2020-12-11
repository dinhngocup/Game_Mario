#pragma once
#include "GameObject.h"
#include "Textures.h"
#include "Game.h"
#include"IntroScene.h"

#define STATE_GHOST_PLATFORM 0
#define STATE_CACTUS 1
#define STATE_HELP_TAG 2
#define STATE_GUARD 3
#define STATE_3_INTRO 4
#define STATE_ARROW 5

#define ANI_GHOST_PLATFORM 0
#define ANI_CACTUS 1
#define ANI_HELP_TAG 2
#define ANI_GUARD 3
#define ANI_3_INTRO 4
#define ANI_ARROW 5

class CInvisibleObject : public CGameObject
{
public:
	CInvisibleObject() {};
	CInvisibleObject(float x, float y, float width, float height, int state = 0);
	~CInvisibleObject() {};

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx = 0, int dy = 0);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();


};

