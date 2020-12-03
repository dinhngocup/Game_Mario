#pragma once
#include "Utils.h"
#include "Sprite.h"
class CTiles
{
	int frame_height, frame_width;

	int column_tile, row_tile;


	// tiles là một vector chứa tile được cắt nhỏ từ tileset
	//vector<LPTILE> tiles;
	// Nên dùng CSprite hay CTile để lưu tile ?

	vector<LPSPRITE> tiles;
	
	static CTiles* __instance;

	LPDIRECT3DTEXTURE9 tex;

public:
	CTiles();

	// mỗi tile là một sprite được cắt từ tilemap 
	void LoadTiles();
	
	static CTiles* GetInstance();

	void LoadResources(int texID, LPCWSTR fileTileset, D3DCOLOR transparentColor, int column, int row);

	int GetFrameWidth();
	int GetFrameHeight();
	void ClearTileSet();
	vector<LPSPRITE> GetTiles();
};

