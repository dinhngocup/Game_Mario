#include "Hub.h"
CHub* CHub::__instance = NULL;
CHub* CHub::GetInstance()
{
	DebugOut(L"new hub\n");
    if (__instance == NULL)
        __instance = new CHub();
    return __instance;
}

void CHub::AddFont(CFont* letter)
{
    letters.push_back(letter);
}


void CHub::Render(float cam_x, float cam_y)
{
	CSprites::GetInstance()->Get(TEX_BLACK_BOARD_ID)->DrawFlipX(cam_x, cam_y);
}

int CHub::GetCardId(int id)
{
    switch (id) {
    case MUSHROOM_CARD:
        return MUSHROOM_CARD_SPRITE_ID;
        break;
    case FIRE_FLOWER_CARD:
        return FIRE_FLOWER_CARD_SPRITE_ID;
        break;
    case STAR_MAN_CARD:
        return STAR_MAN_CARD_SPRITE_ID;
        break;
    default:
        return -1;
        break;

    }
}
