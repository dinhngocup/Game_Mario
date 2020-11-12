#pragma once
#include "GameObject.h"
#include "Brick.h"
#include "BrickQuestion.h"
#include "Koopa.h"
#include "Goomba.h"
#include "InvisibleObject.h"
#include "GameConst.h"

#define MAX_ROW 15
#define MAX_COLUMN 50

#define CELL_WIDTH (SCREEN_WIDTH/4.0f)
#define CELL_HEIGHT (SCREEN_HEIGHT/4.0f)
class CGrid
{
	vector<LPGAMEOBJECT> enemies;
	vector<LPGAMEOBJECT> items;

	// mỗi grid sẽ là vector GO
	vector<LPGAMEOBJECT> cells[MAX_ROW][MAX_COLUMN];

	LPCWSTR objFilePath;
public:
	CGrid(LPCWSTR objFilePath);
	~CGrid() {};

	void AddObjectIntoGrid(int object_type, float x, float y, float w, float h, int ani_id, int type, int state = 0);
	void GetListObjInGrid(float cam_x, float cam_y);
	LPGAMEOBJECT CreateNewObj(int obj_type, float x, float y, float w, float h, int ani_id, int type, int state = 0);
	void Classify(LPGAMEOBJECT obj);
	void ReadFileObj();
	void ReloadGrid();
};

