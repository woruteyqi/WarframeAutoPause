#include <iostream>
#include <fstream>
#include <Windows.h>
#include <vector>
#include <future>
#include <thread>
#include "Logger.h"
#include "KeyManager.h"
#include "EEparser.h"
int main(int argc, char* argv[]) {
	Logger::info("按键盘任意键初始化键盘设备\n");
	KeyManager::InitKeyborad();
	Logger::info("按鼠标任意键或移动位置初始化鼠标设备\n");
	KeyManager::InitMouse();
	Logger::info("快捷键：\nU->查询地形\nI->自动开核桃（鼠标点击-回车）循环\nO->锁定鼠标位置 （锁定绝对坐标为第一个核桃位置，禁止相对移动）\nP->退出程序\n");
	while (1)
	{
		auto key = KeyManager::LastKeyUp();
		switch (key)
		{
		case 'U':
			Logger::debug("抬起U\n");
			std::thread([] {
				static EEparser ep; auto test = ep.CheckVoidTerrain();
				if (test.empty())
				{
					Logger::info("未找到地形\n");
				}
				for (const auto& it : test)
				{
					Logger::warning(std::format("找到{}\n", it));
					Beep(523, 500);//C
					Beep(659, 500);//E
					Beep(783, 500);//G
				}
				}).detach();
			break;
		case 'I':
			Logger::debug("抬起I\n");
			break;
		case 'O':
			Logger::debug("抬起O\n");
			break;
		case 'P':
			Logger::warning("抬起P\n");
			break;
		default:
			Logger::debug(std::format("抬起{:#X}\n",key));
			break;
		}
	}
}