#pragma once
#include "Scene.h"
#include "Tiles.h"
#include "Map.h"
#include "InvisibleObject.h"

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TILESET	0
#define SCENE_SECTION_MAP	1
#define SCENE_SECTION_STATIC_OBJECTS	2


#define MAX_SCENE_LINE 1024

class CIntroScene : public CScene
{
	CTiles* tiles;
	CMap* map;
	vector<LPGAMEOBJECT> ghost_platforms;
public:
	bool change_level = false;
	CIntroScene(int id, LPCWSTR filePath);

	void LoadSceneResources();
	void Update(DWORD dt);
	void Render();
	void Unload();

	void _ParseSection_MAP(string line);
	void _ParseSection_TILESET(string line);
	void _ParseSection_STATIC_OBJECTS(string line);


};


class CIntroSceneKeyHandler : public CScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states) {};
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode) {};
	CIntroSceneKeyHandler(CScene* s) :CScenceKeyHandler(s) {};
};

