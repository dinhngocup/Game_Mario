#pragma once

#include "DefineLibs.h"

#include "Animation.h"

/*
	DB chứa animation của tất cả đối tượng trong game
	VD: animation đi qua trái, phải của mario, koopas.
*/
class CAnimations
{
	static CAnimations* __instance;

	unordered_map<int, LPANIMATION> animations;

public:
	void Add(int id, LPANIMATION ani);
	LPANIMATION Get(int id);
	void Clear();

	static CAnimations* GetInstance();
};


