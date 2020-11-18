﻿#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "GameObject.h"
#include "Brick.h"
#include "Mario.h"
#include "Koopa.h"
#include "Tiles.h"
#include "Map.h"
#include "FireBall.h"
#include "InvisibleObject.h"
#include "BrickQuestion.h"
#include "Goomba.h"
#include "Grid.h"
#include "Hub.h"

#define DEFAULT_CAM_Y	750.0f
#define SCORE_LENGTH	7
#define TIME_LENGTH 3
#define COIN_LENGTH 2
#define TOTAL_NUMBER_IN_HUB 14
struct Number
{
	float x, y;
	int id;
};

class CPlayScene : public CScene
{
protected:
	CMario* player;					// A play scene has to have player, right? 

	// data object for this scene
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_INFO(string line);
	void _ParseSection_MAP(string line);
	void _ParseSection_STATIC_OBJECTS(string line);
	void _ParseSection_TILESET(string line);
	void _ParseSection_HUB(string line);
	void _ParseSection_FONT(string line);
	void ReadFileHub(LPCWSTR filePath);
	int sceneWidth, sceneHeight;
	bool isMoved;
	int world_id, map_id;
	CTiles *tiles;
	CMap* map;
	CGrid* grid;
	CHub* hub;
	vector<Number> numbers;
	DWORD previousTime;
	int time_game;
	int time_limit;
public:
	vector<LPGAMEOBJECT> enemies;
	vector<LPGAMEOBJECT> ghost_platforms;
	vector<LPGAMEOBJECT> items;

	CPlayScene(int id, LPCWSTR filePath);

	virtual void LoadSceneResources();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	void UpdateHub(DWORD dt);
	void RenderHub();
	void UpdateSpeedBar(float mario_speed);
	CMario* GetPlayer() { return player; }
	
	/*vector<LPGAMEOBJECT>* GetEnemiesInScene() { return &enemies; }
	vector<LPGAMEOBJECT>* GetGhostPlatformsInScene() { return &ghost_platforms; }
	vector<LPGAMEOBJECT>* GetItemsInScene() { return &items; }*/


	void SetEnemiesInScene(vector<LPGAMEOBJECT> listEnemy) { enemies.clear(); enemies = listEnemy; }
	void SetGhostPlatformsInScene(vector<LPGAMEOBJECT> listGhostPlatform) { ghost_platforms = listGhostPlatform; }
	void SetItemsInScene(vector<LPGAMEOBJECT> listItem) { items = listItem; }

	//vector<LPGAMEOBJECT>* GetItemsInScene() { return &objects; }
	//void AddObject(LPGAMEOBJECT obj) { objects.push_back(obj); }
	//friend class CPlayScenceKeyHandler;
};

class CPlayScenceKeyHandler : public CScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	CPlayScenceKeyHandler(CScene* s) :CScenceKeyHandler(s) {};
};

