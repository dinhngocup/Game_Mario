#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <unordered_map>


class CSprite
{
	int id;				// Sprite ID in the sprite database

	int left;
	int top;
	int right;
	int bottom;

	LPDIRECT3DTEXTURE9 texture;
public:
	CSprite(int id, int left, int top, int width, int height, LPDIRECT3DTEXTURE9 tex);

	void Draw(float x, float y, int alpha = 255);
	void DrawFlipX(float x, float y, int alpha = 255, int nx = 1, int offset = 0, int ny = 1);
	int Get() { return id; }
};
typedef CSprite* LPSPRITE;



