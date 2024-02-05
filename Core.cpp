#include <random>
#include <mutex>
#include <condition_variable>
#include "Core.h"
#include "KeyManager.h"
#include "Logger.h"
#include "EEparser.h"
#include "ImageProc.h"
void Core::DrifterIntoWall()
{
	KeyManager::PushKey('W'); Logger::debug("PushKey:W\n");
	KeyManager::SendKey(VK_SPACE, 10); Logger::debug("SendKey:VK_SPACE\n");
	KeyManager::SendKey(VK_SHIFT, 10); Logger::debug("SendKey:VK_SHIFT\n");
	KeyManager::SendKey(VK_SPACE, 10); Logger::debug("SendKey:VK_SPACE\n");
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	KeyManager::PopKey('W'); Logger::debug("PopKey:W\n");
}

void Core::FindSleepTerrain()
{
	std::thread([] {
		static EEparser ep;
		const auto&& Terrains = ep.CheckTerrain();
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
			Logger::warning(std::format("找到{}，在第{}个大地形\n", it.first, it.second));
			int Root{ GetFreq(3 + count) }, MajorThird{ GetFreq(7 + count) }, PerfectFifth{ GetFreq(10 + count) };
			Beep(Root, 500);
			Beep(MajorThird, 500);
			Beep(PerfectFifth, 500);
			Beep([&] {
				switch (it.second)
				{
				case 1:
					return Root;
				case 2:
					return MajorThird;
				case 3:
					return PerfectFifth;
				default:
					return Root;
				}
			}(), 500);
			count ++;
		}
	}).detach();
}

void Core::OpenRelic(bool& flag)
{
	static std::mutex mtx;
	static std::condition_variable cv;
	cv.notify_all();
	static const auto&& OpenRelic = [&] {
		std::thread([&] {
			while (1)
			{
				std::unique_lock<std::mutex> lock(mtx);
				cv.wait(lock, [&] {return flag; });
				std::random_device rd;
				KeyManager::SendKey(VK_LBUTTON, std::uniform_int_distribution<>(20, 50)(rd)); Logger::debug("SendKey:VK_LBUTTON\n");
				std::this_thread::sleep_for(std::chrono::milliseconds(std::uniform_int_distribution<>(300, 500)(rd)));
				KeyManager::SendKey(VK_RETURN, std::uniform_int_distribution<>(20, 50)(rd)); Logger::debug("SendKey:VK_RETURN\n");
				std::this_thread::sleep_for(std::chrono::milliseconds(std::uniform_int_distribution<>(1500, 3000)(rd)));
				lock.unlock();
			}
		}).detach();
		return 0;
	}();
}

void Core::LockMousePosition(bool& flag)
{
	static std::mutex mtx;
	static std::condition_variable cv;
	cv.notify_all();
	static const auto&& LockMouse = [&] {
		std::thread([&] {
			bool locked{ false };
			auto&& pos = ImageProc::nor2scr(0.203125f, 0.25925925f);
			while (1)
			{
				std::unique_lock<std::mutex> lock(mtx);
				cv.wait(lock, [&] {
					if (!flag && locked)
					{
						KeyManager::LockMousePosition(false);
						locked = false;
					}
					return flag;
				});

				if (!locked)
				{
					KeyManager::LockMousePosition(true);
					locked = true;
				}
				//防止回中必须保持移动
				std::random_device rd;
				cv.wait_for(lock, std::chrono::milliseconds(std::uniform_int_distribution<>(1000, 5000)(rd)), [&] {
					KeyManager::MoveAbsolute(pos.first, pos.second);
					ImageProc::ActiveWindow();
					return !flag;
				});
				lock.unlock();
			}
		}).detach();
		return 0;
	}();
}
