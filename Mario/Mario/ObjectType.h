#pragma once
#define COIN_ANI	9
#define LEAF_ANI	11
#define MUSHROOM_ANI	12
// id of object in scenetest.txt
enum eTYPE {
	MARIO = 0,

	//GROUND
	BRICK = 1,

	//ENEMIES
	GOOMBA = 2,
	KOOPA = 3,
	GROOM = 4,

	//INVISIBLE OBJECT
	INVISIBLE_OBJECT = 5,

	//BRICK_QUESTION
	BRICK_QUESTION = 6,

	//WEAPON
	FIRE_BALL = 7,

	//BONUS
	COIN = 8,
	MUSHROOM = 9,
	LEAF = 10,
	FIRE_FLOWER = 11,
	STATIC_COIN = 12,
	FIRE_FLOWER_WEAPON = 13,
	FLOWER = 14,
	DEBRIS = 15,

	//SWITCH SCENE
	PORTAL = 50,
};

enum eTYPE_OBJECT {
	GHOST_PLATFORM = 1,
	ENEMY = 2,
	ITEM = 3,
};