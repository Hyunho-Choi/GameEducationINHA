#pragma once
#include "EventManager.h"
#include "Singleton.h"

#define gKeyboardInputManager KeyboardInputManager::GetInstance()

enum class eKeyName { KEY_LEFT = 1, KEY_RIGHT = 2, KEY_FRONT = 4, KEY_BACK = 8, KEY_FRONTLEFT = 16, KEY_FRONTRIGHT = 32,KEY_BACKLEFT = 64, KEY_BACKRIGHT = 128,
	KEY_INTERACTION = 256,KEY_EQUIPCHAGELFFT = 512, KEY_EQUIPCHAGERIGHT = 1024 , KEY_RUN = 2048, KEY_ESC = 4096, KEY_ONOFFCOLLIDE = 8192};

typedef UINT KeyState;

class KeyboardInputManager : public Singleton<KeyboardInputManager>
{
private:
	bool mbDisalble;

	KeyState mKeyState;
	KeyState mPrevKeyState;
protected:
	friend Singleton<KeyboardInputManager>;
	KeyboardInputManager();
public:
	void ChangeKeyState();
	void Update();
	void Disabled();
	void Abled();
};
