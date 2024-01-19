#pragma once
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
	const static std::string& GetCurrentTime();
};