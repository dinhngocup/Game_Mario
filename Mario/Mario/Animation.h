#pragma once
#include "DefineLibs.h"
#include "AnimationFrame.h"
#include "Utils.h"
//#include "Mario.h"
/*
	Quản lý tất cả các frame (sprite + time display) của 1 animation
	với thời gian display 1 frame.
	thời gian display 1 frame trong 1 animation có thể lấy từ chính frame đó
	hoặc lấy defaultTime.
*/
class CAnimation
{
	DWORD lastFrameTime;
	int currentFrame;
	int defaultTime;

	bool lastFrame;
	bool nextIsLastFrame;

	vector<LPANIMATION_FRAME> frames;
public:
	CAnimation(int defaultTime = 100) { this->defaultTime = defaultTime; lastFrameTime = -1; currentFrame = -1; lastFrame = false; nextIsLastFrame = false; }
	void Add(int spriteId, DWORD time = 0);
	int GetCurrentFrame() {
		return currentFrame;
	}

	bool IsLastFrame() { return lastFrame; }
	bool NextIsLastFrame() { return nextIsLastFrame; }
	void ResetFlagLastFrame() {
		currentFrame = -1;
		lastFrame = false;
		nextIsLastFrame = false;
	}
	void Render(float x, float y, int alpha = 255, int nx = 1, int offset = 0, bool spinningFlag = false, int ny = 1, int scale = 3);
};

typedef CAnimation* LPANIMATION;

