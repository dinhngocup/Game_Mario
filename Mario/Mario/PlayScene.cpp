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
#define SCENE_SECTION_OBJECTS	3

#define MAX_SCENE_LINE 1024

/*
	Parse a line in section [OBJECTS]
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return;

	int object_type = atoi(tokens[0].c_str());

	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());

	float w = atof(tokens[3].c_str());
	float h = atof(tokens[4].c_str());

	int ani_set_id = atoi(tokens[5].c_str());

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();

	CGameObject* obj = NULL;

	switch (object_type)
	{

	case eTYPE::BRICK: obj = new CBrick(x, y, w, h); obj->type = eTYPE::BRICK;  break;
	case eTYPE::INVISIBLE_OBJECT: obj = new CInvisibleObject(x, y, w, h); obj->type = eTYPE::INVISIBLE_OBJECT; break;
	case eTYPE::BRICK_QUESTION: obj = new CBrickQuestion(); obj->type = eTYPE::BRICK_QUESTION; break;
	case eTYPE::GOOMBA: obj = new CGoomba(); obj->type = eTYPE::GOOMBA; break;
	case eTYPE::KOOPA: {
		int state = atoi(tokens[6].c_str());
		obj = new CKoopa(state);
		obj->type = eTYPE::KOOPA;
		break;
	}
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);

	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

	obj->SetAnimationSet(ani_set);
	objects.push_back(obj);
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
	DebugOut(L"path ", path);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	int column = atoi(tokens[5].c_str());
	int row = atoi(tokens[6].c_str());

	tiles = CTiles::GetInstance();
	tiles->LoadResources(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B), column, row);
	tiles->LoadTiles();

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

void CPlayScene::LoadSceneResources()
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	player = CMario::GetInstance();
	player->SetPosition(0, 1000);
	player->SetLevel(MARIO_LEVEL_SMALL);
	player->ChangeState(new CStandingState(player->GetLevel()));

	LPANIMATION_SET ani_set = CAnimationSets::GetInstance()->Get(1);

	player->SetAnimationSet(ani_set);
	objects.push_back(player);

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
		if (line == "[OBJECTS]") { section = SCENE_SECTION_OBJECTS; continue; }

		switch (section) {
		case SCENE_SECTION_TILESET:_ParseSection_TILESET(line); break;
		case SCENE_SECTION_MAP: {_ParseSection_MAP(line); break; }
		case SCENE_SECTION_OBJECTS:_ParseSection_OBJECTS(line); break;
		}
	}

	f.close();

	CTextures::GetInstance()->Add(-100, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));


	DebugOut(L"[INFO] Done loading resources of this scene %s\n", sceneFilePath);

}

void CPlayScene::Update(DWORD dt)
{
	// update fire ball
	if (player->is_attacking) {
		CGameObject* obj = NULL;
		float x;
		if (player->nx > 0) {
			x = player->GetX() + MARIO_BIG_BBOX_WIDTH / 2;
		} else 
			x = player->GetX() - MARIO_BIG_BBOX_WIDTH / 2;

		float y = player->GetY();

		obj = new CFireBall(x, y, player->nx);
		//obj = new CFireBall(200, 1100);
		obj->type = eTYPE::FIRE_BALL;
		AddObject(obj);
		player->is_attacking = false;
	}

	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return;

	// Update camera to follow mario
	float cx, cy;
	player->GetPosition(cx, cy);
	CGame* game = CGame::GetInstance();
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


	// need to custom
	for (size_t i = 1; i < objects.size(); i++)
	{
		//if (objects[i]->GetType() == eTYPE::FIRE_BALL && objects[i]->GetX() >= game->GetCamX() + game->GetScreenWidth()) {
		if (!objects[i]->GetHealth()) {
			objects.erase(objects.begin() + i);
		}
	}

}

void CPlayScene::Render()
{
	if (!isMoved)
		map->DrawMap(0.0f, DEFAULT_CAM_Y);
	else {
		float cx, cy;
		CGame::GetInstance()->GetCamPos(cx, cy);
		map->DrawMap(cx, DEFAULT_CAM_Y);
	}

	for (int i = 0; i < objects.size(); i++)
		objects[i]->Render();
}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	player = NULL;
	map = NULL;
	tiles = NULL;
	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
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
