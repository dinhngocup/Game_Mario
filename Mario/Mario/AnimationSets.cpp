#include "AnimationSets.h"
#include "Utils.h"
CAnimationSets* CAnimationSets::__instance = NULL;
CAnimationSets* CAnimationSets::GetInstance()
{
	if (__instance == NULL) __instance = new CAnimationSets();
	return __instance;
}

CAnimationSets::CAnimationSets()
{
}

void CAnimationSets::Add(int id, LPANIMATION_SET ani)
{
	animation_sets[id] = ani;
}

LPANIMATION_SET CAnimationSets::Get(unsigned int id)
{
	LPANIMATION_SET animation_set = animation_sets[id];
	if(animation_set == NULL)
		DebugOut(L"[ERROR] Failed to find animation set id: %d\n", id);
	return animation_set;
}
