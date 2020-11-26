#include "Map.h"
#include "Tiles.h"
#include "Game.h"
#include "DefineLibs.h"

CMap::CMap() {
	
};

void CMap::ReadMap(LPCWSTR filePath)
{
	ifstream f;
	f.open(filePath);
	f >> row_map >> column_map >> height_board;
	for (int i = 0; i < row_map; i++) {
		for (int j = 0; j < column_map; j++) {
			f >> tile_map[i][j];
		}
	}
	f.close();
	width_scene = CTiles::GetInstance()->GetFrameWidth() * column_map;
	height_scene = CTiles::GetInstance()->GetFrameHeight() * row_map;
	
}


void CMap::DrawMap(float cam_x, float cam_y)
{
	CGame* game = CGame::GetInstance();

	// get width height of tile
	int w = CTiles::GetInstance()->GetFrameWidth();
	int h = CTiles::GetInstance()->GetFrameHeight();
	
	int begin_row = cam_y / h;
	int end_row = (cam_y + game->GetScreenHeight() - height_board) / h + 1;

	int begin_column = cam_x / w;
	int end_column = (cam_x + game->GetScreenWidth()) / w +  1;

	if (end_column > column_map)
		end_column = column_map;
	if (end_row > row_map)
		end_row = row_map;
	if (begin_column < 0) begin_column = 0;
	if (begin_row < 0) begin_row = 0;
	/*DebugOut(L"begin row %d\n", begin_row);
	DebugOut(L"end row %d\n", end_row);
	DebugOut(L"begin c %d\n", begin_column);
	DebugOut(L"end c %d\n", end_column);*/
	vector<LPSPRITE> tiles = CTiles::GetInstance()->GetTiles();
	for (int i = begin_row; i < end_row; i++) {
		for (int j = begin_column; j < end_column; j++) {
			if (tile_map[i][j] != 0) {
				tiles[tile_map[i][j] - 1]->Draw(j * w, i * h, 255);
			}
		}
	}
}

int CMap::GetWidthScene()
{
	return width_scene;
}

int CMap::GetHeightScene()
{
	return height_scene;
}
