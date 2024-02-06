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
	std::this_thread::sleep_for(std::chrono::milliseconds(5));
	KeyManager::SendKey(VK_SHIFT, 10); Logger::debug("SendKey:VK_SHIFT\n");
	std::this_thread::sleep_for(std::chrono::milliseconds(5));
	KeyManager::SendKey(VK_SPACE, 30); Logger::debug("SendKey:VK_SPACE\n");
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
				cv.wait_for(lock, std::chrono::milliseconds(std::uniform_int_distribution<>(300, 500)(rd)), [&] {return !flag; });
				KeyManager::SendKey(VK_RETURN, std::uniform_int_distribution<>(20, 50)(rd)); Logger::debug("SendKey:VK_RETURN\n");
				cv.wait_for(lock, std::chrono::milliseconds(std::uniform_int_distribution<>(1500, 3000)(rd)), [&] {return !flag; });
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
				KeyManager::MoveAbsolute(pos.first, pos.second); Logger::debug(std::format("MoveAbsolute: {}, {}\n", pos.first, pos.second));
				ImageProc::ActiveWindow();
				cv.wait_for(lock, std::chrono::milliseconds(std::uniform_int_distribution<>(5000, 10000)(rd)), [&] { return !flag; });
				lock.unlock();
			}
		}).detach();
		return 0;
	}();
}

void Core::AutoPause()
{
	static const auto&& AutoPause = []() {
		std::thread(ImageProc::AutoPause).detach();
		return 0;
	}();
}

void Core::Commander(int argc, char* argv[])
{
	Logger::debug(std::format("argc:{}\n", argc));
	if (argc < 3) return;
	int hours{}, minutes{};
	switch (argc)
	{
	case 3:
		Logger::debug(std::format("命令 {}，参数 {}\n", argv[1], argv[2]));
		
		if (strcmp(argv[1], "-H") && strcmp(argv[1],"-h"))
		{
			Logger::error(std::format("指令错误，在 \033[4m{}\033[m\n", argv[1])); return;
		}
		hours = atoi(argv[2]);
		if (hours <= 0 || hours >= 24)
		{
			Logger::error(std::format("参数错误，在 \033[4m{}\033[m\n", argv[2])); return;
		}
		break;
	case 5:
		Logger::debug(std::format("命令1 {}，参数1 {} | 命令2 {}，参数2 {}\n",argv[1], argv[2], argv[3], argv[4]));

		if (strcmp(argv[1], "-H") && strcmp(argv[1], "-h"))
		{
			Logger::error(std::format("指令错误，在 \033[4m{}\033[m\n", argv[1])); return;
		}
		hours = atoi(argv[2]);
		if (hours < 0 || hours >= 24)
		{
			Logger::error(std::format("参数错误，在 \033[4m{}\033[m\n", argv[2])); return;
		}

		if (strcmp(argv[3], "-M") && strcmp(argv[3], "-m"))
		{
			Logger::error(std::format("指令错误，在 \033[4m{}\033[m\n", argv[3])); return;
		}
		minutes = atoi(argv[4]);
		if (minutes <= 0 || minutes >= 24)
		{
			Logger::error(std::format("参数错误，在 \033[4m{}\033[m\n", argv[4])); return;
		}
		break;
	default:
		Logger::error("启动参数错误\n");
		break;
	}
	
	static const auto StopDuration{ std::chrono::hours(hours) + std::chrono::minutes(minutes)};
	const auto StopTime{ std::chrono::system_clock::now() + StopDuration };
	const auto StopTime_t{ std::chrono::system_clock::to_time_t(StopTime) };
	tm tm{}; localtime_s(&tm, &StopTime_t);
	Logger::info(std::format("程序将在{}小时{}分钟后 ⌈{}⌋ 执行定时暂停\n", hours, minutes,
		std::format("{}年{}月{}日 {:0>2}时{:0>2}分{:0>2}秒",tm.tm_year + 1900,tm.tm_mon + 1,tm.tm_mday, tm.tm_hour,tm.tm_min,tm.tm_sec)));

	std::thread([&] {
		Logger::debug(std::format("StopDuration:{}\n", StopDuration.count()));
		std::this_thread::sleep_for(StopDuration);
		Logger::warning("到达定时！尝试暂停\n");
		KeyManager::SendKey(VK_LBUTTON); Logger::debug("SendKey:VK_LBUTTON\n");
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		KeyManager::SendKey(VK_ESCAPE); Logger::debug("SendKey:VK_ESCAPE\n");
	}).detach();
}
