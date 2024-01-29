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
int main(int argc, char* argv[]) {
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);
	Logger::info("按键盘任意键初始化键盘设备\n");
	KeyManager::InitKeyborad();
	Logger::info("按鼠标任意键或移动位置初始化鼠标设备\n");
	KeyManager::InitMouse();
	std::thread(ImageProc::AutoPause).detach();
	Logger::info("红氧暂停已开启，请将地图模式调整为全屏地图（选项>界面>以叠层地图显示，或者默认按M键切换）\n"
		"注意：游戏应为无边框全屏模式\n"
		"注意：检测到红氧会发出声音，连续检测红氧3次触发暂停\n\n"
		"快捷键：\n"
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
		case 'U':
			//Logger::debug("抬起U\n");
			std::thread([] {
				Logger::info("开始寻找虚空地形\n");
				static EEparser ep; 
				const auto&& Terrains = ep.CheckVoidTerrain();
				const auto&& GetFreq = [](int HalfSteps)
				{
					const int A4{ 440 };
					int Freq{ static_cast<int>(std::pow((float)2, (float)HalfSteps / (float)12) * (float)A4) };
					Logger::debug(std::format("HalfSteps: {} Freq：{}\n", HalfSteps, Freq));
					return Freq;
				};
				if (Terrains.empty())
				{
					Logger::info("未找到地形\n");
					Beep(GetFreq(5), 250);
					Beep(GetFreq(-2), 250);
				}
				int count{ 0 };
				for (const auto& it : Terrains)
				{
					Logger::warning(std::format("找到{}\n", it));
					Beep(GetFreq(3 + count), 500);
					Beep(GetFreq(7 + count), 500);
					Beep(GetFreq(10 + count), 500);
					count ++;
				}
			}).detach();
			break;
		case 'I':
			//Logger::debug("抬起I\n");
			flagOpenRelic = !flagOpenRelic;
			flagOpenRelic ? Logger::info("开始开核桃\n") : Logger::info("停止开核桃\n");
			static const auto&& OpenRelic = [] {
				std::thread([] {
					std::random_device rd;
					while (1)
					{
						if (flagOpenRelic)
						{
							KeyManager::SendKey(VK_LBUTTON, std::uniform_int_distribution<>(20, 50)(rd)); Logger::debug("输入VK_LBUTTON\n");
							std::this_thread::sleep_for(std::chrono::milliseconds(std::uniform_int_distribution<>(300, 500)(rd)));
							KeyManager::SendKey(VK_RETURN, std::uniform_int_distribution<>(20, 50)(rd)); Logger::debug("输入VK_RETURN\n");
							std::this_thread::sleep_for(std::chrono::milliseconds(std::uniform_int_distribution<>(1500, 3000)(rd)));
						}
						else
						{
							std::this_thread::sleep_for(std::chrono::seconds(1));
						}
					}
				}).detach();
				return 0;
			}();
			break;
		case 'O':
			//Logger::debug("抬起O\n");
			flagLockMouse = !flagLockMouse;
			flagLockMouse ? Logger::info("锁定鼠标位置\n") : Logger::info("解锁鼠标位置\n");
			static const auto&& LockMouse = []{
				std::thread([] {
					bool locked{false};
					auto&& pos = ImageProc::nor2scr(0.203125f, 0.25925925f);
					while (1)
					{
						if (flagLockMouse)
						{
							if (!locked)
							{
								KeyManager::LockMousePosition(true);
								locked = true;
							}
							KeyManager::MoveAbsolute(pos.first, pos.second);//防止回中必须保持移动
							ImageProc::ActiveWindow();
						}
						else if (locked)
						{
							KeyManager::LockMousePosition(false);
							locked = false;
						}
						std::random_device rd;
						std::this_thread::sleep_for(std::chrono::milliseconds(std::uniform_int_distribution<>(500, 1500)(rd)));
					}
				}).detach();
				return 0;
			}();
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