#pragma once
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

#define DEFAULT_CAM_Y	720.0f
class CPlayScene : public CScene
{
protected:
	CMario* player;					// A play scene has to have player, right? 

	// con trỏ đến tất cả đối tượng trong game
	//vector<LPGAMEOBJECT> objects;

	vector<LPGAMEOBJECT> enemies;
	vector<LPGAMEOBJECT> ghost_platforms;
	vector<LPGAMEOBJECT> items;



	
	// data object for this scene
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_MAP(string line);
	void _ParseSection_STATIC_OBJECTS(string line);
	void _ParseSection_TILESET(string line);
	
	int sceneWidth, sceneHeight;
	bool isMoved;
	CTiles *tiles;
	CMap* map;
	CGrid* grid;

public:
	CPlayScene(int id, LPCWSTR filePath);

	virtual void LoadSceneResources();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	CMario* GetPlayer() { return player; }
	
	vector<LPGAMEOBJECT>* GetEnemiesInScene() { return &enemies; }
	vector<LPGAMEOBJECT>* GetGhostPlatformsInScene() { return &ghost_platforms; }
	vector<LPGAMEOBJECT>* GetItemsInScene() { return &items; }


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

