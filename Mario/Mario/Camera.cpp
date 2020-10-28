#include "Camera.h"

CCamera* CCamera::__instance = NULL;
CCamera* CCamera::GetInstance()
{
	if (__instance == NULL)
		__instance = new CCamera();
	return __instance;
}
