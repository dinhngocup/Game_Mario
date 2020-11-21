#pragma once
#include "GameObject.h"
#include "Brick.h"
#include "BrickQuestion.h"
#include "Koopa.h"
#include "Goomba.h"
#include "InvisibleObject.h"
#include "GameConst.h"
#include "Coin.h"
#include "FireBall.h"
#include "Mushroom.h"
#include "Leaf.h"
#include "FireFlower.h"


#define CELL_HEIGHT (SCREEN_HEIGHT/4.0f)
#define CELL_WIDTH (SCREEN_WIDTH/4.0f)

#define MAX_ROW (int)(SCENE_HEIGHT/CELL_HEIGHT) + 1
#define MAX_COLUMN (int)(SCENE_WIDTH/CELL_WIDTH) + 1


class CGrid
{
	vector<LPGAMEOBJECT> enemies;
	vector<LPGAMEOBJECT> items;
	vector<LPGAMEOBJECT> bonus;

	// mỗi grid sẽ là vector GO
	vector<LPGAMEOBJECT> cells[MAX_ROW][MAX_COLUMN];

	LPCWSTR objFilePath;
	int current = 4;
public:
	CGrid(LPCWSTR objFilePath);
	~CGrid() {};

	void AddObjectIntoGrid(int object_type, float x, float y, float w, float h, int ani_id, int type, int extra = 0, int nx = 1);
	void GetListObjInGrid(float cam_x, float cam_y);
	LPGAMEOBJECT CreateNewObj(int obj_type, float x, float y, float w, float h, int ani_id, int type, int extra = 0, int nx = 1);
	void Classify(LPGAMEOBJECT obj);
	void UpdatePositionInGrid(float cam_x, float cam_y);
	void ReadFileObj();
	void ReloadGrid();
};

