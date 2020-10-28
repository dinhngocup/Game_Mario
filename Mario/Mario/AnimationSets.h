#pragma once
#include "Animation.h"
// tạo ra một vector gọi là animation set dùng để chứa tất cả animation của một đối tượng
// khi add 1 animation vào CAnimations (DB animation) thì add = id, gom tất cả id này lại
// bỏ vô animation set để khi new đối tượng thay vì get từng animation trong CAnimations
// thì có thể get animationset cho dễ, đỡ đi tìm id của từng animation trong DB
typedef vector<LPANIMATION> CAnimationSet;

typedef CAnimationSet* LPANIMATION_SET;

/*
	Animation sets là DB chứa tất các animation set của tất cả các loại đối tượng
	Khác với CAnimations: cũng chứa tất cả animation của đối tượng nhưng rời rạc, còn animation set
	gom các animation của một loại đối tượng bỏ vào vector
*/
class CAnimationSets
{
	static CAnimationSets* __instance;
	unordered_map<int, LPANIMATION_SET> animation_sets;
public:
	static CAnimationSets* GetInstance();

	CAnimationSets();
	void Add(int id, LPANIMATION_SET ani);
	LPANIMATION_SET Get(unsigned int id);
};

