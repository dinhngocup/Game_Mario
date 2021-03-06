#pragma once
#include "GameObject.h"
#include "Textures.h"
#include "Game.h"
#include "Mario.h"
class CPortal : public CGameObject
{
public:
	float new_cam_x, new_cam_y, start_x_mario, start_y_mario, y_change;
	DWORD start_hide;
	bool is_activated = false;
	int direction_collision;
	CPortal() {};
	CPortal(float x, float y, float width, float height, float cam_x, float cam_y, float start_x, float start_y, int direction_collision, float y_change);
	~CPortal() {};

	void GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx = 0, int dy = 0);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void Render();

	void MoveCamera();
};


