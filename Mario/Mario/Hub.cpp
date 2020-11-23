#include "Hub.h"
CHub* CHub::__instance = NULL;
CHub* CHub::GetInstance()
{
    if (__instance == NULL)
        __instance = new CHub();
    return __instance;
}

void CHub::AddFont(CFont* letter)
{
    letters.push_back(letter);
}


void CHub::Render()
{
    RenderBackgroundHub(hub_posX, hub_posY);
	CSprites::GetInstance()->Get(TEX_BLACK_BOARD_ID)->DrawFlipX(hub_posX, hub_posY);
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

void CHub::SetHubPos(float cam_x, float cam_y)
{
    CGame* game = CGame::GetInstance();
    hub_posX = cam_x + 20;
    hub_posY = cam_y + game->GetScreenHeight() - 116;
} 

void CHub::RenderBackgroundHub(float hub_posX, float hub_posY)
{
    CGame* game = CGame::GetInstance();
    float x_bg = hub_posX - 20;
    float y_bg = hub_posY - 30;
   
    RECT rect;

    LPDIRECT3DTEXTURE9 bbox = CTextures::GetInstance()->Get(-200);

    float l, t, r, b;

    rect.left = 0;
    rect.top = 0;
    rect.right = game->GetScreenWidth();
    rect.bottom = 146;

    CGame::GetInstance()->Draw(x_bg, y_bg, bbox, rect.left, rect.top, rect.right, rect.bottom, 255);


}
