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
#include "AutoGoState.h"
#include "Font.h"
#include "Hub.h"
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

	time_limit = atoi(tokens[1].c_str());

	auto_go = atoi(tokens[2].c_str());
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

void CPlayScene::UpdateCamera(DWORD dt)
{
	CGame* game = CGame::GetInstance();
	float cx, cy;
	player->GetPosition(cx, cy);
	cx -= game->GetScreenWidth() / 2;
	cy -= game->GetScreenHeight() / 2;
	if (auto_go) {
		isMoved = true;
		game->SetCamYPos(DEFAULT_CAM_Y);
		if (game->GetCamX() >= 6144 - game->GetScreenWidth()
			&& game->GetCamX() < 6144) {
			game->SetCamPos(6144 - game->GetScreenWidth(), DEFAULT_CAM_Y);
		}
		else if (game->GetCamX() >= 6192) {

			if (cx >= sceneWidth - game->GetScreenWidth()) {
				game->SetCamPos(sceneWidth - game->GetScreenWidth(), DEFAULT_CAM_Y);
			}
			else {
				player->GetPosition(cx, cy);
				cx -= 384;
				if(cx > 6192)
					game->SetCamXPos(cx);
				else {
					game->SetCamXPos(6192.0f);
				}
			}
		}
		else {
			game->SetCamXPos(game->GetCamX() + 0.09f * dt);
		}
		

	}
	else {
		if (player->state != MARIO_STATE_DIE) {
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
		}
	}
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
	if (player->GetLevel() == 0) {
		player->SetLevel(MARIO_LEVEL_SMALL);
	}
	//player->SetPosition(7000, 1000);
	player->SetPosition(100, 1000);
	player->player_state = new CStandingState(player->GetLevel());
	player->SetState(MARIO_STATE_IDLE);
	player->nx = 1;

	LPANIMATION_SET ani_set = CAnimationSets::GetInstance()->Get(1);

	player->SetAnimationSet(ani_set);
	CHub* hub = CHub::GetInstance();

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

		switch (section) {
		case SCENE_SECTION_INFO:_ParseSection_INFO(line); break;
		case SCENE_SECTION_TILESET:_ParseSection_TILESET(line); break;
		case SCENE_SECTION_MAP: {_ParseSection_MAP(line); break; }
		case SCENE_SECTION_STATIC_OBJECTS: {_ParseSection_STATIC_OBJECTS(line); break; }
		case SCENE_SECTION_OBJECTS:_ParseSection_OBJECTS(line); break;
		}
	}

	f.close();

	CTextures::GetInstance()->Add(-100, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));
	CTextures::GetInstance()->Add(-200, L"textures\\bbox_black.png", D3DCOLOR_XRGB(255, 255, 255));


	DebugOut(L"[INFO] Done loading resources of this scene %s\n", sceneFilePath);
	time_game = time_limit;
	time_scale = 1;
	previousTime = GetTickCount64();
	CGame* game = CGame::GetInstance();
	game->SetCamYPos(DEFAULT_CAM_Y);
}

void CPlayScene::Update(DWORD dt)
{
	DWORD dt_after = dt * time_scale;
	if (player->is_attacking) {
		float x;
		if (player->nx > 0) {
			x = player->GetX() + MARIO_BIG_BBOX_WIDTH / 2;
		}
		else
			x = player->GetX() - MARIO_BIG_BBOX_WIDTH / 2;

		float y = player->GetY();
		grid->AddObjectIntoGrid(eTYPE::FIRE_BALL, x, y, 20, 0, 0, eTYPE_OBJECT::ITEM, 0, player->nx);

		if (player->press_z) {
			grid->AddObjectIntoGrid(eTYPE::FIRE_BALL, x - 20, y, 20, 0, 0, eTYPE_OBJECT::ITEM, 0, player->nx);
		}
		player->press_z = false;
		player->is_attacking = false;
	}
	CHub* hub = CHub::GetInstance();
	CGame* game = CGame::GetInstance();

	grid->GetListObjInGrid(game->GetCamX(), game->GetCamY());

	for (size_t i = 0; i < items.size(); i++)
	{
		if (items[i]->GetType() == eTYPE::RANDOM_BONUS) {
			items[i]->Update(dt);
		}
		else
			items[i]->Update(dt_after);
		items[i]->is_in_grid = false;
	}
	for (size_t i = 0; i < ghost_platforms.size(); i++)
	{
		ghost_platforms[i]->Update(dt_after);
	}
	for (size_t i = 0; i < enemies.size(); i++)
	{
		enemies[i]->Update(dt_after);
		enemies[i]->is_in_grid = false;
	}
	for (size_t i = 0; i < effects.size(); i++)
	{
		effects[i]->Update(dt);
	}

	player->Update(dt);

	UpdateHub(dt_after);

	grid->UpdatePositionInGrid(game->GetCamX(), DEFAULT_CAM_Y);

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return;

	// Update camera to follow mario
	float cx, cy;
	player->GetPosition(cx, cy);
	cx -= game->GetScreenWidth() / 2;
	cy -= game->GetScreenHeight() / 2;

	CMario* mario = CMario::GetInstance();


	UpdateCamera(dt);


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
	if (mario_end_bonus != 0) {
		EndScene(dt);
	}
	if (time_up && GetTickCount64() - time_up_count >= 3000) {
		DebugOut(L"handle time up\n");
		time_up = false;
		if (player->GetLives() > 0)
			player->MinusLive();
		else
			player->ResetLive();
		// chuyen ve scene map
		HandleMarioDie();
		return;
	}
	if (mario_die) {
		HandleMarioDie();
		return;
	}
}

void CPlayScene::Render()
{
	CHub* hub = CHub::GetInstance();
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
	RenderTitle();


}

/*
	Unload current scene
*/

void CPlayScene::Unload()
{
	for (LPGAMEOBJECT obj : enemies)
		delete obj;
	enemies.clear();

	for (LPGAMEOBJECT obj : items)
		delete obj;
	items.clear();

	for (LPGAMEOBJECT obj : ghost_platforms)
		delete obj;
	ghost_platforms.clear();

	for (LPGAMEOBJECT obj : effects)
		delete obj;
	effects.clear();
	if (grid != NULL) {
		grid->ClearGrid();
		delete grid;
	}
	player = NULL;
	if (tiles != NULL) {
		tiles->ClearTileSet();
	}
	if (map != NULL) {
		map->ClearMap();
		delete map;
	}


	LPDIRECT3DTEXTURE9 tex = CTextures::GetInstance()->Get(-100);
	if (tex != NULL) tex->Release();
	tex = CTextures::GetInstance()->Get(-200);
	if (tex != NULL) tex->Release();
	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void CPlayScene::UpdateHub(DWORD dt)
{
	CHub* hub = CHub::GetInstance();

	DWORD now = GetTickCount64();
	if (now - previousTime >= 1000 && time_scale != 0)
	{
		previousTime = GetTickCount64();
		// need to custom, time should be saved in hub
		if (time_game > 0)
			time_game--;
		else if (time_game == 0 && player->state != MARIO_STATE_DIE) {
			// xử lý end time
			DebugOut(L"end time\n");
			player->SetState(MARIO_STATE_DIE);
			time_up = true;
			time_up_count = GetTickCount64();
		}
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
			temp = time_game;
			for (int j = 0; j < (TIME_LENGTH + 11) - i; j++) // push từ trái qua phải
			{
				n = temp % 10;
				temp = temp / 10;
			}
		}
		hub->numbers.at(i).id = n;
	}
}

void CPlayScene::RenderItemHub()
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
		if (i != cards.size() - 1 || (i == cards.size() - 1 && !hide_card)) {
			float x;
			int card_sprite_id = hub->GetCardId(cards.at(i));
			hub->GetCardPosX(x);
			CSprites::GetInstance()->Get(card_sprite_id)->DrawFlipX(hub_x + x + 72 * i, hub_y);

		}
	}
}

void CPlayScene::RenderTitle()
{
	CHub* hub = CHub::GetInstance();
	CGame* game = CGame::GetInstance();
	if (allow_render_first_row_title)
		for (int i = 0; i < hub->end_scene_letters_1.size(); i++)
		{
			CFont* end_scene_letter = hub->GetFont(hub->end_scene_letters_1.at(i).id);
			end_scene_letter->Draw(game->GetCamX()+hub->end_scene_letters_1.at(i).x, hub->end_scene_letters_1.at(i).y);
		}
	if (allow_render_second_row_title) {
		for (int i = 0; i < hub->end_scene_letters_2.size(); i++)
		{
			CFont* end_scene_letter = hub->GetFont(hub->end_scene_letters_2.at(i).id);
			end_scene_letter->Draw(game->GetCamX() + hub->end_scene_letters_2.at(i).x, hub->end_scene_letters_2.at(i).y);

		}
		int card_sprite_id = hub->GetCardId(mario_end_bonus);
		CSprites::GetInstance()->Get(card_sprite_id)->DrawFlipX(game->GetCamX() + hub->card_in_title_X, hub->card_in_title_Y);

	}
	if (time_up) {
		for (int i = 0; i < hub->time_up_title.size(); i++)
		{
			CFont* number = hub->GetFont(hub->time_up_title.at(i).id);
			number->Draw(hub->time_up_title.at(i).x + game->GetCamX(), hub->time_up_title.at(i).y + game->GetCamY());
		}
	}

}

void CPlayScene::UpdateSpeedBar(float mario_speed)
{
	CHub* hub = CHub::GetInstance();
	if (mario_speed <= 0.3f) {
		for (int i = 15; i <= 20; i++) {
			hub->numbers.at(i).id = 38;
		}
		return;
	}

	int boundary = (mario_speed - 0.3) / (0.4 / 6);
	if (boundary > 5) boundary = 5;
	if (boundary < 0) boundary = 0;
	int out_bound = 15 + boundary + 1;

	for (int i = 15; i <= 15 + boundary; i++) {
		hub->numbers.at(i).id = 37;
	}
	for (int i = out_bound; i <= 20; i++) {
		hub->numbers.at(i).id = 38;
	}
	if (mario_speed >= 0.7) {
		if (!is_updated_bar) {
			previousTimeUpdateSpeedBar = GetTickCount64();
			is_updated_bar = true;
		}
		DWORD now = GetTickCount64();
		if (now - previousTimeUpdateSpeedBar > 200) {
			is_updated_bar = false;
			if (hub->numbers.at(21).id == 40)
				hub->numbers.at(21).id = 39;
			else
				hub->numbers.at(21).id = 40;
		}
	}
	else {
		hub->numbers.at(21).id = 40;
		is_updated_bar = false;
	}

}

void CPlayScene::HandleMarioDie()
{
	time_scale = 0;
	CGame* game = CGame::GetInstance();
	mario_die = false;
	player->SetLevel(MARIO_LEVEL_SMALL);
	// nếu đúng là chuyển qua scene world map nha !!!!!!

	delete player->player_state;
	game->SwitchScene(WORLD_MAP);
}

void CPlayScene::EndScene(DWORD dt)
{
	CMario* mario = CMario::GetInstance();
	CGame* game = CGame::GetInstance();
	// tí nữa hẳn bật =)))
	mario->MarioAutoGo();
	if (!allow_render_second_row_title &&
		GetTickCount64() - start_count >= 1000 && start_count != 0) {
		//DebugOut(L"endscene %d\n", mario_end_bonus);
		allow_render_second_row_title = true;
		mario->AddCard(mario_end_bonus);
		mario->AddScore(CARD_SCORE);
		start_count = GetTickCount64();
	}
	if (GetTickCount64() - start_count >= 3000 && start_count != 0 && time_game > 0) {
		DWORD now = GetTickCount64();
		if (start_flicker_card == 0)
			start_flicker_card = GetTickCount64();
		if (now - start_flicker_card >= 200) {
			hide_card = !hide_card;
			start_flicker_card = GetTickCount64();
		}
		if (now - previousTime >= 40)
		{
			previousTime = GetTickCount64();
			time_game--;
		}
		if (time_game == 0)
			start_count = GetTickCount64();

	}
	if (time_game == 0 && GetTickCount64() - start_count >= 2000) {
		//DebugOut(L"chuyen scene \n");
		hide_card = false;
		// nếu đúng là chuyển qua scene world map nha !!!!!!
		mario->up_scene = true;
		delete player->player_state;

		game->SwitchScene(WORLD_MAP);
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
