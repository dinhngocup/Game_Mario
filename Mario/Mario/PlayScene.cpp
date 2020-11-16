#include <iostream>
#include <fstream>

#include "PlayScene.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Animations.h"
#include "ObjectType.h"
#include "MarioConst.h"
#include "StandingState.h"
using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	key_handler = new CPlayScenceKeyHandler(this);
}

/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TILESET	1
#define SCENE_SECTION_MAP	2
#define SCENE_SECTION_STATIC_OBJECTS	3
#define SCENE_SECTION_OBJECTS	4
#define SCENE_SECTION_HUB	5
#define SCENE_SECTION_FONT	6

#define MAX_SCENE_LINE 1024



/*
	Parse a line in section [OBJECTS]
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	if (line == "") return;
	wstring path = ToWSTR(line);
	grid = new CGrid(path.c_str());
	grid->ReadFileObj();
}

void CPlayScene::_ParseSection_TILESET(string line)
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

void CPlayScene::_ParseSection_HUB(string line)
{
	vector<string> tokens = split(line);
	if (tokens.size() < 3) return;
	Number number;
	number.x = atoi(tokens[0].c_str());
	number.y = atoi(tokens[1].c_str());
	number.id = atoi(tokens[2].c_str());
	numbers.push_back(number);

}

void CPlayScene::_ParseSection_FONT(string line)
{
	vector<string> tokens = split(line);
	if (tokens.size() < 5) return;

	int l = atof(tokens[0].c_str());
	int t = atof(tokens[1].c_str());

	int w = atof(tokens[2].c_str());
	int h = atof(tokens[3].c_str());

	int textID = atoi(tokens[4].c_str());

	CFont* letter = new CFont(l, t, w, h, textID);
	hub->AddFont(letter);

}

void CPlayScene::_ParseSection_MAP(string line)
{
	// skip empty line
	if (line == "") return;

	wstring path = ToWSTR(line);
	this->map = new CMap();
	map->ReadMap(path.c_str());
	sceneWidth = map->GetWidthScene();
	sceneHeight = map->GetHeightScene();

}

void CPlayScene::_ParseSection_STATIC_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 7) return;

	int object_type = atoi(tokens[0].c_str());

	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());

	float w = atof(tokens[3].c_str());
	float h = atof(tokens[4].c_str());

	int ani_set_id = atoi(tokens[5].c_str());
	int type = atoi(tokens[6].c_str());

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();

	CGameObject* obj = NULL;

	switch (object_type)
	{

	case eTYPE::BRICK: obj = new CBrick(x, y, w, h); obj->type = eTYPE::BRICK;  break;
	case eTYPE::INVISIBLE_OBJECT: obj = new CInvisibleObject(x, y, w, h); obj->type = eTYPE::INVISIBLE_OBJECT; break;
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

void CPlayScene::LoadSceneResources()
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	player = CMario::GetInstance();
	player->SetPosition(100, 1000);
	player->SetLevel(MARIO_LEVEL_SMALL);
	player->ChangeState(new CStandingState(player->GetLevel()));

	LPANIMATION_SET ani_set = CAnimationSets::GetInstance()->Get(1);

	player->SetAnimationSet(ani_set);
	hub = CHub::GetInstance();
	previousTime = GetTickCount();
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
		if (line == "[OBJECTS]") { section = SCENE_SECTION_OBJECTS; continue; }
		if (line == "[HUB]") { section = SCENE_SECTION_HUB; continue; }
		if (line == "[FONT]") { section = SCENE_SECTION_FONT; continue; }

		switch (section) {
		case SCENE_SECTION_TILESET:_ParseSection_TILESET(line); break;
		case SCENE_SECTION_MAP: {_ParseSection_MAP(line); break; }
		case SCENE_SECTION_STATIC_OBJECTS: {_ParseSection_STATIC_OBJECTS(line); break; }
		case SCENE_SECTION_OBJECTS:_ParseSection_OBJECTS(line); break;
		case SCENE_SECTION_HUB:_ParseSection_HUB(line); break;
		case SCENE_SECTION_FONT:_ParseSection_FONT(line); break;
		}
	}

	f.close();

	CTextures::GetInstance()->Add(-100, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));


	DebugOut(L"[INFO] Done loading resources of this scene %s\n", sceneFilePath);

}

void CPlayScene::Update(DWORD dt)
{
	UpdateHub(dt);
	if (player->is_attacking) {
		CGameObject* obj = NULL;
		float x;
		if (player->nx > 0) {
			x = player->GetX() + MARIO_BIG_BBOX_WIDTH / 2;
		}
		else
			x = player->GetX() - MARIO_BIG_BBOX_WIDTH / 2;

		float y = player->GetY();

		obj = new CFireBall(x, y, player->nx);

		obj->type = eTYPE::FIRE_BALL;
		items.push_back(obj);
		player->is_attacking = false;
	}

	CGame* game = CGame::GetInstance();
	grid->GetListObjInGrid(game->GetCamX(), game->GetCamY());

	for (size_t i = 0; i < items.size(); i++)
	{
		items[i]->Update(dt);
	}
	for (size_t i = 0; i < enemies.size(); i++)
	{
		enemies[i]->Update(dt);
	}
	player->Update(dt);

	grid->UpdatePositionInGrid(game->GetCamX(), DEFAULT_CAM_Y);

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return;

	// Update camera to follow mario
	float cx, cy;
	player->GetPosition(cx, cy);
	cx -= game->GetScreenWidth() / 2;
	cy -= game->GetScreenHeight() / 2;


	if (cx <= 0)
	{
		game->SetCamPos(0.0f, DEFAULT_CAM_Y);
	}
	else if (cx >= sceneWidth - game->GetScreenWidth()) {
		game->SetCamPos(sceneWidth - game->GetScreenWidth(), DEFAULT_CAM_Y);

	}
	else
	{
		game->SetCamPos(cx, DEFAULT_CAM_Y);
		isMoved = true;
	}



	for (size_t i = 0; i < enemies.size(); i++)
	{
		if (!enemies[i]->GetHealth()) {
			LPGAMEOBJECT x = enemies[i];
			enemies.erase(enemies.begin() + i);
		}
	}
	for (size_t i = 0; i < items.size(); i++)
	{
		if (!items[i]->GetHealth()) {

			items.erase(items.begin() + i);
		}
	}

}

void CPlayScene::Render()
{
	float cx, cy;
	CGame* game = CGame::GetInstance();
	game->GetCamPos(cx, cy);
	if (!isMoved) {
		map->DrawMap(0.0f, DEFAULT_CAM_Y);
	}
	else {
		map->DrawMap(cx, cy);

	}



	for (size_t i = 0; i < enemies.size(); i++)
	{
		enemies[i]->Render();
	}
	player->Render();
	for (size_t i = 0; i < ghost_platforms.size(); i++)
	{
		ghost_platforms[i]->Render();
	}
	for (size_t i = 0; i < items.size(); i++)
	{
		items[i]->Render();
	}
	if (!isMoved) {
		hub->Render(20.0f, DEFAULT_CAM_Y + game->GetScreenHeight() - 125);
	}
	else {
		hub->Render(cx + 20.0f, cy + game->GetScreenHeight() - 125);
	}
	RenderHub();
}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	/*for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();*/
	for (LPGAMEOBJECT obj : enemies)
		delete obj;
	enemies.clear();

	for (LPGAMEOBJECT obj : items)
		delete obj;
	items.clear();

	player = NULL;
	map = NULL;
	tiles = NULL;
	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void CPlayScene::UpdateHub(DWORD dt)
{
	
	DWORD now = GetTickCount();
	if (now - previousTime >= 1000)
	{
		previousTime = GetTickCount();
		// need to custom, time should be saved in hub
		time--;
	}
	for (int i = 0; i < numbers.size(); i++)
	{
		int n = 0;
		int temp = time;
		//time
		// need to custom cause hub has another elements

		for (int j = 0; j < 3 - i; j++) // push từ trái qua phải
		{
			n = temp % 10;
			temp = temp / 10;
		}
		numbers.at(i).id = n;
	}
}

void CPlayScene::RenderHub()
{
	for (int i = 0; i < numbers.size(); i++)
	{
		CFont* number = hub->GetFont(numbers.at(i).id);
		number->Draw(numbers.at(i).x, numbers.at(i).y);
	}
}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	CMario* mario = ((CPlayScene*)scence)->GetPlayer();
	mario->OnKeyDown(KeyCode);
}

void CPlayScenceKeyHandler::OnKeyUp(int KeyCode)
{
	CMario* mario = ((CPlayScene*)scence)->GetPlayer();

	mario->OnKeyUp(KeyCode);

}

void CPlayScenceKeyHandler::KeyState(BYTE* states)
{
	CMario* mario = ((CPlayScene*)scence)->GetPlayer();
	mario->KeyState(states);
}
