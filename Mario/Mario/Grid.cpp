#include "Grid.h"
#include "PlayScene.h"

CGrid::CGrid(LPCWSTR objFilePath, LPCWSTR gridFilePath)
{
	DebugOut(L"new\n");
	this->objFilePath = objFilePath;
	this->gridFilePath = gridFilePath;
}

void CGrid::AddObjectIntoGrid(int object_type, float x, float y, float w, float h, int ani_id, int type, int extra, int nx, int angle)
{
	int top = (int)(y / CELL_HEIGHT);
	int bottom = (int)((y + h) / CELL_HEIGHT);
	int left = (int)(x / CELL_WIDTH);
	int right = (int)((x + w) / CELL_WIDTH);

	LPGAMEOBJECT obj = CreateNewObj(object_type, x, y, w, h, ani_id, type, extra, nx, angle);
	
	for (int i = top; i <= bottom; i++)
		for (int j = left; j <= right; j++) {
			cells[i][j].push_back(obj);
		}
}
void CGrid::GetListObjInGrid(float cam_x, float cam_y)
{
	/*DebugOut(L"******************\n");

	for (int i = 1; i <= 7; i++)
		for (int j = 1; j <= 41; j++) {
			if (cells[i][j].size() != 0) {
				DebugOut(L"%d\t", i);
				DebugOut(L"%d\t", j);
				for (int k = 0; k < cells[i][j].size(); k++)
					if(k == cells[i][j].size() - 1)
					DebugOut(L"%d", cells[i][j].at(k)->id_grid);
					else
					DebugOut(L"%d\t", cells[i][j].at(k)->id_grid);
				DebugOut(L"\n");
			}
		}*/

	CGame* game = CGame::GetInstance();
	// clear toàn bộ các vector chứa obj trước
	enemies.clear();
	items.clear();
	bonus.clear();

	// tu vi tri cam, lay ra all obj, xet loai roi push vo vector, roi set lai vector trong playscene
	int top = (int)((cam_y) / CELL_HEIGHT);
	int bottom = (int)((cam_y + game->GetScreenHeight()) / CELL_HEIGHT) - 1;

	int left = (int)((cam_x) / CELL_WIDTH);
	int right = (int)((cam_x + game->GetScreenWidth()) / CELL_WIDTH);

	for (int i = top - 1; i <= bottom + 1; i++)
		for (int j = left - 1; j <= right + 1; j++) {
			if (j < 0) j = 0;
			if (i < 0) i = 0;
				/*DebugOut(L"i %d\n", i);
				DebugOut(L"j %d\n", j);
				DebugOut(L"size %d\n", cells[i][j].size());*/

			for (int k = 0; k < cells[i][j].size(); k++) {
				//DebugOut(L"id %d\n", cells[i][j].at(k)->GetId());
				if (cells[i][j].at(k)->GetHealth()) {
					if (j >= left && j <= right)
						cells[i][j].at(k)->is_appeared = true;
					if (!cells[i][j].at(k)->is_in_grid && cells[i][j].at(k)->is_appeared) {
						Classify(cells[i][j].at(k));
						cells[i][j].at(k)->is_in_grid = true;
					}

				}
			}
		}
	for (LPGAMEOBJECT obj : bonus)
		items.push_back(obj);
	bonus.clear();
	//DebugOut(L"size %d\n", items.size());
	game->GetCurrentScene()->SetEnemiesInScene(enemies);
	game->GetCurrentScene()->SetItemsInScene(items);

}

LPGAMEOBJECT CGrid::CreateNewObj(int object_type, float x, float y, float w, float h, int ani_id, int type, int extra, int nx, int angle,int id_grid)
{
	CGameObject* obj = NULL;
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	switch (object_type)
	{
	case eTYPE::BRICK_QUESTION: {
		obj = new CBrickQuestion(extra);
		obj->SetStartPosition(x, y);
		obj->type = eTYPE::BRICK_QUESTION;
		break;
	}
	case eTYPE::GOOMBA: {
		obj = new CGoomba(extra);
		obj->SetStartPosition(x, y);
		obj->type = eTYPE::GOOMBA;
		break;
	}
	case eTYPE::KOOPA: {
		obj = new CKoopa(extra);
		obj->SetStartPosition(x, y);
		obj->type = eTYPE::KOOPA;
		break;
	}
	case eTYPE::FIRE_BALL: {
		obj = new CFireBall(x, y, nx);
		obj->type = eTYPE::FIRE_BALL;
		obj->type_object = type;
		return obj;
	}
	case eTYPE::COIN: {
		obj = new CCoin(x, y);
		obj->type = eTYPE::COIN;
		break;
	}
	case eTYPE::MUSHROOM: {
		obj = new CMushroom(x, y, extra);
		obj->type = eTYPE::MUSHROOM;
		break;
	}
	case eTYPE::LEAF: {
		obj = new CLeaf(x, y);
		obj->type = eTYPE::LEAF;
		break;
	}
	case eTYPE::FIRE_FLOWER: {
		obj = new CFireFlower(extra, x, y, w, h);
		obj->type = eTYPE::FIRE_FLOWER;
		break;
	}
	case eTYPE::STATIC_COIN: {
		obj = new CStaticCoin(x, y);
		obj->type = eTYPE::STATIC_COIN;
		break;
	}
	case eTYPE::FIRE_FLOWER_WEAPON: {
		obj = new CFireFlowerWeapon(x, y, nx, extra, angle);
		obj->type = eTYPE::FIRE_FLOWER_WEAPON;
		break;
	}
	case eTYPE::FLOWER: {
		obj = new CFlower(x, y, w, h);
		obj->type = eTYPE::FLOWER;
		break;
	}
	case eTYPE::BRICK: {
		obj = new CBrick(x, y, w, h, BLING_BLING_BRICK, extra);
		obj->type = eTYPE::BRICK;
		break;
	}
	case eTYPE::BUTTON_P: {
		obj = new CButtonP(x, y);
		obj->type = eTYPE::BUTTON_P;
		break;
	}
	case eTYPE::RANDOM_BONUS: {
		obj = new CRandomBonus(x, y);
		obj->type = eTYPE::RANDOM_BONUS;
		break;
	}
	case eTYPE::MOVING_PLATFORM: {
		obj = new CMovingPlatform();
		obj->type = eTYPE::MOVING_PLATFORM;
		break;
	}
	case eTYPE::BOOMERANG_BRO: {
		obj = new CBoomerangBro();
		obj->type = eTYPE::BOOMERANG_BRO;
		obj->SetStartPosition(x, y);
		break;
	}
	case eTYPE::BOOMERANG: {
		obj = new CBoomerang(x, y);
		obj->type = eTYPE::BOOMERANG;
		break;
	}
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return NULL;
	}
	obj->type_object = type;

	// General object setup
	obj->SetPosition(x, y);
	obj->h = h;
	obj->w = w;
	LPANIMATION_SET ani_set = animation_sets->Get(ani_id);
	if (id_grid != 0)
		obj->id_grid = id_grid;
	obj->SetAnimationSet(ani_set);
	return obj;
}

void CGrid::Classify(LPGAMEOBJECT obj)
{
	switch (obj->type_object) {
	case eTYPE_OBJECT::ENEMY:
		enemies.push_back(obj);
		break;
	case eTYPE_OBJECT::ITEM:
		if (obj->type == eTYPE::BRICK_QUESTION)
			items.push_back(obj);
		else {
			bonus.push_back(obj);
		}
		break;
	default:
		DebugOut(L"ERROR typeee: %d\n", obj->type);
		break;
	}
}

void CGrid::UpdatePositionInGrid(float cam_x, float cam_y)
{
	CGame* game = CGame::GetInstance();
	int top_cell = (int)((cam_y) / CELL_HEIGHT);
	int bottom_cell = (int)((cam_y + game->GetScreenHeight()) / CELL_HEIGHT) - 1;

	int left_cell = (int)((cam_x) / CELL_WIDTH);
	int right_cell = (int)((cam_x + game->GetScreenWidth()) / CELL_WIDTH);
	enemies.clear();
	items.clear();


	CPlayScene* scene = (CPlayScene*)game->GetCurrentScene();
	enemies = scene->enemies;
	items = scene->items;

	for (int m = 0; m < enemies.size(); m++) {
		LPGAMEOBJECT enemy = enemies[m];

		for (int i = top_cell - 1; i <= bottom_cell + 1; i++)
			for (int j = left_cell - 2; j <= right_cell + 3; j++) {
				if (j < 0) j = 0;
				if (i < 0) i = 0;
				for (int k = 0; k < cells[i][j].size(); k++) {
					if (cells[i][j].at(k)->GetId() == enemy->GetId()) {
						cells[i][j].erase(cells[i][j].begin() + k);
					}
				}
			}

		int top = (int)(enemy->GetY() / CELL_HEIGHT);
		int bottom = (int)((enemy->GetY() + enemy->h) / CELL_HEIGHT);
		int left = (int)(enemy->GetX() / CELL_WIDTH);
		int right = (int)((enemy->GetX() + enemy->w) / CELL_WIDTH);

		//DebugOut(L"left %d\n", left);
		//DebugOut(L"right %d\n", right);
		for (int i = top; i <= bottom; i++)
			for (int j = left; j <= right; j++) {
				cells[i][j].push_back(enemy);
			}
	}


	// lấy 2 dòng biên
	for (int x = left_cell - 1; x < right_cell + 1; x++) {
		if (x < 0) x = 0;
		//if (top_cell - 1 < 0) top_cell = 1;
		//for (int k = 0; k < cells[top_cell - 1][x].size(); k++) {
		//	if (cells[bottom_cell + 1][x].at(k)->GetHealth()
		//		&& cells[top_cell - 1][x].at(k)->type_object == eTYPE_OBJECT::ENEMY &&
		//		cells[top_cell - 1][x].at(k)->is_appeared) {
		//		int quantity = 0;
		//		for (LPGAMEOBJECT obj : listResetObj)
		//			if (obj->GetId() == cells[top_cell - 1][x].at(k)->GetId()) {
		//				quantity++;
		//				break;
		//			}
		//		if (quantity == 0) {
		//			//DebugOut(L"push 2 dong bien\n");
		//			cells[top_cell - 1][x].at(k)->is_appeared = false;
		//			listResetObj.push_back(cells[top_cell - 1][x].at(k));
		//		}
		//	}
		//}
		for (int k = 0; k < cells[bottom_cell + 1][x].size(); k++) {
			if (cells[bottom_cell + 1][x].at(k)->ableToCheckCollision
				&& cells[bottom_cell + 1][x].at(k)->type_object == eTYPE_OBJECT::ENEMY &&
				cells[bottom_cell + 1][x].at(k)->is_appeared) {
				int quantity = 0;

				for (LPGAMEOBJECT obj : listResetObj)
					if (obj->GetId() == cells[bottom_cell + 1][x].at(k)->GetId()) {
						quantity++;
						break;
					}
				if (quantity == 0) {
					//DebugOut(L"push 2 dong bien\n");
					cells[bottom_cell + 1][x].at(k)->is_appeared = false;
					listResetObj.push_back(cells[bottom_cell + 1][x].at(k));
				}
			}
		}
	}



	// lấy 2 cột biên
	for (int i = top_cell - 1; i <= bottom_cell + 1; i++) {
		if (i < 0) i = 0;
		for (int k = 0; k < cells[i][right_cell + 2].size(); k++) {
			if (cells[i][right_cell + 2].at(k)->GetHealth()
				&& cells[i][right_cell + 2].at(k)->type_object == eTYPE_OBJECT::ENEMY
				&& cells[i][right_cell + 2].at(k)->is_appeared) {
				int quantity = 0;
				for (LPGAMEOBJECT obj : listResetObj)
					if (obj->GetId() == cells[i][right_cell + 2].at(k)->GetId()) {
						quantity++;
						break;
					}
				if (quantity == 0) {
					//DebugOut(L"push cot bien phai\n");
					cells[i][right_cell + 2].at(k)->is_appeared = false;
					listResetObj.push_back(cells[i][right_cell + 2].at(k));

				}
			}
		}
		for (int k = 0; k < cells[i][left_cell - 2].size(); k++) {
			if (left_cell - 1 < 0) left_cell = 2;
			if (cells[i][left_cell - 2].at(k)->GetHealth()
				&& cells[i][left_cell - 2].at(k)->type_object == eTYPE_OBJECT::ENEMY
				&& cells[i][left_cell - 2].at(k)->is_appeared) {
				if (left_cell < 0) left_cell = 0;
				int quantity = 0;
				for (LPGAMEOBJECT obj : listResetObj)
					if (obj->GetId() == cells[i][left_cell - 2].at(k)->GetId()) {
						quantity++;
						break;
					}
				if (quantity == 0) {
					//DebugOut(L"push 2 cot bien trai\n");
					cells[i][left_cell - 2].at(k)->is_appeared = false;
					listResetObj.push_back(cells[i][left_cell - 2].at(k));

				}

			}
		}


	}

	for (int m = 0; m < items.size(); m++) {
		LPGAMEOBJECT item = items[m];

		for (int i = top_cell - 1; i <= bottom_cell + 1; i++)
			for (int j = left_cell - 2; j <= right_cell + 3; j++) {
				if (j < 0) j = 0;
				if (i < 0) i = 0;
				for (int k = 0; k < cells[i][j].size(); k++) {
					if (cells[i][j].at(k)->GetId() == item->GetId()) {
						cells[i][j].erase(cells[i][j].begin() + k);
					}
				}
			}

		int top = (int)(item->GetY() / CELL_HEIGHT);
		int bottom = (int)((item->GetY() + item->h) / CELL_HEIGHT);
		int left = (int)(item->GetX() / CELL_WIDTH);
		int right = (int)((item->GetX() + item->w) / CELL_WIDTH);
		for (int i = top; i <= bottom; i++)
			for (int j = left; j <= right; j++) {
				cells[i][j].push_back(item);
			}
	}

	ResetListObj(cam_x, cam_y);

}

void CGrid::ReadFileObj()
{
	ifstream f;
	f.open(objFilePath);
	char str[MAX_SCENE_LINE];

	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);
		vector<string> tokens = split(line);
		if (line[0] == '#') {
			continue;
		}
		if (tokens.size() < 8) continue;
		int id_grid = atoi(tokens[0].c_str());
		int object_type = atoi(tokens[1].c_str());
		float x = atof(tokens[2].c_str());
		float y = atof(tokens[3].c_str());

		float w = atof(tokens[4].c_str());
		float h = atof(tokens[5].c_str());

		int ani_id = atoi(tokens[6].c_str());


		int type = atoi(tokens[7].c_str());
		int extra = 0;
		if (object_type == 2 || object_type == 3 ||
			object_type == 6 || object_type == 11 ||
			object_type == 1)
			extra = atoi(tokens[8].c_str());

		//AddObjectIntoGrid(object_type, x, y, w, h, ani_id, type, extra);
		LPGAMEOBJECT obj = CreateNewObj(object_type, x, y, w, h, ani_id, type, extra,1,1,id_grid);
		total_obj.push_back(obj);
	}

	f.close();
}

void CGrid::ReadFileGrid()
{
	//objs.clear();
	ifstream f;
	f.open(gridFilePath);
	char str[MAX_SCENE_LINE];

	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);
		vector<string> tokens = split(line);
		if (line[0] == '#') {
			continue;
		}
		if (tokens.size() < 3) continue;
		int i = atoi(tokens[0].c_str());
		int j = atoi(tokens[1].c_str());
		for (int k = 2; k < tokens.size(); k++) {
			int id = atoi(tokens[k].c_str());
			for (LPGAMEOBJECT obj : total_obj)
				if (obj->id_grid == id) {
					cells[i][j].push_back(obj);
				}
			
		}
	}

	f.close();
}

void CGrid::ReloadGrid()
{
	//DebugOut(L"reload grid\n");
	current = -1;
	for (int i = 0; i < MAX_ROW; i++)
		for (int j = 0; j < MAX_COLUMN; j++)
			cells[i][j].clear();
	ReadFileObj();
}

void CGrid::ResetListObj(float cam_x, float cam_y)
{
	CGame* game = CGame::GetInstance();
	int size = listResetObj.size();
	//DebugOut(L"size %d\n", size);

	for (int i = 0; i < size; i++) {
		if (!listResetObj[i]->GetHealth()) {
			listResetObj.erase(listResetObj.begin() + i);
			size--;
		}
		else {
			// check start pos coi có trong cam k có thì return
			if (listResetObj[i]->start_x + listResetObj[i]->w >= cam_x - 200 && listResetObj[i]->start_x <= cam_x + game->GetScreenWidth() + 200) {

			}
			else {
				//DebugOut(L"type %d\n", listResetObj[i]->GetType());
							// tính hiện tại nó ở grid nào
				int top_cell = (int)((listResetObj[i]->y) / CELL_HEIGHT);
				int bottom_cell = (int)((listResetObj[i]->y + listResetObj[i]->h) / CELL_HEIGHT);

				int left_cell = (int)((listResetObj[i]->x) / CELL_WIDTH);
				int right_cell = (int)((listResetObj[i]->x + listResetObj[i]->w) / CELL_WIDTH);

				// xóa khỏi grid hiện tại
				for (int m = top_cell; m <= bottom_cell; m++)
					for (int n = left_cell; n <= right_cell; n++) {
						for (int k = 0; k < cells[m][n].size(); k++) {
							if (cells[m][n].at(k)->GetId() == listResetObj[i]->GetId()) {

								cells[m][n].erase(cells[m][n].begin() + k);
							}
						}
					}
				// add vô đúng grid của nó
				int top = (int)((listResetObj[i]->start_y) / CELL_HEIGHT);
				int bottom = (int)((listResetObj[i]->start_y + listResetObj[i]->h) / CELL_HEIGHT);

				int left = (int)((listResetObj[i]->start_x) / CELL_WIDTH);
				int right = (int)((listResetObj[i]->start_x + listResetObj[i]->w) / CELL_WIDTH);
				for (int m = top; m <= bottom; m++)
					for (int n = left; n <= right; n++) {
						listResetObj[i]->is_in_grid = false;
						listResetObj[i]->ResetPosition();
						cells[m][n].push_back(listResetObj[i]);
					}
				listResetObj.erase(listResetObj.begin() + i);
				size--;
			}


		}



	}
}

void CGrid::ClearGrid()
{
	total_obj.clear();

	enemies.clear();


	items.clear();


	bonus.clear();

	/*for (LPGAMEOBJECT obj : listResetObj)
		delete obj;*/
	listResetObj.clear();
}





