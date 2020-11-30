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
#define SCENE_SECTION_INFO 0
#define SCENE_SECTION_TILESET	1
#define SCENE_SECTION_MAP	2
#define SCENE_SECTION_STATIC_OBJECTS	3
#define SCENE_SECTION_OBJECTS	4
#define SCENE_SECTION_HUB_PATH	5
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

void CPlayScene::_ParseSection_INFO(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines

	world_id = atoi(tokens[0].c_str());
	map_id = atoi(tokens[1].c_str());
	time_limit = atoi(tokens[2].c_str());
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
	// skip empty line
	if (line == "") return;

	wstring path = ToWSTR(line);
	ReadFileHub(path.c_str());
	

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

void CPlayScene::ReadFileHub(LPCWSTR filePath)
{
	ifstream f;
	f.open(filePath);

	int quantity, card_pos_X;
	f >> quantity >> card_pos_X;

	hub->SetCardPosX(card_pos_X);

	for (int i = 0; i < quantity; i++) {
		Number number;
		f >> number.x >> number.y >> number.id;
		numbers.push_back(number);
	}
	f.close();
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
	float new_cam_x, new_cam_y, start_x, start_y, direction_collision, y_change;
	if (object_type == eTYPE::PORTAL) {
		new_cam_x = atoi(tokens[7].c_str());
		new_cam_y = atoi(tokens[8].c_str());
		start_x = atoi(tokens[9].c_str());
		start_y = atoi(tokens[10].c_str());
		direction_collision = atoi(tokens[11].c_str());
		y_change = atoi(tokens[12].c_str());
	}
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();

	CGameObject* obj = NULL;

	switch (object_type)
	{
	case eTYPE::PORTAL: obj = new CPortal(x, y, w, h, new_cam_x, new_cam_y, start_x, start_y, direction_collision, y_change); obj->type = eTYPE::PORTAL;  break;
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

	ifstream f;
	f.open(sceneFilePath);

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
		if (line == "[OBJECTS]") { section = SCENE_SECTION_OBJECTS; continue; }
		if (line == "[HUB_PATH]") { section = SCENE_SECTION_HUB_PATH; continue; }
		if (line == "[FONT]") { section = SCENE_SECTION_FONT; continue; }

		switch (section) {
		case SCENE_SECTION_INFO:_ParseSection_INFO(line); break;
		case SCENE_SECTION_TILESET:_ParseSection_TILESET(line); break;
		case SCENE_SECTION_MAP: {_ParseSection_MAP(line); break; }
		case SCENE_SECTION_STATIC_OBJECTS: {_ParseSection_STATIC_OBJECTS(line); break; }
		case SCENE_SECTION_OBJECTS:_ParseSection_OBJECTS(line); break;
		case SCENE_SECTION_HUB_PATH:_ParseSection_HUB(line); break;
		case SCENE_SECTION_FONT:_ParseSection_FONT(line); break;
		}
	}

	f.close();

	CTextures::GetInstance()->Add(-100, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));
	CTextures::GetInstance()->Add(-200, L"textures\\bbox_black.png", D3DCOLOR_XRGB(255, 255, 255));


	DebugOut(L"[INFO] Done loading resources of this scene %s\n", sceneFilePath);
	time_game = time_limit;
	previousTime = GetTickCount64();
}

void CPlayScene::Update(DWORD dt)
{
	if (player->is_attacking) {
		float x;
		if (player->nx > 0) {
			x = player->GetX() + MARIO_BIG_BBOX_WIDTH / 2;
		}
		else
			x = player->GetX() - MARIO_BIG_BBOX_WIDTH / 2;

		float y = player->GetY();
		grid->AddObjectIntoGrid(eTYPE::FIRE_BALL, x, y, 20, 0, 0, eTYPE_OBJECT::ITEM, 0, player->nx);

		player->is_attacking = false;
	}

	CGame* game = CGame::GetInstance();

	grid->GetListObjInGrid(game->GetCamX(), game->GetCamY());

	for (size_t i = 0; i < items.size(); i++)
	{
		items[i]->Update(dt);
		items[i]->is_in_grid = false;
	}
	for (size_t i = 0; i < ghost_platforms.size(); i++)
	{
		ghost_platforms[i]->Update(dt);
	}
	for (size_t i = 0; i < enemies.size(); i++)
	{
		enemies[i]->Update(dt);
		enemies[i]->is_in_grid = false;
	}
	for (size_t i = 0; i < effects.size(); i++)
	{
		effects[i]->Update(dt);
	}
	player->Update(dt);
	UpdateHub(dt);

	grid->UpdatePositionInGrid(game->GetCamX(), DEFAULT_CAM_Y);

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return;

	// Update camera to follow mario
	float cx, cy;
	player->GetPosition(cx, cy);
	cx -= game->GetScreenWidth() / 2;
	cy -= game->GetScreenHeight() / 2;

	CMario* mario = CMario::GetInstance();
	if (mario->GetLevel() == RACCOON_LEVEL_BIG || mario->y < DEFAULT_CAM_Y) {
		if (dynamic_cast<CFallingState*>(mario->player_state) && cy >= DEFAULT_CAM_Y)
			mario->is_flying = false;

		if (mario->is_flying && cy < DEFAULT_CAM_Y) {
			if (cy <= 145.0f) {
				game->SetCamYPos(145.0f);
			}
			else
				game->SetCamYPos(cy);
		}
		else {
			if (game->GetCamY() == DEFAULT_CAM_Y)
				game->SetCamYPos(DEFAULT_CAM_Y);
		}

		if (cx <= 0)
		{
			game->SetCamXPos(0.0f);
		}
		else if (cx >= sceneWidth - game->GetScreenWidth()) {
			game->SetCamXPos(sceneWidth - game->GetScreenWidth());
		}
		else
		{
			game->SetCamXPos(cx);
			isMoved = true;
		}
	}
	else {
		if (cx <= 0)
		{
			game->SetCamPos(0.0f, DEFAULT_CAM_Y);
		}
		else if (cx >= sceneWidth - game->GetScreenWidth()) {
			game->SetCamPos(sceneWidth - game->GetScreenWidth(), DEFAULT_CAM_Y);
		}
		else
		{
			game->SetCamXPos(cx);
			isMoved = true;
		}
		if (mario->is_flying) {
			if (cy < DEFAULT_CAM_Y) {
				game->SetCamYPos(cy);
			}
			else {
				game->SetCamYPos(DEFAULT_CAM_Y);
				mario->is_flying = false;
			}
		}
	}

	for (size_t i = 0; i < enemies.size(); i++)
	{
		if (!enemies[i]->GetHealth()) {
			enemies.erase(enemies.begin() + i);

		}
	}
	for (size_t i = 0; i < items.size(); i++)
	{
		if (!items[i]->GetHealth()) {

			items.erase(items.begin() + i);
		}
	}
	for (size_t i = 0; i < effects.size(); i++)
	{
		if (!effects[i]->GetHealth()) {

			effects.erase(effects.begin() + i);
		}
	}
	hub->SetHubPos(game->GetCamX(), game->GetCamY());
}

void CPlayScene::Render()
{
	float cx, cy;
	CGame* game = CGame::GetInstance();
	game->GetCamPos(cx, cy);
	/*DebugOut(L"cam_x %f\n", cx);
	DebugOut(L"cam_y %f\n", cy);*/
	if (!isMoved) {
		map->DrawMap(0.0f, DEFAULT_CAM_Y);
	}
	else {
		map->DrawMap(cx, cy);

	}
	//map->DrawMap(cx, cy);


	for (size_t i = 0; i < enemies.size(); i++)
	{
		enemies[i]->Render();
	}
	player->Render();

	for (size_t i = 0; i < items.size(); i++)
	{
		items[i]->Render();
	}
	for (size_t i = 0; i < effects.size(); i++)
	{
		effects[i]->Render();
	}
	for (size_t i = 0; i < ghost_platforms.size(); i++)
	{
		ghost_platforms[i]->Render();
	}
	hub->Render();
	RenderItemHub();
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
	hub = NULL;
	numbers.clear();
	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void CPlayScene::UpdateHub(DWORD dt)
{
	DWORD now = GetTickCount64();
	if (now - previousTime >= 1000)
	{
		previousTime = GetTickCount64();
		// need to custom, time should be saved in hub
		time_game--;
	}

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
			n = map_id;
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
			temp = time_game;
			for (int j = 0; j < (TIME_LENGTH + 11) - i; j++) // push từ trái qua phải
			{
				n = temp % 10;
				temp = temp / 10;
			}
		}
		numbers.at(i).id = n;
	}
}

void CPlayScene::RenderItemHub()
{
	float hub_x, hub_y;
	hub->GetHubPos(hub_x, hub_y);

	// all number in hub
	for (int i = 0; i < numbers.size(); i++)
	{
		CFont* number = hub->GetFont(numbers.at(i).id);
		number->Draw(numbers.at(i).x + hub_x, numbers.at(i).y + hub_y);
	}
	// cards
	vector<int> cards = player->GetCards();
	for (int i = 0; i < cards.size(); i++)
	{
		int card_sprite_id = hub->GetCardId(cards.at(i));
		float x;
		hub->GetCardPosX(x);
		CSprites::GetInstance()->Get(card_sprite_id)->DrawFlipX(hub_x + x + 72 * i, hub_y);
	}
}

void CPlayScene::UpdateSpeedBar(float mario_speed)
{
	if (mario_speed <= 0.3f) {
		for (int i = 15; i <= 20; i++) {
			numbers.at(i).id = 38;
		}
		return;
	}

	int boundary = (mario_speed - 0.3) / (0.4 / 6);
	if (boundary > 5) boundary = 5;
	if (boundary < 0) boundary = 0;
	int out_bound = 15 + boundary + 1;

	for (int i = 15; i <= 15 + boundary; i++) {
		numbers.at(i).id = 37;
	}
	for (int i = out_bound; i <= 20; i++) {
		numbers.at(i).id = 38;
	}
	if (mario_speed >= 0.7) {
		if (!is_updated_bar) {
			previousTimeUpdateSpeedBar = GetTickCount64();
			is_updated_bar = true;
		}
		DWORD now = GetTickCount64();
		if (now - previousTimeUpdateSpeedBar > 200) {
			is_updated_bar = false;
			if (numbers.at(21).id == 40)
				numbers.at(21).id = 39;
			else
				numbers.at(21).id = 40;
		}
	}
	else {
		numbers.at(21).id = 40;
		is_updated_bar = false;
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
