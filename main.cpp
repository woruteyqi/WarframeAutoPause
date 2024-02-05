#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <random>
#include <cmath>
#include <Windows.h>
#include "Logger.h"
#include "KeyManager.h"
#include "EEparser.h"
#include "ImageProc.h"
#include "Core.h"
int main(int argc, char* argv[]) {
	Logger::info("按键盘任意键初始化键盘设备\n");
	KeyManager::InitKeyborad();
	Logger::info("按鼠标任意键或移动位置初始化鼠标设备\n");
	KeyManager::InitMouse();
	std::thread(ImageProc::AutoPause).detach();
	Logger::info("红氧暂停已开启，请将地图模式调整为全屏地图（选项>界面>以叠层地图显示，或者默认按M键切换）\n"
		"注意：游戏应为无边框全屏模式\n"
		"注意：连续检测红氧3次触发暂停且发出声音\n\n"
		"快捷键：\n"
		"右SHIFT->漂泊者翻墙\n"
		"U->查询地形（查询到和未查询到有不一样的声音提示）\n"
		"I->自动开核桃（鼠标点击-回车）循环，使用随机延迟\n"
		"O->锁定鼠标位置（锁定绝对坐标为第一个核桃位置，禁止相对移动，同时会保持游戏窗口激活并置于顶层）\n"
		"P->退出程序\n\n");
	while (1)
	{
		UINT key{ KeyManager::LastKeyUp() };
		static bool flagOpenRelic{ false }, flagLockMouse{false};
		static int exitcount{ 0 };

		switch (key)
		{
		case VK_RSHIFT:	
			//Logger::debug("抬起RSHIFT\n");
			Logger::info("漂泊者翻墙\n");
			Core::DrifterIntoWall();
			break;
		case 'U':
			//Logger::debug("抬起U\n");
			Logger::info("开始寻找睡觉地形\n");
			Core::FindSleepTerrain();
			break;
		case 'I':
			//Logger::debug("抬起I\n");
			flagOpenRelic = !flagOpenRelic;
			flagOpenRelic ? Logger::info("开始开核桃\n") : Logger::info("停止开核桃\n");
			Core::OpenRelic(flagOpenRelic);
			break;
		case 'O':
			//Logger::debug("抬起O\n");
			flagLockMouse = !flagLockMouse;
			flagLockMouse ? Logger::info("锁定鼠标位置\n") : Logger::info("解锁鼠标位置\n");
			Core::LockMousePosition(flagLockMouse);
			break;
		case 'P':
			//Logger::debug("抬起P\n");
			exitcount++;
			if (exitcount > 1)
			{
				Logger::info("已退出\n");
				exit(1);
			}
			Logger::warning("确定退出请再按P，按任意非快捷键将重置计数\n");
			break;
		default:
			exitcount = 0;
			//Logger::debug(std::format("抬起{:#X}\n",key));
			break;
		}
	}
}