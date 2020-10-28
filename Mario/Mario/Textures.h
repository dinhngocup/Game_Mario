#pragma once
#include <unordered_map>
#include <d3dx9.h>


/*
	Manage texture database
*/
class CTextures
{
	static CTextures* __instance;

	unordered_map<int, LPDIRECT3DTEXTURE9> textures;

public:
	CTextures();
	void Add(int id, LPCWSTR filePath, D3DCOLOR transparentColor);
	void AddMap(int id, LPCWSTR filePath, D3DCOLOR transparentColor, int &frame_height, int &frame_width, int column, int row);
	LPDIRECT3DTEXTURE9 Get(unsigned int i);

	void Clear();
	static CTextures* GetInstance();
};

