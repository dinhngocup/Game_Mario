#include "IntroScene.h"

CIntroScene::CIntroScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	
	key_handler = new CIntroSceneKeyHandler(this);
}

void CIntroScene::LoadSceneResources()
{
	ifstream f;
	f.open(sceneFilePath);
	char str[MAX_SCENE_LINE];
	int section = SCENE_SECTION_UNKNOWN;
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);
		// skip the comment
		if (line[0] == '#') continue;
		if (line == "[TILESET]") { section = SCENE_SECTION_TILESET; continue; }
		if (line == "[MAP]") { section = SCENE_SECTION_MAP; continue; }
		if (line == "[STATIC_OBJECTS]") { section = SCENE_SECTION_STATIC_OBJECTS; continue; }


		switch (section) {
		case SCENE_SECTION_TILESET:_ParseSection_TILESET(line); break;
		case SCENE_SECTION_MAP: _ParseSection_MAP(line); break;
		case SCENE_SECTION_STATIC_OBJECTS: _ParseSection_STATIC_OBJECTS(line); break;
		}
	}

	f.close();
	
	DebugOut(L"[INFO] Done loading resources of this scene %s\n", sceneFilePath);
}

void CIntroScene::Update(DWORD dt)
{
	for (size_t i = 0; i < ghost_platforms.size(); i++)
	{
		ghost_platforms[i]->Update(dt);
	}
}

void CIntroScene::Render()
{
	map->DrawMap(0.0f, 0.0f);
	for (LPGAMEOBJECT obj : ghost_platforms)
		obj->Render();
}

void CIntroScene::Unload()
{
	for (LPGAMEOBJECT obj : ghost_platforms)
		delete obj;
	ghost_platforms.clear();

	if (tiles != NULL) {
		tiles->ClearTileSet();
	}
	if (map != NULL) {
		map->ClearMap();
		delete map;
	}
	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void CIntroScene::_ParseSection_MAP(string line)
{
	if (line == "") return;
	wstring path = ToWSTR(line);
	this->map = new CMap();
	map->ReadMap(path.c_str());
}

void CIntroScene::_ParseSection_TILESET(string line)
{

	vector<string> tokens = split(line);

	if (tokens.size() < 7) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());


	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	int column = atoi(tokens[5].c_str());
	int row = atoi(tokens[6].c_str());

	tiles = CTiles::GetInstance();
	tiles->LoadResources(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B), column, row);
	tiles->LoadTiles();
}

void CIntroScene::_ParseSection_STATIC_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 8) return;

	int object_type = atoi(tokens[0].c_str());

	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());

	float w = atof(tokens[3].c_str());
	float h = atof(tokens[4].c_str());

	int ani_set_id = atoi(tokens[5].c_str());
	int state = atoi(tokens[6].c_str());
	int type = atoi(tokens[7].c_str());

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();

	CGameObject* obj = NULL;

	switch (object_type)
	{
	case eTYPE::INVISIBLE_OBJECT: obj = new CInvisibleObject(x, y, w, h, state); obj->type = eTYPE::INVISIBLE_OBJECT; break;
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);

	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

	obj->SetAnimationSet(ani_set);

	ghost_platforms.push_back(obj);
}

void CIntroSceneKeyHandler::OnKeyDown(int KeyCode)
{
	CGame* game = CGame::GetInstance();
	CIntroScene* scene = (CIntroScene*)game->GetCurrentScene();

	switch (KeyCode) {
	case DIK_W:
		game->SwitchScene(WORLD_MAP);
		break;
	case DIK_Q:
		scene->change_level = true;
		break;
	}
}
