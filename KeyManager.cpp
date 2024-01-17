#include "KeyManager.h"

void KeyManager::InitKeyborad()
{
	if (KeyboradApp == nullptr)
	{
		KeyboradApp = new Keyborad;
	}
}

void KeyManager::InitMouse()
{
	if (MouseApp == nullptr)
	{
		MouseApp = new Mouse;
	}
}

void KeyManager::PressKey(UINT VK, UINT milliseconds)
{
	if (KeyboradApp != nullptr)
	{
		KeyboradApp->PushKey(MapVirtualKey(VK, MAPVK_VK_TO_VSC));
		std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
		KeyboradApp->PopKey(MapVirtualKey(VK, MAPVK_VK_TO_VSC));
	}
}

void KeyManager::PushKey(UINT VK)
{
	if (KeyboradApp != nullptr) 
	{
		KeyboradApp->PushKey(MapVirtualKey(VK, MAPVK_VK_TO_VSC));
	}
}

void KeyManager::PopKey(UINT VK)
{
	if (KeyboradApp != nullptr)
	{
		KeyboradApp->PopKey(MapVirtualKey(VK, MAPVK_VK_TO_VSC));
	}
}

KeyManager::Keyborad::Keyborad()
{
	Context = interception_create_context();
	interception_set_filter(Context, interception_is_keyboard, INTERCEPTION_FILTER_KEY_ALL);
	Device = interception_wait(Context);
}

KeyManager::Keyborad::~Keyborad()
{
	interception_destroy_context(Context);
}

void KeyManager::Keyborad::PushKey(UINT VCS)
{
	const InterceptionKeyStroke push = { (USHORT)VCS,INTERCEPTION_KEY_DOWN ,NULL };
	interception_send(Context, Device, (InterceptionStroke*)&push, 1);
}

void KeyManager::Keyborad::PopKey(UINT VCS)
{
	const InterceptionKeyStroke pop = { (USHORT)VCS,INTERCEPTION_KEY_UP ,NULL };
	interception_send(Context, Device, (InterceptionStroke*)&pop, 1);
}

KeyManager::Keyborad* KeyManager::KeyboradApp{ nullptr };

KeyManager::Mouse::Mouse()
{
	Context = interception_create_context();
	interception_set_filter(Context, interception_is_mouse, INTERCEPTION_FILTER_KEY_ALL);
	Device = interception_wait(Context);
}
KeyManager::Mouse::~Mouse()
{
	interception_destroy_context(Context);
}
KeyManager::Mouse* KeyManager::MouseApp{nullptr};
