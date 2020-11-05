#include "Enemy.h"
#include "Koopa.h"
#include "Goomba.h"


void CEnemy::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
}


void CEnemy::IsCollisionWithBrick(LPCOLLISIONEVENT e)
{
	// đảo chiều vận tốc khi đụng hộp ngang
	if (e->nx != 0) vx *= -1;
	if (e->ny != 0) vy = 0;
}

void CEnemy::IsCollisionWithEnemy(LPCOLLISIONEVENT e)
{
	/*switch (e->obj->state) {
	case STATE_DIE_BY_WEAPON:
		DebugOut(L"cham nam\n");
		x += dx;
		break;
	case STATE_SPIN:
		DebugOut(L"cham rua state spin\n");
		if (e->nx > 0)
			nx = 1;
		else 
			nx = -1;
		AttackedByShell();
		break;
	default:
		if (e->nx != 0) {
			vx *= -1;
			e->obj->vx *= -1;
		}
		break;
	}*/
}
