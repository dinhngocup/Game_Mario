#include "Enemy.h"
#include "Koopa.h"
#include "Goomba.h"


void CEnemy::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
}


void CEnemy::IsCollisionWithBrick(LPCOLLISIONEVENT e)
{
	// đảo chiều vận tốc khi đụng hộp ngang
	if (e->nx != 0) { 
		vx *= -1;
		nx *= -1;
	}
	if (e->ny != 0) vy = 0;
}

void CEnemy::IsCollisionWithEnemy(LPCOLLISIONEVENT e)
{
	
}
