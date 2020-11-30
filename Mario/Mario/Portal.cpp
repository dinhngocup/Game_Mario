#include "Portal.h"

CPortal::CPortal(float x, float y, float width, float height, float cam_x, float cam_y, float start_x, float start_y, int direction_collision, float y_change)
{
	this->w = width;
	this->h = height;
	this->new_cam_x = cam_x;
	this->new_cam_y = cam_y;
	this->start_x_mario = start_x;
	this->start_y_mario = start_y;
	this->direction_collision = direction_collision;
	this->y_change = y_change;
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
			mario->x = start_x_mario;
			mario->y = start_y_mario;
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
	//mario->ChangeState(new CFallingState(mario->GetLevel()));
	game->SetCamXPos(new_cam_x);
	game->SetCamYPos(new_cam_y);

	// set lại vị trí mario
	/*mario->x = start_x_mario;
	mario->y = new_cam_y + 114;*/
	is_activated = false;
	
}
