﻿#pragma once
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
#include "Flower.h"
#include "StaticCoin.h"
#include "FireFlowerWeapon.h"
#include "ButtonP.h"
#include "RandomBonus.h"
#include "MovingPlatform.h"
#include "BoomerangBro.h"
#include "Boomerang.h"

//173 186.25
#define CELL_HEIGHT (692/4.0f)
#define CELL_WIDTH (745/4.0f)

// 12 48
//#define MAX_ROW (int)(SCENE_HEIGHT/CELL_HEIGHT) + 1
//#define MAX_COLUMN (int)(SCENE_WIDTH/CELL_WIDTH) + 1
#define MAX_ROW 13
#define MAX_COLUMN 47


class CGrid
{
	bool hi = true;
	vector<LPGAMEOBJECT> total_obj;
	vector<LPGAMEOBJECT> enemies;
	vector<LPGAMEOBJECT> items;
	vector<LPGAMEOBJECT> bonus;


	// mỗi grid sẽ là vector GO
	vector<LPGAMEOBJECT> cells[MAX_ROW][MAX_COLUMN];

	LPCWSTR objFilePath;
	LPCWSTR gridFilePath;
	int current = 4;
public:
	vector<LPGAMEOBJECT> listResetObj;
	CGrid(LPCWSTR objFilePath, LPCWSTR gridFilePath);
	~CGrid() {};

	//void AddObjectIntoGridByFile(int object_type, float x, float y, float w, float h, int ani_id, int top, int bottom, int right, int left, int type, int extra = 0, int nx = 1, int angle = 0);
	void AddObjectIntoGrid(int object_type, float x, float y, float w, float h, int ani_id, int type, int extra = 0, int nx = 1, int angle = 0);
	void GetListObjInGrid(float cam_x, float cam_y);
	LPGAMEOBJECT CreateNewObj(int obj_type, float x, float y, float w, float h, int ani_id, int type, int extra = 0, int nx = 1, int angle = 1,int id_grid = 0);
	void Classify(LPGAMEOBJECT obj);
	void UpdatePositionInGrid(float cam_x, float cam_y);
	void ReadFileObj();
	void ReadFileGrid();
	void ReloadGrid();
	void ResetListObj(float cam_x, float cam_y);
	void ClearGrid();
};

