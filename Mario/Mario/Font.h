#pragma once
#include "DefineLibs.h"
#include "Textures.h"

class CFont
{
	int left, top, w, h;
	int textID;

public:
	CFont(int left, int top, int w, int h, int textID);
	void Draw(float x, float y);
};

