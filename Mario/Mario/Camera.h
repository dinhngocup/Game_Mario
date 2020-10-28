#pragma once
#include "DefineLibs.h"
/*
	Get/Set vị trí của camera
*/
class CCamera
{
	float cam_x = 0.0f;
	float cam_y = 0.0f;
	static CCamera* __instance;
	// Có nên set vx của CCamera = vx của mario hong
public:
	void getCamPos(float& x, float& y) {
		x = cam_x;
		y = cam_y;
	}
	void SetCamPos(float x, float y) { cam_x = x; cam_y = y; }

	CCamera* GetInstance();
};

