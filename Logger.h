#pragma once
#include "Windows.h"
#include <string>
#include <format>
#include <iostream>

class Logger
{
public:
	static void debug(const std::string&& message);
	static void info(const std::string&& message);
	static void warning(const std::string&& message);
	static void error(const std::string&& message);
	static void fatal(const std::string&& message);
private:
	const static std::string& GetCurrentDateAndTime();
	inline static const int ConsoleCp{ (SetConsoleCP(65001) && SetConsoleOutputCP(65001)) ?  65001 : -1};//仅用于在main函数前改变控制台代码页
};