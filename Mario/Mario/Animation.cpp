#include "Animation.h"
#include "Utils.h"

#include "Sprites.h"
#include "Mario.h"



void CAnimation::Add(int spriteId, DWORD time)
{
	int t = time;
	if (time == 0) t = this->defaultTime;

	LPSPRITE sprite = CSprites::GetInstance()->Get(spriteId);

	if (sprite == NULL)
	{
		DebugOut(L"[ERROR] Sprite ID %d cannot be found!\n", spriteId);
	}

	LPANIMATION_FRAME frame = new CAnimationFrame(sprite, t);
	frames.push_back(frame);
}

void CAnimation::Render(float x, float y, int alpha, int nx, int offset, bool spinningFlag, int ny)
{
	DWORD now = GetTickCount();
	if (currentFrame == -1)
	{
		currentFrame = 0;
		lastFrameTime = now;
	}
	else
	{
		DWORD t = frames[currentFrame]->GetTime();
		if (now - lastFrameTime > t)
		{
			currentFrame++;
			lastFrameTime = now;

			if (currentFrame + 1 == frames.size()) {
				nextIsLastFrame = true;
			}
			else {
				nextIsLastFrame = false;
			}

			if (currentFrame == frames.size()) {
				currentFrame = 0;
				lastFrame = true;
			} else
				lastFrame = false;
		}
	}
	// set offset cho ani spinning
	CMario* mario = CMario::GetInstance();
	if (spinningFlag) {
		if (currentFrame == 0 || currentFrame == 4) {
			frames[currentFrame]->GetSprite()->DrawFlipX(x, y, alpha, nx, offset);
		}
		else if (currentFrame == 2) {
			frames[currentFrame]->GetSprite()->DrawFlipX(x, y, alpha, nx, -offset);
		}
		else {
			frames[currentFrame]->GetSprite()->DrawFlipX(x, y, alpha, nx, 0);
		}
	}
	else
		frames[currentFrame]->GetSprite()->DrawFlipX(x, y, alpha, nx, offset, ny);
}
