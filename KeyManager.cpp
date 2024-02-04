#include "KeyManager.h"
#include "Logger.h"
void KeyManager::InitKeyborad()
{
	if (KeyboradApp == nullptr)
	{
		KeyboradApp = new Keyborad;
	}
}

KeyManager::Keyborad::Keyborad()
{
	Context = interception_create_context();
	if (Context == 0)
	{
		Logger::fatal("驱动通信失败！！！未安装驱动？如果装好了要重启系统\n");
		Logger::fatal("程序将在10秒后退出\n");
		std::this_thread::sleep_for(std::chrono::seconds(10));
		exit(0);
	}
	interception_set_filter(Context, interception_is_keyboard, INTERCEPTION_FILTER_KEY_ALL);
	Device = interception_wait(Context);
	interception_set_filter(Context, interception_is_keyboard, INTERCEPTION_FILTER_KEY_NONE);

}

KeyManager::Keyborad::~Keyborad()
{
	interception_destroy_context(Context);
}

void KeyManager::PushKey(const UINT VK)
{

		if (VK <= 6)
		{
			if (MouseApp != nullptr)
			{
				const USHORT MousePushKey[6]{0x1,0x4,0,0x10,0x40,0x100};
				MouseApp->PushKey(MousePushKey[VK-1]);
			}
		}
		else
		{
			if (KeyboradApp != nullptr)
			{
				KeyboradApp->PushKey(MapVirtualKey(VK, MAPVK_VK_TO_VSC_EX));
			}
		}
}

void KeyManager::PopKey(const UINT VK)
{
	if (VK <=6)
	{
		if(MouseApp != nullptr)
		{
			const USHORT MousePopKey[6]{ 0x2,0x8,0,0x20,0x80,0x200 };
			MouseApp->PopKey(MousePopKey[VK - 1]);
		}
	}
	else
	{
		if (KeyboradApp != nullptr)
		{
			KeyboradApp->PopKey(MapVirtualKey(VK, MAPVK_VK_TO_VSC_EX));
		}
	}

}

void KeyManager::SendKey(const UINT VK, const UINT interval_milliseconds)
{
	if (KeyboradApp != nullptr)
	{
		KeyManager::PushKey(VK);
		std::this_thread::sleep_for(std::chrono::milliseconds(interval_milliseconds));
		KeyManager::PopKey(VK);
	}
}

bool KeyManager::WaitKeyDown(const UINT VK)
{
	if (KeyboradApp == nullptr)
	{
		return false;
	}
	return KeyboradApp->WaitKey(MapVirtualKey(VK, MAPVK_VK_TO_VSC_EX), false);
}

bool KeyManager::WaitKeyUp(const UINT VK)
{
	if (KeyboradApp == nullptr)
	{
		return false;
	}
	return KeyboradApp->WaitKey(MapVirtualKey(VK, MAPVK_VK_TO_VSC_EX), true);
}

UINT KeyManager::LastKeyDown()
{
	if (KeyboradApp == nullptr)
	{
		return false;
	}
	while (1)
	{
		auto key = KeyboradApp->GetKeyState();
		if (key.second == InterceptionKeyState::INTERCEPTION_KEY_DOWN)
		{
			return MapVirtualKey(key.first, MAPVK_VK_TO_VSC_EX);
		}
	}
}

UINT KeyManager::LastKeyUp()
{
	if (KeyboradApp == nullptr)
	{
		return false;
	}
	while (1)
	{
		auto key = KeyboradApp->GetKeyState();
		if (key.second == InterceptionKeyState::INTERCEPTION_KEY_UP)
		{
			return MapVirtualKey(key.first, MAPVK_VSC_TO_VK_EX);
		}
	}
}

void KeyManager::MoveAbsolute(const UINT x, const UINT y)
{
	if (MouseApp != nullptr)
	{
		MouseApp->MovePosition(x, y, false);
	}
}

void KeyManager::MoveRelative(const int x, const int y)
{
	if (MouseApp != nullptr)
	{
		MouseApp->MovePosition(x, y, true);
	}
}

void KeyManager::LockMousePosition(const bool isLock)
{
	if (MouseApp != nullptr)
	{
		MouseApp->LockPosition(isLock);
	}
}

void KeyManager::Keyborad::PushKey(const UINT VSC)
{
	const InterceptionKeyStroke push{ (USHORT)VSC,INTERCEPTION_KEY_DOWN ,NULL };
	interception_send(Context, Device, (InterceptionStroke*)&push, 1);
}

void KeyManager::Keyborad::PopKey(const UINT VSC)
{
	const InterceptionKeyStroke pop{ (USHORT)VSC,INTERCEPTION_KEY_UP ,NULL };
	interception_send(Context, Device, (InterceptionStroke*)&pop, 1);
}

std::pair<USHORT, USHORT> KeyManager::Keyborad::GetKeyState()
{
	interception_set_filter(Context, interception_is_keyboard, INTERCEPTION_FILTER_KEY_DOWN|INTERCEPTION_FILTER_KEY_UP);
	InterceptionStroke stroke{};
	if (interception_receive(Context, interception_wait(Context), &stroke, 1))
	{
		interception_send(Context, Device, &stroke, 1);
	}
	interception_set_filter(Context, interception_is_keyboard, INTERCEPTION_FILTER_KEY_NONE);
	return std::make_pair( ((InterceptionKeyStroke*)&stroke)->code, ((InterceptionKeyStroke*)&stroke)->state);
}

bool KeyManager::Keyborad::WaitKey(const UINT VSC, const bool pushORpop)
{
	interception_set_filter(Context, interception_is_keyboard, pushORpop ? INTERCEPTION_FILTER_KEY_UP : INTERCEPTION_FILTER_KEY_DOWN);
	InterceptionStroke stroke{};
	while (interception_wait(Context))
	{
		if (interception_receive(Context, Device, &stroke, 1))
		{
			interception_send(Context, Device, &stroke, 1);
		}
		if (((InterceptionKeyStroke*)&stroke)->code == VSC)
		{
			break;
		}
	}
	interception_set_filter(Context, interception_is_keyboard, INTERCEPTION_FILTER_KEY_NONE);
	return true;
}

void KeyManager::InitMouse()
{
	if (MouseApp == nullptr)
	{
		MouseApp = new Mouse;
	}
}

KeyManager::Mouse::Mouse()
{
	Context = interception_create_context();
	if (Context == 0)
	{
		Logger::fatal("驱动通信失败！！！未安装驱动？如果装好了要重启系统\n");
		Logger::fatal("程序将在10秒后退出\n");
		std::this_thread::sleep_for(std::chrono::seconds(10));
		exit(0);
	}
	interception_set_filter(Context, interception_is_mouse, INTERCEPTION_FILTER_MOUSE_ALL);
	Device = interception_wait(Context);
	interception_set_filter(Context, interception_is_mouse, INTERCEPTION_FILTER_MOUSE_NONE);
}

KeyManager::Mouse::~Mouse()
{
	interception_destroy_context(Context);
}

void KeyManager::Mouse::PushKey(const USHORT MouseKey)
{
	const InterceptionMouseStroke push{ MouseKey, INTERCEPTION_MOUSE_MOVE_RELATIVE, 0, 0, 0, 0 };
	interception_send(Context, Device, (InterceptionStroke*)&push, 1);
}

void KeyManager::Mouse::PopKey(const USHORT MouseKey)
{
	const InterceptionMouseStroke pop{ MouseKey, INTERCEPTION_MOUSE_MOVE_RELATIVE, 0, 0, 0, 0 };
	interception_send(Context, Device, (InterceptionStroke*)&pop, 1);
}

void KeyManager::Mouse::MovePosition(const int x, const int y, const bool absolutORrelative)
{
	if (absolutORrelative)
	{
		const InterceptionMouseStroke move{ NULL,INTERCEPTION_MOUSE_MOVE_RELATIVE ,NULL,x,y,NULL };
		interception_send(Context, Device, (InterceptionStroke*)&move, 1);
	}
	else 
	{
		const InterceptionMouseStroke move{ NULL, INTERCEPTION_MOUSE_MOVE_ABSOLUTE, NULL,
			x * 65535 / GetSystemMetrics(SM_CXSCREEN), y * 65535 / GetSystemMetrics(SM_CYSCREEN), NULL };

		interception_send(Context, Device, (InterceptionStroke*)&move, 1);
	}
}

void KeyManager::Mouse::LockPosition(const bool isLock)
{
	if (isLock)
	{
		interception_set_filter(Context, interception_is_mouse, INTERCEPTION_FILTER_MOUSE_MOVE);
	}
	else
	{
		interception_set_filter(Context, interception_is_mouse, INTERCEPTION_FILTER_MOUSE_NONE);
	}
}
