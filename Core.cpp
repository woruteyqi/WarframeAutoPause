#include <random>
#include <mutex>
#include <filesystem>
#include <fstream>
#include <condition_variable>
#include "Core.h"
#include "KeyManager.h"
#include "Logger.h"
#include "EEparser.h"
#include "ImageProc.h"
#include "cmdline.h"
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
			Logger::info("未找到睡觉地形\n");
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
				KeyManager::MoveAbsolute(pos.first, pos.second);
				//Logger::debug(std::format("MoveAbsolute: {}, {}\n", pos.first, pos.second));
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
	cmdline::parser p{};
	p.add<int>("hour", 'h', "需要暂停的小时数[0,23]", false, 0,cmdline::range(0,23));
	p.add<int>("minute", 'm', "需要暂停的分钟数[1,59]", false, 0, cmdline::range(0, 59));
	p.add<std::string>("last_path", 'l',"原始文件目录",false,"");

	p.parse_check(argc,argv);
	std::string last_path{ p.get<std::string>("last_path") };
	int hours{p.get<int>("hour")}, minutes{p.get<int>("minute")};

	namespace fs = std::filesystem;
	fs::path currentPath = fs::absolute(fs::path(argv[0]));
	if (last_path.empty()) {
		Logger::debug("empty\n");
		Logger::debug(std::format("currentPath: {}\n", currentPath.string()));
		srand(static_cast<unsigned int>(time(nullptr)));

		const std::string characters = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
		const auto charactersLength = characters.length();

		std::random_device rd;
		std::mt19937 generator(rd());
		std::uniform_int_distribution<size_t> distribution(0, charactersLength - 1);

		std::string randomFileName;
		for (int i = 0; i < 10; ++i) {
			randomFileName += characters[distribution(generator)];
		}
		randomFileName += ".exe";

		Logger::debug(std::format("randomFileName: {}\n", randomFileName));
		fs::path copyPath = currentPath.parent_path() / randomFileName;
		Logger::debug(std::format("copyPath: {}\n", copyPath.string()));
		std::ifstream sourceFile(currentPath, std::ios::binary);
		std::ofstream destFile(copyPath, std::ios::binary);
		destFile << sourceFile.rdbuf();
		sourceFile.close();
		destFile.close();
		std::string commandLine{ std::format("{} -h {} -m {} -l {}",copyPath.string(),hours,minutes,currentPath.string()) };

		// 启动新进程
		STARTUPINFOA si;
		PROCESS_INFORMATION pi;

		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));

		CreateProcessA(
			NULL,                           // 模块名，使用可执行文件路径
			const_cast<char*>(commandLine.c_str()), // 命令行
			NULL,                           // 进程安全描述符
			NULL,                           // 线程安全描述符
			FALSE,                          // 继承句柄标志
			0,                              // 创建标志
			NULL,                           // 使用父进程的环境变量
			NULL,                           // 使用父进程的当前目录
			&si,                            // 启动信息
			&pi                             // 进程信息
		);
		exit(0);
	}
	else
	{
		Logger::debug(std::format("last_path: {}\n",last_path));
		fs::remove(last_path);
		SetConsoleTitleA(currentPath.filename().string().c_str());
	}
	if (!minutes && ! hours) return;
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
