#include "Grid.h"
#include "PlayScene.h"

CGrid::CGrid(LPCWSTR objFilePath)
{
	//DebugOut(L"new grid\n");
	this->objFilePath = objFilePath;
}

void CGrid::AddObjectIntoGrid(int object_type, float x, float y, float w, float h, int ani_id, int type, int extra, int nx)
{
	int top = (int)(y / CELL_HEIGHT);
	int bottom = (int)((y + h) / CELL_HEIGHT);
	int left = (int)(x / CELL_WIDTH);
	int right = (int)((x + w) / CELL_WIDTH);

	LPGAMEOBJECT obj = CreateNewObj(object_type, x, y, w, h, ani_id, type, extra, nx);

	for (int i = top; i <= bottom; i++)
		for (int j = left; j <= right; j++) {
			cells[i][j].push_back(obj);
		}
}

void CGrid::GetListObjInGrid(float cam_x, float cam_y)
{
	CGame* game = CGame::GetInstance();
	// clear toàn bộ các vector chứa obj trước
	enemies.clear();
	items.clear();
	bonus.clear();

	// tu vi tri cam, lay ra all obj, xet loai roi push vo vector, roi set lai vector trong playscene
	int top = (int)((cam_y) / CELL_HEIGHT);
	int bottom = (int)((cam_y + game->GetScreenHeight()) / CELL_HEIGHT) - 1;

	int left = (int)((cam_x) / CELL_WIDTH);
	int right = (int)((cam_x + game->GetScreenWidth()) / CELL_WIDTH) - 1;
	/*DebugOut(L"t %d\n", top);
	DebugOut(L"b %d\n", bottom);
	DebugOut(L"l %d\n", left);
	DebugOut(L"r %d\n", right);*/

	for (int i = top - 1; i <= bottom + 1; i++)
		for (int j = left - 1; j <= right + 1; j++) {
			if (left < 0) left = 0;
			for (int k = 0; k < cells[i][j].size(); k++) {
				if (cells[i][j].at(k)->GetHealth() && !cells[i][j].at(k)->is_in_grid) {
					Classify(cells[i][j].at(k));
					cells[i][j].at(k)->is_in_grid = true;
				}
			}
		}
	for (LPGAMEOBJECT obj : bonus)
		items.push_back(obj);
	bonus.clear();
	game->GetCurrentScene()->SetEnemiesInScene(enemies);
	game->GetCurrentScene()->SetItemsInScene(items);

}

LPGAMEOBJECT CGrid::CreateNewObj(int object_type, float x, float y, float w, float h, int ani_id, int type, int extra, int nx)
{
	//DebugOut(L"insert\n");
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
		obj = new CMushroom(x, y);
		obj->type = eTYPE::MUSHROOM;
		break;
	}
	case eTYPE::LEAF: {
		obj = new CLeaf(x, y);
		obj->type = eTYPE::LEAF;
		break;
	}
	case eTYPE::FIRE_FLOWER: {
		obj = new CFireFlower(extra);
		obj->start_y = y;
		obj->type = eTYPE::FIRE_FLOWER;
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
	int right_cell = (int)((cam_x + game->GetScreenWidth()) / CELL_WIDTH) - 1;
	enemies.clear();
	items.clear();


	CPlayScene* scene = (CPlayScene*)game->GetCurrentScene();
	enemies = scene->enemies;
	items = scene->items;

	for (int m = 0; m < enemies.size(); m++) {
		LPGAMEOBJECT enemy = enemies[m];

		for (int i = top_cell - 1; i <= bottom_cell + 1; i++)
			for (int j = left_cell - 1; j <= right_cell + 1; j++) {
				if (left_cell < 0) left_cell = 0;
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
		for (int i = top; i <= bottom; i++)
			for (int j = left; j <= right; j++) {
				cells[i][j].push_back(enemy);
			}
	}

	
	// lấy 2 dòng biên
	for (int x = left_cell - 1; x < right_cell + 1; x++) {
		for (int k = 0; k < cells[top_cell - 1][x].size(); k++) {
			if (cells[top_cell - 1][x].at(k)->GetHealth()
				&& cells[top_cell - 1][x].at(k)->type_object == eTYPE_OBJECT::ENEMY) {
				int quantity = 0;
				for (LPGAMEOBJECT obj : listResetObj)
					if (obj->GetId() == cells[top_cell - 1][x].at(k)->GetId()) {
						quantity++;
						break;
					}
				if (quantity == 0)
					listResetObj.push_back(cells[top_cell - 1][x].at(k));
			}
		}
		for (int k = 0; k < cells[bottom_cell + 1][x].size(); k++) {
			if (cells[bottom_cell + 1][x].at(k)->GetHealth()
				&& cells[bottom_cell + 1][x].at(k)->type_object == eTYPE_OBJECT::ENEMY) {
				int quantity = 0;
				for (LPGAMEOBJECT obj : listResetObj)
					if (obj->GetId() == cells[bottom_cell + 1][x].at(k)->GetId()) {
						quantity++;
						break;
					}
				if (quantity == 0) {
					listResetObj.push_back(cells[bottom_cell + 1][x].at(k));
				}
			}
		}
	}



	// lấy 2 cột biên
	for (int i = top_cell - 1; i <= bottom_cell + 1; i++) {
		for (int k = 0; k < cells[i][right_cell + 2].size(); k++) {
			if (cells[i][right_cell + 2].at(k)->GetHealth()
				&& cells[i][right_cell + 2].at(k)->type_object == eTYPE_OBJECT::ENEMY) {
				int quantity = 0;
				for (LPGAMEOBJECT obj : listResetObj)
					if (obj->GetId() == cells[i][right_cell + 2].at(k)->GetId()) {
						quantity++;
						break;
					}
				if (quantity == 0)
					listResetObj.push_back(cells[i][right_cell + 2].at(k));
			}
		}
		for (int k = 0; k < cells[i][left_cell - 2].size(); k++) {
			if (cells[i][left_cell - 2].at(k)->GetHealth()
				&& cells[i][left_cell - 2].at(k)->type_object == eTYPE_OBJECT::ENEMY) {
				if (left_cell < 0) left_cell = 0;
				int quantity = 0;
				for (LPGAMEOBJECT obj : listResetObj)
					if (obj->GetId() == cells[i][left_cell - 2].at(k)->GetId()) {
						quantity++;
						break;
					}
				if (quantity == 0)
					listResetObj.push_back(cells[i][left_cell - 2].at(k));

			}
		}


	}


	for (int m = 0; m < items.size(); m++) {
		LPGAMEOBJECT item = items[m];

		for (int i = top_cell; i <= bottom_cell; i++)
			for (int j = left_cell; j <= right_cell; j++) {
				if (left_cell < 0) left_cell = 0;
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
		if (tokens.size() < 7) continue;
		int object_type = atoi(tokens[0].c_str());
		float x = atof(tokens[1].c_str());
		float y = atof(tokens[2].c_str());

		float w = atof(tokens[3].c_str());
		float h = atof(tokens[4].c_str());

		int ani_id = atoi(tokens[5].c_str());
		int type = atoi(tokens[6].c_str());
		int extra = 0;
		if (object_type == 2 || object_type == 3 || object_type == 6 || object_type == 11)
			extra = atoi(tokens[7].c_str());

		AddObjectIntoGrid(object_type, x, y, w, h, ani_id, type, extra);
	}

	f.close();
}

void CGrid::ReloadGrid()
{
	DebugOut(L"reload grid\n");
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
	for (int i = 0; i < size; i++) {
		// check start pos coi có trong cam k có thì return
		if (listResetObj[i]->start_x >= cam_x && listResetObj[i]->start_x + listResetObj[i]->w <= cam_x + game->GetScreenWidth() + CELL_WIDTH)
			return;

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





