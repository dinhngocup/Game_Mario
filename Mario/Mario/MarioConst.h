#pragma once

#define MARIO_WALKING_SPEED		0.25f 
#define MARIO_ACCELERATION	0.0005f 
#define MARIO_RUNNING_SPEED		0.25f 

#define MARIO_JUMP_SPEED_Y		0.6f
#define MARIO_JUMP_DEFLECT_SPEED 0.2f
#define MARIO_GRAVITY			0.002f
#define MARIO_DIE_DEFLECT_SPEED	 0.5f

#define MARIO_FLYING_SPEED		-0.4f
#define WALKING_ON_THE_AIR		0.35f
#define GO_DOWN_SPEED 0.05f

#define TIME_LIMIT_FROM_SMALL_TO_BIG	1100
#define TIME_LIMIT_FROM_BIG_TO_SMALL	700
#define TIME_GROWING_UP		1800
#define TIME_CHANGE_STATE_WITHIN_GROWING_UP		200
#define DISPARITY_SMALL_TO_MIDDLE_MARIO		12
#define DISPARITY_BIG_TO_MIDDLE_MARIO		24

#define TIME_SPINNING 600
#define TIME_CONVERT_SPINNING_STATE	120
#define TIME_THROWING_FIRE 400
#define TIME_KICKING 200
#define TIME_MAX_JUMP 300
#define TIME_TRANSFORM_EFFECT	600

#define OFFSET_RACCOON	6
#define MAX_SPEED_RUNNING	0.7f
#define TIME_SKID	400

#define TIME_BLOCK_PRESS_Z	1000
#define MARIO_WALKING_ACCELERATION	0.0008f 

#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200
#define MARIO_STATE_JUMP			300
#define MARIO_STATE_DIE				400
#define MARIO_STATE_CROUCH				500
#define MARIO_STATE_HIDE_UNTOUCHABLE			600
#define MARIO_STATE_UNHIDE_UNTOUCHABLE			700
#define MARIO_STATE_HIDE			800
#define MARIO_STATE_SMALL_GROW_UP			900
#define MARIO_STATE_BIG_GROW_UP			1000
#define MARIO_STATE_BIG_END_GROW_UP			1100
#define MARIO_STATE_ATTACKING		1200
#define MARIO_STATE_NO_ATTACKING			1300
#define MARIO_AUTO_GO			1400
#define MARIO_END_GROWING			1500

#define MARIO_ANI_BIG_IDLE_RIGHT		0
#define MARIO_ANI_SMALL_IDLE_RIGHT		1
#define RACCOON_ANI_BIG_IDLE_RIGHT		2
#define FIRE_MARIO_ANI_IDLE_RIGHT		3

#define MARIO_ANI_BIG_WALKING_RIGHT			4
#define MARIO_ANI_SMALL_WALKING_RIGHT		5
#define RACCOON_ANI_BIG_WALKING_RIGHT		6
#define FIRE_MARIO_ANI_WALKING_RIGHT			7

#define MARIO_ANI_DIE				8

#define MARIO_ANI_BIG_JUMPING_RIGHT			9
#define MARIO_ANI_SMALL_JUMPING_RIGHT		10
#define RACCOON_ANI_BIG_JUMPING_RIGHT		11
#define FIRE_MARIO_ANI_JUMPING_RIGHT			12

#define MARIO_ANI_BIG_CROUCHING_RIGHT			13
#define RACCOON_ANI_BIG_CROUCHING_RIGHT		14
#define FIRE_MARIO_ANI_CROUCHING_RIGHT			15


#define MARIO_ANI_FALLING_SMALL				16
#define MARIO_ANI_FALLING_BIG				17
#define RACCOON_ANI_FALLING_BIG				18
#define FIRE_MARIO_ANI_FALLING			19

#define MARIO_ANI_SMALL_RUNNING			20
#define MARIO_ANI_BIG_RUNNING			21
#define RACCOON_ANI_RUNNING_BIG				22
#define FIRE_MARIO_ANI_RUNNING			23

#define RACCOON_ANI_SPINNING_BIG				24

#define MARIO_ANI_BIG_SKID			25
#define RACCOON_ANI_SKID_BIG				26
#define MARIO_ANI_SMALL_SKID			27
#define FIRE_MARIO_ANI_SKID			28

#define FIRE_MARIO_ANI_THROW			29

#define MARIO_ANI_SMALL_FLYING			30
#define MARIO_ANI_BIG_FLYING			31
#define RACCOON_ANI_FLYING			32
#define FIRE_ANI_FLYING			33
#define FIRE_ANI_FLYING_THROW			34


#define MARIO_ANI_SMALL_HOLDING			35
#define MARIO_ANI_BIG_HOLDING			36
#define RACCOON_ANI_HOLDING			37
#define FIRE_ANI_HOLDING			38

#define MARIO_ANI_SMALL_KICK			39
#define MARIO_ANI_BIG_KICK			40
#define RACCOON_ANI_KICK			41
#define FIRE_ANI_KICK			42

#define MARIO_ANI_SMALL_STAND_HOLD			43
#define MARIO_ANI_BIG_STAND_HOLD			44
#define RACCOON_ANI_STAND_HOLD			45
#define FIRE_ANI_STAND_HOLD			46

#define MARIO_ANI_SMALL_GROW_UP			47
#define MARIO_ANI_BIG_GROW_UP			48

#define MARIO_ANI_SMALL_GO_DOWN			49
#define MARIO_ANI_BIG_GO_DOWN			50
#define RACCOON_ANI_GO_DOWN			51
#define FIRE_ANI_GO_DOWN			52


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

#define MARIO_UNTOUCHABLE_TIME 4000

#define SMALL_MARIO_OFFSETX 12
#define RACCOON_MARIO_OFFSETX 22
#define BIG_MARIO_OFFSETX 16

#define DISPARITIES	27.0f
#define MARIO_DEFAULT_LIVE	4

