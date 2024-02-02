#include "Logger.h"
#include <chrono>
#include <ctime>
void Logger::debug(const std::string&& message)
{
#ifdef _DEBUG
	std::clog << GetCurrentDateAndTime() << "\033[2m[DEBUG] " << message << "\033[m";
#endif 
}

void Logger::info(const std::string&& message)
{
	std::clog << GetCurrentDateAndTime() << "\033[32m[INFO]\033[m " << message ;
}

void Logger::warning(const std::string&& message)
{
	std::clog << GetCurrentDateAndTime() << "\033[33;1m[WARNING] " << message << "\033[m";
}

void Logger::error(const std::string&& message)
{
	std::cerr << GetCurrentDateAndTime() << "\033[31;5m[ERROR]\033[m \033[31m" << message << "\033[m";
}

void Logger::fatal(const std::string&& message)
{
	std::cerr << GetCurrentDateAndTime() << "\033[101;5m[FATAL]\033[m \033[91;1m" << message << "\033[m";
}

const std::string& Logger::GetCurrentDateAndTime()
{
	auto currentTime = std::chrono::system_clock::now();
	auto time_t = std::chrono::system_clock::to_time_t(currentTime);
	tm tm{}; localtime_s(&tm,&time_t);

	static std::string formatTime{};
	formatTime = std::format("[{}-{}-{} {:0>2}:{:0>2}:{:0>2}.{:0>3}]",
		tm.tm_year + 1900,
		tm.tm_mon + 1,
		tm.tm_mday + 1,
		tm.tm_hour,
		tm.tm_min,
		tm.tm_sec,
		std::chrono::duration_cast<std::chrono::milliseconds>(currentTime.time_since_epoch()).count() % 1000
	);
	return std::ref(formatTime);
}
