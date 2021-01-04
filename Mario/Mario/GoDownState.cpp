#include "GoDownState.h"
#include "PlayScene.h"

CGoDownState::CGoDownState(int level)
{
	//DebugOut(L"go down\n");
	CMario* mario = CMario::GetInstance();
	CGame* game = CGame::GetInstance();
	CPlayScene* scene = (CPlayScene*)game->GetCurrentScene();
	this->level = level;
	SetAnimation(level);
	mario->vx = 0;
	scene->time_scale = 0;
}

void CGoDownState::Update(float dt)
{
	CMario* mario = CMario::GetInstance();
	CGame* game = CGame::GetInstance();

	if (mario->collide_with_portal == 1) {
		mario->vy = -GO_DOWN_SPEED;
		if (mario->y + mario->h <= start_y) {
			mario->collide_with_portal = 0;
			CPlayScene* scene = (CPlayScene*)game->GetCurrentScene();
			scene->time_scale = 1;
			mario->ChangeState(new CFallingState(level));
		}
	}
	else if (mario->collide_with_portal == -1) {
		mario->vy = GO_DOWN_SPEED;
		if (mario->y >= start_y) {
			
			mario->collide_with_portal = 0;
			CPlayScene* scene = (CPlayScene*)game->GetCurrentScene();
			scene->time_scale = 1;
			mario->ChangeState(new CFallingState(level));
		}
	}
	else if (mario->collide_with_portal == 2) {
		if (!mario->is_hide) {
			mario->vy = GO_DOWN_SPEED;
		}
		else {
			mario->vy = -GO_DOWN_SPEED;
			if (mario->y + mario->h <= start_y) {
				mario->collide_with_portal = 0;
				CPlayScene* scene = (CPlayScene*)game->GetCurrentScene();
				scene->time_scale = 1;
				mario->is_hide = false;
				mario->ChangeState(new CFallingState(level));
			}
		}
	}
}

void CGoDownState::SetAnimation(int level)
{
	this->level = level;
	CMario* mario = CMario::GetInstance();
	switch (level) {
	case MARIO_LEVEL_BIG:
		ani = MARIO_ANI_BIG_GO_DOWN;
		break;
	case RACCOON_LEVEL_BIG:
		ani = RACCOON_ANI_GO_DOWN;
		break;
	case FIRE_LEVEL:
		ani = FIRE_ANI_GO_DOWN;
		break;
	case MARIO_LEVEL_SMALL:
		ani = MARIO_ANI_SMALL_GO_DOWN;
		break;
	}
}

