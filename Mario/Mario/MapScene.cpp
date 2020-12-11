#include "MapScene.h"

void CMapSceneKeyHandler::KeyState(BYTE* states)
{


}

void CMapSceneKeyHandler::OnKeyDown(int KeyCode)
{
	CMario* mario = CMario::GetInstance();
	CGame* game = CGame::GetInstance();
	CMapScene* scene = (CMapScene*)game->GetCurrentScene();
	if (!mario->is_auto_go_in_map) {
		switch (KeyCode) {
		case DIK_ESCAPE:
			game->SwitchScene(INTRO_SCENE);
			break;
		case DIK_S:
			if (scene->current_portal->is_portal) {
				if (mario->up_scene)
					game->SwitchScene(SCENE_2);
				else
					game->SwitchScene(SCENE_1);

			}
			break;
		case DIK_LEFT:
			if (scene->current_portal->l != -1) {
				int id_next_portal = scene->current_portal->l;
				scene->current_portal = NULL;
				scene->current_portal = dynamic_cast<CMapPortal*>(scene->map_portals[id_next_portal]);
				//mario->SetPosition(scene->current_portal->x, scene->current_portal->y);
				mario->vx = -0.2f;
				mario->vy = 0.0f;
				mario->is_auto_go_in_map = true;
			}
			break;
		case DIK_RIGHT:
			if (scene->current_portal->r != -1) {
				int id_next_portal = scene->current_portal->r;
				scene->current_portal = NULL;
				scene->current_portal = dynamic_cast<CMapPortal*>(scene->map_portals[id_next_portal]);
				mario->vx = 0.2f;
				mario->vy = 0.0f;

				mario->is_auto_go_in_map = true;
				//mario->SetPosition(scene->current_portal->x, scene->current_portal->y);
			}
			break;
		case DIK_UP:
			if (scene->current_portal->t != -1) {
				int id_next_portal = scene->current_portal->t;
				scene->current_portal = NULL;
				scene->current_portal = dynamic_cast<CMapPortal*>(scene->map_portals[id_next_portal]);
				mario->vy = -0.2f;
				mario->vx = 0.0f;

				mario->is_auto_go_in_map = true;
				//mario->SetPosition(scene->current_portal->x, scene->current_portal->y);
			}
			break;
		case DIK_DOWN:
			if (scene->current_portal->b != -1) {
				int id_next_portal = scene->current_portal->b;
				scene->current_portal = NULL;
				scene->current_portal = dynamic_cast<CMapPortal*>(scene->map_portals[id_next_portal]);
				mario->vy = 0.2f;
				mario->vx = 0.0f;
				mario->is_auto_go_in_map = true;
				//mario->SetPosition(scene->current_portal->x, scene->current_portal->y);
			}
			break;
		}
	}
	
}

void CMapSceneKeyHandler::OnKeyUp(int KeyCode)
{
}

CMapScene::CMapScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	key_handler = new CMapSceneKeyHandler(this);
}

void CMapScene::LoadSceneResources()
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);
	player = CMario::GetInstance();
	player->vx = 0;
	player->vy = 0;
	CHub* hub = CHub::GetInstance();
	CGame* game = CGame::GetInstance();
	ifstream f;
	f.open(sceneFilePath);
	game->SetCamPos(0.0f, 0.0f);
	char str[MAX_SCENE_LINE];
	int section = SCENE_SECTION_UNKNOWN;
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);
		// skip the comment
		if (line[0] == '#') continue;
		if (line == "[SCENE_INFO]") { section = SCENE_SECTION_INFO; continue; }
		if (line == "[TILESET]") { section = SCENE_SECTION_TILESET; continue; }
		if (line == "[MAP]") { section = SCENE_SECTION_MAP; continue; }
		if (line == "[STATIC_OBJECTS]") { section = SCENE_SECTION_STATIC_OBJECTS; continue; }
		if (line == "[MAP_SELECTION]") { section = SCENE_SECTION_MAP_SELECTION; continue; }


		switch (section) {
		case SCENE_SECTION_INFO:_ParseSection_INFO(line); break;
		case SCENE_SECTION_TILESET:_ParseSection_TILESET(line); break;
		case SCENE_SECTION_MAP: _ParseSection_MAP(line); break;
		case SCENE_SECTION_STATIC_OBJECTS: _ParseSection_STATIC_OBJECTS(line); break;
		case SCENE_SECTION_MAP_SELECTION: _ParseSection_MAP_SELECTION(line); break;

		}
	}

	f.close();
	CTextures::GetInstance()->Add(-100, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));
	CTextures::GetInstance()->Add(-200, L"textures\\bbox_black.png", D3DCOLOR_XRGB(255, 255, 255));


	DebugOut(L"[INFO] Done loading resources of this scene %s\n", sceneFilePath);
	hub->SetHubPos(game->GetCamX(), game->GetCamY());
	// chỉ gọi 1 lần trong load resource vì trong map không có thay đổi thông số của hub
	GetHubStatistic();
	if (player->GetLevel() == 0) {
		player->SetLevel(MARIO_LEVEL_SMALL);
	}
	current_portal = dynamic_cast<CMapPortal*>(map_portals[player->current_map_portal_id]);

	player->SetPosition(current_portal->x, current_portal->y);

	player->nx = 1;

	LPANIMATION_SET ani_set = CAnimationSets::GetInstance()->Get(1);

	player->SetAnimationSet(ani_set);
}

void CMapScene::Update(DWORD dt)
{
	for (size_t i = 0; i < ghost_platforms.size(); i++)
	{
		ghost_platforms[i]->Update(dt);
	}
	for (size_t i = 0; i < map_portals.size(); i++)
	{
		map_portals[i]->Update(dt);
	}
	player->UpdateInMapScene(dt);
}

void CMapScene::Render()
{
	CGame* game = CGame::GetInstance();
	CHub* hub = CHub::GetInstance();
	map->DrawMap(0.0f, 0.0f);
	for (size_t i = 0; i < ghost_platforms.size(); i++)
	{
		ghost_platforms[i]->Render();
	}
	for (size_t i = 0; i < map_portals.size(); i++)
	{
		map_portals[i]->Render();
	}
	hub->Render();
	RenderItemHub();
	player->RenderInMapScene();
}

void CMapScene::Unload()
{
	for (LPGAMEOBJECT obj : map_portals)
		delete obj;
	map_portals.clear();

	current_portal = NULL;

	for (LPGAMEOBJECT obj : ghost_platforms)
		delete obj;
	ghost_platforms.clear();

	player = NULL;
	if (tiles != NULL) {
		tiles->ClearTileSet();
	}
	if (map != NULL) {
		map->ClearMap();
		delete map;
	}


	LPDIRECT3DTEXTURE9 tex = CTextures::GetInstance()->Get(-200);
	if (tex != NULL) tex->Release();
	tex = CTextures::GetInstance()->Get(-100);
	if (tex != NULL) tex->Release();
	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void CMapScene::_ParseSection_INFO(string line)
{

	if (line == "") return;

	world_id = atoi(line.c_str());
}

void CMapScene::_ParseSection_MAP(string line)
{
	if (line == "") return;
	wstring path = ToWSTR(line);
	this->map = new CMap();
	map->ReadMap(path.c_str());
}

void CMapScene::_ParseSection_TILESET(string line)
{
	// tách chuỗi nhận được thành những từ riêng biệt phân cách bởi dấu tab (line)
	vector<string> tokens = split(line);
	// nếu tổng số phần tử tách ra < 7 phần tử thì dữ liệu từ hàng đó bị sai
	if (tokens.size() < 7) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());

	// Hàm chuyển đổi chuỗi ToWSTR
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

void CMapScene::RenderItemHub()
{
	CHub* hub = CHub::GetInstance();
	float hub_x, hub_y;
	hub->GetHubPos(hub_x, hub_y);

	// all number in hub
	for (int i = 0; i < hub->numbers.size(); i++)
	{
		CFont* number = hub->GetFont(hub->numbers.at(i).id);
		number->Draw(hub->numbers.at(i).x + hub_x, hub->numbers.at(i).y + hub_y);
	}
	// cards
	vector<int> cards = player->GetCards();
	for (int i = 0; i < cards.size(); i++)
	{
		float x;
		int card_sprite_id = hub->GetCardId(cards.at(i));
		hub->GetCardPosX(x);
		CSprites::GetInstance()->Get(card_sprite_id)->DrawFlipX(hub_x + x + 72 * i, hub_y);
	}
}

void CMapScene::_ParseSection_STATIC_OBJECTS(string line)
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

void CMapScene::_ParseSection_MAP_SELECTION(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 9) return;


	int id = atof(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());

	int is_portal = atof(tokens[3].c_str());
	int l = atof(tokens[4].c_str());

	int t = atoi(tokens[5].c_str());
	int r = atoi(tokens[6].c_str());
	int b = atoi(tokens[7].c_str());
	int state = atoi(tokens[8].c_str());

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();

	CGameObject* obj = new CMapPortal(id, x, y, state, is_portal, l, t, r, b);

	map_portals.push_back(obj);
}

void CMapScene::GetHubStatistic()
{
	CHub* hub = CHub::GetInstance();
	for (int i = 0; i < TOTAL_NUMBER_IN_HUB; i++)
	{
		int n = 0;
		int temp;
		if (i == 0) {
			// get world number
			n = world_id;
		}
		else if (i >= 1 && i <= 2) {
			// get coin number

			temp = player->GetCoin();
			for (int j = 0; j < (COIN_LENGTH + 1) - i; j++)
			{
				n = temp % 10;
				temp = temp / 10;
			}
		}
		else if (i == 3) {
			// get map number
			n = player->GetLives();
		}
		else if (i >= 4 && i <= 10) {
			// get score
			temp = player->GetScore();
			// số lần chia (giá trị của j) sẽ lấy ra được con số ở vị trí đó
			for (int j = 0; j < (SCORE_LENGTH + 4) - i; j++) // push từ trái qua phải
			{
				n = temp % 10;
				temp = temp / 10;
			}
		}
		else if (i >= 11 && i <= 13) {
			// get time
			temp = 0;
			for (int j = 0; j < (TIME_LENGTH + 11) - i; j++) // push từ trái qua phải
			{
				n = temp % 10;
				temp = temp / 10;
			}
		}
		hub->numbers.at(i).id = n;
	}
}
