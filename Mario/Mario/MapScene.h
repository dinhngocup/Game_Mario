#pragma once
#include "Scene.h"
#include "Mario.h"
#include "Tiles.h"
#include "Map.h"
#include "MapPortal.h"

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_INFO 0
#define SCENE_SECTION_TILESET	1
#define SCENE_SECTION_MAP	2
#define SCENE_SECTION_STATIC_OBJECTS	3
#define SCENE_SECTION_MAP_SELECTION	4


#define MAX_SCENE_LINE 1024
#define TOTAL_NUMBER_IN_HUB 14
#define SCORE_LENGTH	7
#define TIME_LENGTH 3
#define COIN_LENGTH 2

class CMapScene : public CScene
{
	CMario* player;
	CTiles* tiles;
	CMap* map;
	int world_id;

public:
	CMapPortal* current_portal;
	vector<LPGAMEOBJECT> ghost_platforms;
	vector<LPGAMEOBJECT> map_portals;
	CMapScene(int id, LPCWSTR filePath);

	void LoadSceneResources();
	void Update(DWORD dt);
	void Render();
	void Unload();

	void _ParseSection_INFO(string line);
	void _ParseSection_MAP(string line);
	void _ParseSection_TILESET(string line);
	void _ParseSection_STATIC_OBJECTS(string line);
	void _ParseSection_MAP_SELECTION(string line);


	void RenderItemHub();
	void GetHubStatistic();
};


class CMapSceneKeyHandler : public CScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	CMapSceneKeyHandler(CScene* s) :CScenceKeyHandler(s) {};
};
