#pragma once

#include <unordered_map>

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "Hub.h"

#define DIRECTINPUT_VERSION 0x0800

#define SECTION_UNKNOWN -1
#define SECTION_TEXTURES 2
#define SECTION_SPRITES 3
#define SECTION_ANIMATIONS 4
#define SECTION_ANIMATION_SETS	5
#define SECTION_HUB 6
#define SECTION_FONT 7

#define TYPE_PLAY_SCENE 1
#define TYPE_WORLD_MAP 0
#define TYPE_INTRO 2

#define MAX_SCENE_LINE	1024
#include <dinput.h>

#include "Scene.h"

using namespace std;

#define KEYBOARD_BUFFER_SIZE 1024

class CGame
{
	static CGame* __instance;
	CHub* hub_game;
	HWND hWnd;									// Window handle

	LPDIRECT3D9 d3d = NULL;						// Direct3D handle
	LPDIRECT3DDEVICE9 d3ddv = NULL;				// Direct3D device object

	LPDIRECT3DSURFACE9 backBuffer = NULL;


	LPD3DXSPRITE spriteHandler = NULL;			// Sprite helper library to help us draw 2D image on the screen 

	LPDIRECTINPUT8       di;		// The DirectInput object         
	LPDIRECTINPUTDEVICE8 didv;		// The keyboard device 

	BYTE  keyStates[256];			// DirectInput keyboard state buffer 
	DIDEVICEOBJECTDATA keyEvents[KEYBOARD_BUFFER_SIZE];		// Buffered keyboard data

	LPKEYEVENTHANDLER keyHandler;

	float cam_x = 0.0f;
	float cam_y = 0.0f;
	/*float cam_x = 5904;
	float cam_y = 1392;*/

	int screen_width;
	int screen_height;

	unordered_map<int, LPSCENE> scenes;

	void _ParseSection_SETTINGS(string line);
	void _ParseSection_SCENES(string line);
	void _ParseSection_RESOURCES(string line);
	void _ParseSection_HUB(string line);
	void _ParseSection_FONT(string line);
	void ReadFileHub(LPCWSTR filePath);

	// utils used to load resources
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);

public:
	int current_scene;
	void _ParseSection_TEXTURES(string line);

	void InitKeyboard();
	void SetKeyHandler(LPKEYEVENTHANDLER handler) { keyHandler = handler; }
	void Init(HWND hWnd);
	void Draw(float x, float y, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom, int alpha = 255);

	int IsKeyDown(int KeyCode);
	void ProcessKeyboard();

	void Load(LPCWSTR gameFile);
	void LoadResources(LPCWSTR resourceFile);
	LPSCENE GetCurrentScene() { return scenes[current_scene]; }
	void SwitchScene(int scene_id);

	int GetScreenWidth() { return screen_width; }
	int GetScreenHeight() { return screen_height; }

	static void SweptAABB(
		float ml,			// move left 
		float mt,			// move top
		float mr,			// move right 
		float mb,			// move bottom
		float dx,			// 
		float dy,			// 
		float sl,			// static left
		float st,
		float sr,
		float sb,
		float& t,
		float& nx,
		float& ny);

	LPDIRECT3DDEVICE9 GetDirect3DDevice() { return this->d3ddv; }
	LPDIRECT3DSURFACE9 GetBackBuffer() { return backBuffer; }
	LPD3DXSPRITE GetSpriteHandler() { return this->spriteHandler; }

	void SetCamXPos(float x) { cam_x = x; }
	void SetCamYPos(float y) { cam_y = y; }
		
	void SetCamPos(float x, float y) { cam_x = x; cam_y = y; }
	void GetCamPos(float& x, float& y) { x = cam_x; y = cam_y; }
	float GetCamX() { return cam_x; }
	float GetCamY() { return cam_y; }
	static CGame* GetInstance();

	~CGame();
};


