#pragma once

#define MARIO_WALKING_SPEED		0.3f 
#define MARIO_ACCELERATION	0.0008f 
#define MARIO_RUNNING_SPEED		0.3f 
//0.1f
#define MARIO_JUMP_SPEED_Y		0.6f
#define MARIO_JUMP_DEFLECT_SPEED 0.2f
#define MARIO_GRAVITY			0.002f
#define MARIO_DIE_DEFLECT_SPEED	 0.5f

#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200
#define MARIO_STATE_JUMP			300
#define MARIO_STATE_DIE				400
#define MARIO_STATE_CROUCH				500

#define MARIO_ANI_BIG_IDLE_RIGHT		0
#define MARIO_ANI_SMALL_IDLE_RIGHT		1
#define RACCOON_ANI_BIG_IDLE_RIGHT		2

#define MARIO_ANI_BIG_WALKING_RIGHT			3
#define MARIO_ANI_SMALL_WALKING_RIGHT		4
#define RACCOON_ANI_BIG_WALKING_RIGHT		5

#define MARIO_ANI_BIG_JUMPING_RIGHT			6
#define MARIO_ANI_SMALL_JUMPING_RIGHT		7
#define RACCOON_ANI_BIG_JUMPING_RIGHT		8

#define MARIO_ANI_BIG_CROUCHING_RIGHT			9
#define RACCOON_ANI_BIG_CROUCHING_RIGHT		10

#define MARIO_ANI_DIE				11

#define MARIO_ANI_FALLING_SMALL				12
#define MARIO_ANI_FALLING_BIG				13
#define RACCOON_ANI_FALLING_BIG				14

#define MARIO_ANI_SMALL_RUNNING			15
#define MARIO_ANI_BIG_RUNNING			16
#define RACCOON_ANI_RUNNING_BIG				17

#define RACCOON_ANI_SPINNING_BIG				18

#define MARIO_ANI_BIG_SKID			19
#define RACCOON_ANI_SKID_BIG				20
#define MARIO_ANI_SMALL_SKID			21

#define FIRE_MARIO_ANI_IDLE_RIGHT		22
#define FIRE_MARIO_ANI_WALKING_RIGHT			23
#define FIRE_MARIO_ANI_JUMPING_RIGHT			24
#define FIRE_MARIO_ANI_CROUCHING_RIGHT			25
#define FIRE_MARIO_ANI_FALLING			26
#define FIRE_MARIO_ANI_RUNNING			27
#define FIRE_MARIO_ANI_SKID			28
#define FIRE_MARIO_ANI_THROW			29

#define MARIO_ANI_SMALL_FLYING			30
#define MARIO_ANI_BIG_FLYING			31
#define RACCOON_ANI_FLYING			32
#define FIRE_ANI_FLYING			33

#define FIRE_ANI_FLYING_THROW			34


#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2
#define	RACCOON_LEVEL_BIG		3
#define	FIRE_LEVEL		4

#define MARIO_BIG_BBOX_WIDTH  45
#define MARIO_BIG_BBOX_HEIGHT 81

#define MARIO_CROUCH_BBOX_HEIGHT 54
#define RACCOON_BBOX_WIDTH 63

#define MARIO_SMALL_BBOX_WIDTH  39
#define MARIO_SMALL_BBOX_HEIGHT 45

#define MARIO_UNTOUCHABLE_TIME 5000

#define SMALL_MARIO_OFFSETX 12
#define RACCOON_MARIO_OFFSETX 22
#define BIG_MARIO_OFFSETX 16

#define DISPARITIES	14.0f


