#include "Portal.h"

CPortal::CPortal(float x, float y, float width, float height, float cam_x, float cam_y, float start_x, int direction_collision)
{
	this->w = width;
	this->h = height;
	this->new_cam_x = cam_x;
	this->new_cam_y = cam_y;
	this->start_x_mario = start_x;
	this->direction_collision = direction_collision;
	id++;
}

void CPortal::GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx, int dy)
{
	left = x;
	top = y;
	right = left + w;
	bottom = top + h;
}

void CPortal::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CMario* mario = CMario::GetInstance();
	//if (GetTickCount64() - start_hide >= 2000 && is_activated) {	
	mario->GetHeightMario();
	if (is_activated) {
		if (direction_collision == -1 && mario->y > y ||
			direction_collision == 1 && mario->y + mario->h < y + h) {
			MoveCamera();
		}
	}
}

void CPortal::Render()
{
	RenderBoundingBox();
}

void CPortal::MoveCamera()
{
	CGame* game = CGame::GetInstance();
	CMario* mario = CMario::GetInstance();
	// set lại cam x, y
	mario->ChangeState(new CFallingState(mario->GetLevel()));
	game->SetCamXPos(new_cam_x);
	game->SetCamYPos(new_cam_y);

	// set lại vị trí mario
	mario->x = start_x_mario;
	mario->y = new_cam_y + 114;
	is_activated = false;
	mario->collide_with_portal = 0;
}
