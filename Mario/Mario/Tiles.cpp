#include "Tiles.h"
#include "Textures.h"
#include "DefineLibs.h"
#include "Game.h"
#include "Sprite.h"
CTiles* CTiles::__instance = NULL;

CTiles::CTiles()
{
}

void CTiles::LoadResources(int texID, LPCWSTR fileTileset, D3DCOLOR transparentColor, int column, int row)
{
	// Tại sao phải có một hàm add map texture riêng?
	// vì muốn get height và width của texture, nếu ko viết thêm hàm thì có cách nào truyển tham chiếu k
	CTextures::GetInstance()->AddMap(texID, fileTileset, transparentColor, frame_height, frame_width, column, row);

	column_tile = column;
	row_tile = row;

	tex = CTextures::GetInstance()->Get(texID);
}

int CTiles::GetFrameWidth()
{
	return frame_width;
}

int CTiles::GetFrameHeight()
{
	return frame_height;
}

void CTiles::ClearTileSet()
{
	for (LPSPRITE tile : tiles) {
		delete tile;
	}
	tiles.clear();

	LPDIRECT3DTEXTURE9 tex = CTextures::GetInstance()->Get(40);
	if (tex != NULL) tex->Release();


}

vector<LPSPRITE> CTiles::GetTiles()
{
	return tiles;
}

void CTiles::LoadTiles()
{
	// chỉ số i,j có thể đổi thành cột và hàng được ko?
	// nếu đổi thành cột và hàng thì khi push vào vector tiles sẽ push theo chiều dọc, cột này qua cột kia
	// trong khi tile map mình đọc theo hàng ngang, hàng này đến hàng kia
	CGame* game = CGame::GetInstance();
	int index = 0;
	for (int i = 0; i < row_tile; i++)
		for (int j = 0; j < column_tile; j++) {
			int top = i * frame_height;
			int left = j * frame_width;
			int width = frame_width;
			int height = frame_height;

			LPSPRITE tile = new CSprite(index, left, top, width, height, tex);
			tiles.push_back(tile);
			
			index++;

		}
}

CTiles* CTiles::GetInstance()
{
	if (__instance == NULL)
		__instance = new CTiles();
	return __instance;
}
