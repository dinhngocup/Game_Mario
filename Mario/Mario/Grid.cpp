#include "Grid.h"

CGrid::CGrid(LPCWSTR objFilePath)
{
	DebugOut(L"new grid\n");
	this->objFilePath = objFilePath;
}

void CGrid::AddObjectIntoGrid(int object_type, float x, float y, float w, float h, int ani_id, int type, int state)
{
	
	x -= w / 2;
	y -= h / 2;

	int top = (int)(y / CELL_HEIGHT);
	int bottom = (int)((y + h) / CELL_HEIGHT);
	int left = (int)(x / CELL_WIDTH);
	int right = (int)((x + w) / CELL_WIDTH);
	LPGAMEOBJECT obj = CreateNewObj(object_type, x, y, w, h, ani_id, type, state);

	

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

	/*DebugOut(L"left %d\n", left);
	DebugOut(L"right %d\n", right);*/

	for (int i = top; i <= bottom; i++)
		for (int j = left; j <= right; j++) {
			for (int k = 0; k < cells[i][j].size(); k++) {
				// obj is still alive
				if (cells[i][j].at(k)->GetHealth()) {
					Classify(cells[i][j].at(k));
				}
			}
		}
	CGame* game = CGame::GetInstance();

	game->GetCurrentScene()->SetEnemiesInScene(enemies);

}

LPGAMEOBJECT CGrid::CreateNewObj(int object_type, float x, float y, float w, float h, int ani_id, int type, int state)
{
	//DebugOut(L"insert\n");
	CGameObject* obj = NULL;
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	switch (object_type)
	{
	case eTYPE::BRICK_QUESTION:
		obj = new CBrickQuestion();
		obj->type = eTYPE::BRICK_QUESTION;

		break;
	case eTYPE::GOOMBA: {
		obj = new CGoomba(state);
		DebugOut(L"y %f\n", y);
		obj->type = eTYPE::GOOMBA;
		break; }
	case eTYPE::KOOPA: {
		DebugOut(L"new koopa\n");
		obj = new CKoopa(state);
		obj->type = eTYPE::KOOPA;
		break;
	}
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return NULL;
	}
	obj->type_object = type;

	// General object setup
	obj->SetPosition(x, y);

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
		int state = 0;
		if (object_type == 2 || object_type == 3)
			state = atoi(tokens[7].c_str());

		AddObjectIntoGrid(object_type, x, y, w, h, ani_id, type, state);
	}

	f.close();
}

void CGrid::ReloadGrid()
{
	//DebugOut(L"reload grid\n");
	for (int i = 0; i < MAX_ROW; i++)
		for (int j = 0; j < MAX_COLUMN; j++)
			cells[i][j].clear();
	ReadFileObj();
}


