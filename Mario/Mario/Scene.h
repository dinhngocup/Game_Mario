#pragma once

#include <d3dx9.h>
#include "KeyEventHandler.h"
#include "GameObject.h"

class CScene
{
protected:
	// do cài đặt xử lý logic bàn phím cho mỗi scene khác nhau nên phải có con trỏ xử lý ở class này
	CKeyEventHandler* key_handler;
	int id;
	LPCWSTR sceneFilePath;

public:
	CScene(int id, LPCWSTR filePath);
	CScene() {};
	CKeyEventHandler* GetKeyEventHandler() { return key_handler; }
	// tải resources của scene lên bộ nhớ
	virtual void LoadSceneResources() = 0;
	// xóa toàn bộ dữ liệu của scene
	virtual void Unload() = 0;
	virtual void Update(DWORD dt) = 0;
	virtual void Render() = 0;

	virtual void SetEnemiesInScene(vector<LPGAMEOBJECT> listEnemy) {}
	virtual void SetGhostPlatformsInScene(vector<LPGAMEOBJECT> listGhostPlatform) {}
	virtual void SetItemsInScene(vector<LPGAMEOBJECT> listItem) {}

};
typedef CScene* LPSCENE;


class CScenceKeyHandler : public CKeyEventHandler
{
protected:
	CScene* scence;

public:
	virtual void KeyState(BYTE* states) = 0;
	virtual void OnKeyDown(int KeyCode) = 0;
	virtual void OnKeyUp(int KeyCode) = 0;

	CScenceKeyHandler(CScene* s) :CKeyEventHandler() { scence = s; }
};
