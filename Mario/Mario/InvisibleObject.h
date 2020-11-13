#pragma once
#include "GameObject.h"
#include "Textures.h"
#include "Game.h"
class CInvisibleObject : public CGameObject
{
private:
	float width;
	float height;
public:
	CInvisibleObject() {};
	CInvisibleObject(float x, float y, float width, float height);
	~CInvisibleObject() {};

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx = 0, int dy = 0);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();

	float GetWidth() { return width; }
	float GetHeight() { return height; }
	void SetWidth(float width) { this->width = width; }
	void SetHeight(float height) { this->height = height; }

};

