#pragma once
#include "Windows.h"
#include "Logger.h"
#include "lib/Op/libop.h"
class ImageProc
{
public:
	static void AutoPause();
private:
	inline static libop* OP = new libop();
	inline static HWND WfGameWindow{ [] {
		long ret{};
		OP->FindWindowByProcess(L"Warframe.x64.exe",L"", L"Warframe",&ret);
		Logger::debug(std::format("游戏窗口句柄：{:X}\n", ret));
		return reinterpret_cast<HWND>(ret);
		}()
	};
	//屏幕坐标转换归一化坐标
	static std::pair<float, float> scr2nor(int X, int Y,
		int screenWidth = GetSystemMetrics(SM_CXSCREEN), int screenHeight = GetSystemMetrics(SM_CYSCREEN));

	//归一化坐标转换屏幕坐标
	static std::pair<int, int> nor2scr(float X, float Y,
		int screenWidth = GetSystemMetrics(SM_CXSCREEN), int screenHeight = GetSystemMetrics(SM_CYSCREEN));
};

