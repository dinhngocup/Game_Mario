#pragma once
#include "DefineLibs.h"
#include "Tiles.h"
class CMap
{

	int column_map, row_map;

	int height_board;

	int tile_map[500][500];

	int width_scene;
	int height_scene;

public:
	CMap();
	void ReadMap(LPCWSTR filePath);
	void DrawMap(float cam_x, float cam_y);
	int GetWidthScene();
	int GetHeightScene();
};

