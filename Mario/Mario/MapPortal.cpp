#include "MapPortal.h"

CMapPortal::CMapPortal(int id, float x, float y, int state, bool is_portal, int l, int t, int r, int b)
{
	this->id_portal = id;
	this->x = x;
	this->y = y;
	this->state = state;
	this->is_portal = is_portal;
	this->l = l;
	this->t = t;
	this->r = r;
	this->b = b;

	this->w = 48;
	this->h = 48;
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(26);
	SetAnimationSet(ani_set);
	this->type_object = eTYPE::INVISIBLE_OBJECT;
}

void CMapPortal::GetBoundingBox(float& left, float& top, float& right, float& bottom, int dx, int dy)
{
}

void CMapPortal::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
}

void CMapPortal::Render()
{
	int ani = 8;
	switch (state) {
	case STATE_START:
		ani = ANI_START;
		break;
	case STATE_1:
		ani = ANI_1;
		break;
	case STATE_2:
		ani = ANI_2;
		break;
	case STATE_3:
		ani = ANI_3;
		break;
	case STATE_4:
		ani = ANI_4;
		break;
	case STATE_5:
		ani = ANI_5;
		break;
	case STATE_6:
		ani = ANI_6;
		break;
	case STATE_CARD:
		ani = ANI_CARD;
		break;
	case STATE_EMPTY:
		ani = ANI_EMPTY;
		break;
	}
	animation_set->at(ani)->Render(x, y);

}
