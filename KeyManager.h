#pragma once
#include "lib/Interception/interception.h"
#include <chrono>
#include <thread>
#include <Windows.h>
class KeyManager
{
public:
	static void InitKeyborad();
	static void InitMouse();
	static void PressKey(UINT VK, UINT milliseconds = 0);
	static void PushKey(UINT VK);
	static void PopKey(UINT VK);
private:
	class Keyborad
	{
	public:
		Keyborad();
		~Keyborad();
		void PushKey(UINT VCS);
		void PopKey(UINT VCS);
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
	private:
		InterceptionContext Context;
		InterceptionDevice  Device;
	}; 
	static Mouse* MouseApp;
};