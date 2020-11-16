#include "Grid.h"
#include "PlayScene.h"

CGrid::CGrid(LPCWSTR objFilePath)
{
	//DebugOut(L"new grid\n");
	this->objFilePath = objFilePath;
}

void CGrid::AddObjectIntoGrid(int id, int object_type, float x, float y, float w, float h, int ani_id, int type, int state)
{
	int top = (int)(y / CELL_HEIGHT);
	int bottom = (int)((y + h) / CELL_HEIGHT);
	int left = (int)(x / CELL_WIDTH);
	int right = (int)((x + w) / CELL_WIDTH);

	LPGAMEOBJECT obj = CreateNewObj(id, object_type, x, y, w, h, ani_id, type, state);



	for (int i = top; i <= bottom; i++)
		for (int j = left; j <= right; j++) {
			cells[i][j].push_back(obj);
		}
}

void CGrid::GetListObjInGrid(float cam_x, float cam_y)
{
	// clear toàn bộ các vector chứa obj trước
	enemies.clear();
	items.clear();

	// tu vi tri cam, lay ra all obj, xet loai roi push vo vector, roi set lai vector trong playscene
	int top = (int)((cam_y) / CELL_HEIGHT);
	int bottom = (int)((cam_y + SCREEN_HEIGHT) / CELL_HEIGHT);

	int left = (int)((cam_x) / CELL_WIDTH);
	int right = (int)((cam_x + SCREEN_WIDTH) / CELL_WIDTH);



	/*if (current != right) {
		for (int i = top; i <= bottom; i++)
			for (int k = 0; k < cells[i][right].size(); k++)
				if (cells[i][right].at(k)->GetHealth()) {
					cells[i][right].at(k)->ResetPosition();
				}
		current = right;
	}*/


	//DebugOut(L"right %d\n", right);
	for (int i = top; i <= bottom; i++)
		for (int j = left - 1; j <= right; j++) {
			if (left < 0) left = 0;
			for (int k = 0; k < cells[i][j].size(); k++) {
				if (cells[i][j].at(k)->GetHealth()) {
					Classify(cells[i][j].at(k));
				}
			}
		}

	CGame* game = CGame::GetInstance();

	game->GetCurrentScene()->SetEnemiesInScene(enemies);
	game->GetCurrentScene()->SetItemsInScene(items);
	//DebugOut(L"size ene %d\n", enemies.size());

}

LPGAMEOBJECT CGrid::CreateNewObj(int id, int object_type, float x, float y, float w, float h, int ani_id, int type, int state)
{
	//DebugOut(L"insert\n");
	CGameObject* obj = NULL;
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	switch (object_type)
	{
	case eTYPE::BRICK_QUESTION: {
		obj = new CBrickQuestion();
		obj->SetStartPosition(x, y);
		obj->type = eTYPE::BRICK_QUESTION;
		break;
	}
	case eTYPE::GOOMBA: {
		obj = new CGoomba(state);
		obj->SetStartPosition(x, y);
		obj->type = eTYPE::GOOMBA;
		break;
	}
	case eTYPE::KOOPA: {
		obj = new CKoopa(state);
		obj->SetStartPosition(x, y);
		obj->type = eTYPE::KOOPA;
		break;
	}
	case eTYPE::COIN: {
		obj = new CCoin();
		obj->SetStartPosition(x, y);
		obj->type = eTYPE::COIN;
		break;
	}
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return NULL;
	}
	obj->type_object = type;

	// General object setup
	obj->SetPosition(x, y);
	obj->SetId(id);
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
		items.push_back(obj);
		break;
	default:
		DebugOut(L"ERROR typeee: %d\n", obj->type);
		break;
	}
}

void CGrid::UpdatePositionInGrid(float cam_x, float cam_y)
{
	int top_cell = (int)((cam_y) / CELL_HEIGHT);
	int bottom_cell = (int)((cam_y + SCREEN_HEIGHT) / CELL_HEIGHT);

	int left_cell = (int)((cam_x) / CELL_WIDTH) - 1;
	int right_cell = (int)((cam_x + SCREEN_WIDTH) / CELL_WIDTH);
	enemies.clear();
	items.clear();

	CGame* game = CGame::GetInstance();
	CPlayScene* scene = (CPlayScene*)game->GetCurrentScene();
	enemies = scene->enemies;
	items = scene->items;

	for (int m = 0; m < enemies.size(); m++) {
		LPGAMEOBJECT enemy = enemies[m];

		for (int i = top_cell; i <= bottom_cell; i++)
			for (int j = left_cell; j <= right_cell; j++) {
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

	/*for (int i = top_cell; i <= bottom_cell; i++) {
		for (int j = 0; j < cells[i][right_cell + 1].size(); j++) {
			cells[i][right_cell + 1].at(j)->ResetPosition();
		}
		for (int j = 0; j < cells[i][left_cell -1].size(); j++) {
			cells[i][left_cell - 1].at(j)->ResetPosition();
		}
	}*/



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
		int id = atoi(tokens[0].c_str());
		int object_type = atoi(tokens[1].c_str());
		float x = atof(tokens[2].c_str());
		float y = atof(tokens[3].c_str());

		float w = atof(tokens[4].c_str());
		float h = atof(tokens[5].c_str());

		int ani_id = atoi(tokens[6].c_str());
		int type = atoi(tokens[7].c_str());
		int state = 0;
		if (object_type == 2 || object_type == 3)
			state = atoi(tokens[8].c_str());

		AddObjectIntoGrid(id, object_type, x, y, w, h, ani_id, type, state);
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


