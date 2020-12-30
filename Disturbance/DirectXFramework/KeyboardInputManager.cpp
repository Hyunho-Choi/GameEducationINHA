#include "stdafx.h"
#include "KeyboardInputManager.h"

KeyboardInputManager::KeyboardInputManager()
	: mKeyState(0)
	, mPrevKeyState(0)
	, mbDisalble(false)
{
}

void KeyboardInputManager::ChangeKeyState()
{
	mKeyState = 0;
	if (GetKeyState('W') & 0x8000)
	{
		mKeyState = mKeyState | (UINT)eKeyName::KEY_FRONT;
	}
	if (GetKeyState('S') & 0x8000)
	{
		mKeyState = mKeyState | (UINT)eKeyName::KEY_BACK;
	}
	if (GetKeyState('A') & 0x8000)
	{
		mKeyState = mKeyState | (UINT)eKeyName::KEY_LEFT;
	}
	if (GetKeyState('D') & 0x8000)
	{
		mKeyState = mKeyState | (UINT)eKeyName::KEY_RIGHT;
	}
	if (GetKeyState('Q') & 0x8000)
	{
	}
	if (GetKeyState('E') & 0x8000)
	{
	}
	if (GetKeyState(VK_ESCAPE) & 0x8000)
	{
		mKeyState = mKeyState | (UINT)eKeyName::KEY_ESC;
	}
	if (GetKeyState(VK_SHIFT) & 0x8000)
	{
	}
	if (GetKeyState(VK_SPACE) & 0x8000)
	{
		mKeyState = mKeyState | (UINT)eKeyName::KEY_INTERACTION;
	}
	if (GetKeyState(VK_F1) & 0x8000)
	{
		mKeyState = mKeyState | (UINT)eKeyName::KEY_ONOFFCOLLIDE;
	}
}

void KeyboardInputManager::Update()
{
	if (!mbDisalble)
	{
		ChangeKeyState();

		bool prevKey = false;
		bool curKey = false;
		eKeyName keyName;

		if ((curKey = (mKeyState & (UINT)eKeyName::KEY_FRONT)))
		{
			keyName = eKeyName::KEY_FRONT;
			gEventManager->EventOccurred(eEventName::KEY_DOWN, &keyName);
		}
		else if ((prevKey = (mPrevKeyState & (UINT)eKeyName::KEY_FRONT)))
		{
			keyName = eKeyName::KEY_FRONT;
			gEventManager->EventOccurred(eEventName::KEY_UP, &keyName);
		}

		if ((curKey = (mKeyState & (UINT)eKeyName::KEY_BACK)))
		{
			keyName = eKeyName::KEY_BACK;
			gEventManager->EventOccurred(eEventName::KEY_DOWN, &keyName);
		}
		else if ((prevKey = (mPrevKeyState & (UINT)eKeyName::KEY_BACK)))
		{
			keyName = eKeyName::KEY_BACK;
			gEventManager->EventOccurred(eEventName::KEY_UP, &keyName);
		}

		if ((curKey = (mKeyState & (UINT)eKeyName::KEY_LEFT)))
		{
			keyName = eKeyName::KEY_LEFT;
			gEventManager->EventOccurred(eEventName::KEY_DOWN, &keyName);
		}
		else if ((prevKey = (mPrevKeyState & (UINT)eKeyName::KEY_LEFT)))
		{
			keyName = eKeyName::KEY_LEFT;
			gEventManager->EventOccurred(eEventName::KEY_UP, &keyName);
		}

		if ((curKey = (mKeyState & (UINT)eKeyName::KEY_RIGHT)))
		{
			keyName = eKeyName::KEY_RIGHT;
			gEventManager->EventOccurred(eEventName::KEY_DOWN, &keyName);
		}
		else if ((prevKey = (mPrevKeyState & (UINT)eKeyName::KEY_RIGHT)))
		{
			keyName = eKeyName::KEY_RIGHT;
			gEventManager->EventOccurred(eEventName::KEY_UP, &keyName);
		}

		if ((curKey = (mKeyState & (UINT)eKeyName::KEY_INTERACTION)))
		{
			keyName = eKeyName::KEY_INTERACTION;
			gEventManager->EventOccurred(eEventName::KEY_DOWN, &keyName);
		}
		else if ((prevKey = (mPrevKeyState & (UINT)eKeyName::KEY_INTERACTION)))
		{
			keyName = eKeyName::KEY_INTERACTION;
			gEventManager->EventOccurred(eEventName::KEY_UP, &keyName);
		}

		if ((curKey = (mKeyState & (UINT)eKeyName::KEY_ESC)))
		{
			keyName = eKeyName::KEY_ESC;
			gEventManager->EventOccurred(eEventName::KEY_DOWN, &keyName);
		}
		else if ((prevKey = (mPrevKeyState & (UINT)eKeyName::KEY_ESC)))
		{
			keyName = eKeyName::KEY_ESC;
			gEventManager->EventOccurred(eEventName::KEY_UP, &keyName);
		}
		else if ((prevKey = (mPrevKeyState & (UINT)eKeyName::KEY_ONOFFCOLLIDE)))
		{
			keyName = eKeyName::KEY_ONOFFCOLLIDE;
			gEventManager->EventOccurred(eEventName::KEY_UP, &keyName);
		}

		
		mPrevKeyState = mKeyState;
	}
}

void KeyboardInputManager::Disabled()
{
	mbDisalble = true;
}

void KeyboardInputManager::Abled()
{
	mbDisalble = false;
}

