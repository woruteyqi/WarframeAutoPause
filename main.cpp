#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <future>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <random>
#include <Windows.h>
#include "Logger.h"
#include "KeyManager.h"
#include "EEparser.h"
int main(int argc, char* argv[]) {
	Logger::info("按键盘任意键初始化键盘设备\n");
	KeyManager::InitKeyborad();
	Logger::info("按鼠标任意键或移动位置初始化鼠标设备\n");
	KeyManager::InitMouse();
	Logger::info("快捷键：\n"
		"U->查询地形（查询到有声音提示）\n"
		"I->自动开核桃（鼠标点击-回车）循环，使用随机延迟\n"
		"O->锁定鼠标位置 （锁定绝对坐标为第一个核桃位置，禁止相对移动）\n"
		"P->退出程序\n");
	while (1)
	{
		UINT key{ KeyManager::LastKeyUp() };
		static std::mutex mtx;
		static std::condition_variable cv;
		static bool flag[3]{false};
		static int exitcount{ 0 };

		switch (key)
		{
		case 'U':
			Logger::debug("抬起U\n");
			std::thread([] {
				Logger::info("开始寻找虚空地形\n");
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
			flag[0] = !flag[0];
			flag[0] ? Logger::info("开始开核桃\n") : Logger::info("停止开核桃\n");
			if (flag[0])
			{
				std::thread([&] {
					std::random_device rd;
					std::lock_guard<std::mutex> lock(mtx);
					flag[1] = false;
					while (flag[0])
					{
						KeyManager::SendKey(VK_LBUTTON, std::uniform_int_distribution<>(20, 50)(rd)); Logger::debug("输入鼠标左键\n");
						std::this_thread::sleep_for(std::chrono::milliseconds(std::uniform_int_distribution<>(300, 500)(rd)));
						KeyManager::SendKey(VK_RETURN, std::uniform_int_distribution<>(20, 50)(rd)); Logger::debug("输入键盘回车\n");
						std::this_thread::sleep_for(std::chrono::milliseconds(std::uniform_int_distribution<>(1500, 3000)(rd)));
					}
					flag[1] = true;
					cv.notify_one();
				}).detach();
			}
			break;
		case 'O':
			Logger::debug("抬起O\n");
			flag[2] = !flag[2];
			flag[2] ? Logger::info("锁定鼠标位置\n") : Logger::info("解锁鼠标位置\n");
			KeyManager::MoveAbsolute(390, 280);
			KeyManager::LockMousePosition(flag[2]);
			break;
		case 'P':
			Logger::debug("抬起P\n");
			exitcount++;
			if (exitcount > 1)
			{
				if (flag[0])
				{
					Logger::info("目前有开核桃任务！等待开核桃线程结束\n");
					flag[0] = false;
					std::unique_lock<std::mutex> lock(mtx);
					cv.wait(lock, [] {return flag[1]; });
				}
				Logger::info("已退出\n");
				exit(1);
			}
			Logger::warning("确定退出请再按P\n");
			break;
		default:
			exitcount = 0;
			Logger::debug(std::format("抬起{:#X}\n",key));
			break;
		}
	}
}