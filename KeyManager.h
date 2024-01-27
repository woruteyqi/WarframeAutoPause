#pragma once
#include "lib/Interception/interception.h"
#include <chrono>
#include <thread>
#include <future>
#include <functional>
#include <Windows.h>

class KeyManager
{
public:
	//阻塞线程，初始化键盘设备
	static void InitKeyborad();

	//阻塞线程，初始化鼠标设备
	static void InitMouse();

	//按下按键
	static void PushKey(const UINT VK);

	//抬起按键
	static void PopKey(const UINT VK);

	//发送按键
	static void SendKey(const UINT VK, const UINT interval_milliseconds = 0);

	//阻塞线程，只当按下给定按键后返回true
	static bool WaitKeyDown(const UINT VK);

	//阻塞线程，只当抬起给定按键后返回true
	static bool WaitKeyUp(const UINT VK);

	//阻塞线程，返回第一个按下的按键
	static UINT LastKeyDown();

	//阻塞线程，返回第一个抬起的按键
	static UINT LastKeyUp();

	//鼠标移动到绝对坐标
	static void MoveAbsolute(const UINT x, const UINT y);

	//鼠标移动到相对坐标
	static void MoveRelative(const int x, const int y);

	//锁定鼠标位置
	static void LockMousePosition(const bool isLock);
private:
	class Keyborad
	{
	public:
		Keyborad();
		~Keyborad();
		void PushKey(const UINT VSC);
		void PopKey(const UINT VSC);
		std::pair<USHORT, USHORT> GetKeyState();
		bool WaitKey(const UINT VSC, const bool pushORpop);
	private:
		InterceptionContext Context;
		InterceptionDevice  Device;
	}; 
	static Keyborad* KeyboradApp;

	class Mouse
	{
	public:
		Mouse();
		~Mouse();
		void PushKey(const USHORT MouseKey);
		void PopKey(const USHORT MouseKey);
		void MovePosition(const int x, const int y, const bool absolutORrelative);
		void LockPosition(const bool isLock);
	private:
		InterceptionContext Context;
		InterceptionDevice  Device;
	}; 
	static Mouse* MouseApp;
};