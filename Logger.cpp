#include "Logger.h"
#include <chrono>
#include <ctime>
void Logger::debug(const std::string&& message)
{
	std::clog << GetCurrentTime() << "\033[2m[DEBUG]\033[m " << message;
}

void Logger::info(const std::string&& message)
{
	std::clog << GetCurrentTime() << "[INFO] " << message;
}

void Logger::warning(const std::string&& message)
{
	std::clog << GetCurrentTime() << "\033[33m[WARNING]\033[m " << message ;
}

void Logger::error(const std::string&& message)
{
	std::cerr << GetCurrentTime() << "\033[31m[ERROR]\033[m " << message ;
}

void Logger::fatal(const std::string&& message)
{
	std::cerr << GetCurrentTime() << "\033[101m[FATAL]\033[m " << message;
}

const std::string& Logger::GetCurrentTime()
{
	auto currentTime = std::chrono::system_clock::now();
	auto time_t = std::chrono::system_clock::to_time_t(currentTime);
	tm tm{}; localtime_s(&tm,&time_t);

	static std::string formatTime{};
	formatTime = std::format("[{}年{}月{}日{:0>2}时{:0>2}分{:0>2}秒]",
		tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_yday + 1, tm.tm_hour, tm.tm_min, tm.tm_sec);
	return formatTime;
}
