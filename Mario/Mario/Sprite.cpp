#include "Sprite.h"
#include "Game.h"
#include "Utils.h"

CSprite::CSprite(int id, int left, int top, int width, int height, LPDIRECT3DTEXTURE9 tex)
{
	this->id = id;
	this->left = left;
	this->top = top;
	this->right = left + width;
	this->bottom = top + height;
	this->texture = tex;
}

void CSprite::Draw(float x, float y, int alpha)
{
	CGame* game = CGame::GetInstance();
	game->Draw(x, y, texture, left, top, right, bottom, alpha);
}

void CSprite::DrawFlipX(float x, float y, int alpha, int nx, int offset, int ny, int scale)
{
	LPD3DXSPRITE spriteHandler = CGame::GetInstance()->GetSpriteHandler();

	float cam_x, cam_y;
	CGame::GetInstance()->GetCamPos(cam_x, cam_y);

	x += (right - left) * scale / 2;
	y += (bottom - top) * scale / 2;

	// tọa độ của sprite trên màn hình
	D3DXVECTOR3 p(floor(x - cam_x), floor(y - cam_y), 0);

	// tọa độ của sprite trong texture
	RECT r;
	r.left = left;
	r.top = top;
	r.right = right;
	r.bottom = bottom;

	// dùng ma trận middleTransform để tính ra các thông số transform
	// dùng ma trận newTransform để lưu lại ma trận sau khi tính -  tức là tile sau khi đã scale bự lên
	// dùng ma trận oldTransform để lưu lại tile 16x16
	D3DXMATRIX oldTransform, middleTransform;
	spriteHandler->GetTransform(&oldTransform);

	/*
	&D3DXVECTOR2(p.x , p.y): vị trí xoay sprite, với giá trị bằng đúng position thì xoay quanh tâm
	&D3DXVECTOR2(scale, scale): phóng đại theo hai chiều x, y nếu âm là lật frame ngược lại
	các thông số còn lại dùng cho việc xoay góc bao nhiêu độ,... ở đây vẽ tile không cần
	*/
	D3DXVECTOR2 vector_p(p.x, p.y);
	D3DXVECTOR2 vector_scale(scale * nx, scale * ny);

	D3DXMatrixTransformation2D(&middleTransform, &vector_p, 0, &vector_scale, NULL, 0.0f, NULL);

	D3DXVECTOR3 center = D3DXVECTOR3((float)(right - left) / 2 + offset, (float)(bottom - top) / 2, 0);

	D3DXMATRIX newTransform = oldTransform * middleTransform;
	spriteHandler->SetTransform(&newTransform);
	spriteHandler->Draw(texture, &r, &center, &p, D3DCOLOR_ARGB(alpha, 255, 255, 255));
	spriteHandler->SetTransform(&oldTransform);
}
