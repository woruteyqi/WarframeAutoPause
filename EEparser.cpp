#include <fstream>
#include <filesystem>
#include <chrono>
#include <algorithm>
#include "EEparser.h"
#include "Logger.h"


EEparser::EEparser()
{
	char* localappdada{};
	size_t length{};
	if (_dupenv_s(&localappdada, &length, "LOCALAPPDATA") == 0) {
		Logger::debug(std::format("系统环境变量 %LocalAppData% 为：\"{}\"，路径长度：{} 字节\n", localappdada, length));
		this->EElogPath = std::move(localappdada);
		this->EElogPath.append("\\Warframe\\EE.log");
		Logger::debug(std::format("EE 路径为：\"{}\"，路径长度 {} 字节\n", EElogPath, EElogPath.size()));
	}
	else {
		Logger::error("无法获取系统环境变量 %LocalAppData% 的值\n");
	}
}

EEparser::~EEparser()
{
}

std::pair<std::string, long long> EEparser::QueryForLastGenerate() const
{
	auto currentPath{ std::filesystem::current_path() / "EE.log" };
	std::filesystem::copy_file(EElogPath, currentPath,std::filesystem::copy_options::overwrite_existing);
	Logger::debug(std::format("从{}复制到{}\n", EElogPath, currentPath.string()));
	std::ifstream EElog(currentPath);
	if (!EElog)
	{
		Logger::error("打开EE失败\n");
		return { "",0 };
	}
	EElog.seekg(0, std::ios::end);
	long fileSize{ (long)EElog.tellg() };
	Logger::debug(std::format("EE大小：{}字节，{:.2f}MB\n", fileSize, fileSize / 1024.0f / 1024.0f));
	
	auto begin{ std::chrono::high_resolution_clock::now() };
	std::string Buffer;
	Buffer.resize(fileSize);
	EElog.seekg(0, std::ios::beg);
	EElog.read(&Buffer[0], fileSize);
	auto end{ std::chrono::high_resolution_clock::now() };
	Logger::debug(std::format("读取EE文件耗时：{}\n", std::chrono::duration_cast<std::chrono::milliseconds>(end - begin)));
	EElog.close();
	std::filesystem::remove(currentPath);
	Logger::debug(std::format("读取完成，删除临时文件{}\n", currentPath.string()));
	
	auto ftime_begin{ Buffer.find("Current time: ") + 14 };
	auto ftime_end{ ftime_begin };
	for (size_t i{};i<6;i++) ftime_end = Buffer.find(' ', ftime_end + 1);
	auto start_time{ Buffer.substr(ftime_begin, ftime_end - ftime_begin) };
	Logger::debug(std::format("time_string:{}\n", start_time));
	std::tm timeInfo = {};
	std::istringstream ss(start_time);
	ss >> std::get_time(&timeInfo, "%a %b %d %H:%M:%S %Y");
	std::chrono::system_clock::time_point timePoint = std::chrono::system_clock::from_time_t(std::mktime(&timeInfo));
	auto timestamp = std::chrono::duration_cast<std::chrono::seconds>(timePoint.time_since_epoch()).count();


	auto lastpos{ Buffer.rfind("Sys [Info]: Generated layout in") };
	lastpos = Buffer.rfind("\n", lastpos);
	Logger::debug(std::format("最后一次地形生成开始位置：{}\n", lastpos));
	auto endpos{ Buffer.find("Sys [Info]: \n", lastpos + 100) };
	endpos = Buffer.find("\n", endpos);
	Logger::debug(std::format("最后一次地形生成终止位置：{}\n", endpos));
	auto LastGenerate{ Buffer.substr(lastpos, endpos - lastpos) };
	Logger::debug(std::format("最后一次地形生成的信息：{}\n", LastGenerate));
	
	
	return { LastGenerate,timestamp + std::atoi(LastGenerate.substr(0, LastGenerate.find_first_of('.')).c_str())};
}


std::vector<std::pair<std::string, int>> EEparser::CheckTerrain() const
{
	std::vector<std::pair<std::string, int>> result{ };
	auto generate{ QueryForLastGenerate() };
	const std::string lastGenerate{ generate.first };
	if (lastGenerate.empty()) return result;

	std::chrono::system_clock::time_point timePoint = std::chrono::system_clock::from_time_t(generate.second);
	std::time_t time = std::chrono::system_clock::to_time_t(timePoint);
	std::tm tm{}; localtime_s(&tm, &time);
	Logger::info(std::format("最后一次地形生成于 ⌈{}⌋\n",
		std::format("{}年{}月{}日 {:0>2}时{:0>2}分{:0>2}秒", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec)));

	size_t IntermediatePos[3]{std::string::npos};
	IntermediatePos[0] = lastGenerate.find("Sys [Info]: I:");
	IntermediatePos[1] = lastGenerate.find("Sys [Info]: I:", IntermediatePos[0] == std::string::npos ? IntermediatePos[0] : IntermediatePos[0] + 50);
	IntermediatePos[2] = lastGenerate.find("Sys [Info]: I:", IntermediatePos[1] == std::string::npos ? IntermediatePos[1] : IntermediatePos[1] + 50);
	Logger::debug(std::format("大地形1位置{}，大地形2位置{}，大地形3位置{}\n", IntermediatePos[0], IntermediatePos[1], IntermediatePos[2]));
	for (const auto& it : SleepTerrains)
	{
		auto pos = lastGenerate.find(it.second);
		if (pos != std::string::npos)
		{
			Logger::debug(std::format("找到睡觉地形，位置{}\n", pos));
			result.push_back({
				it.first,
				[&] {
					if (IntermediatePos[0] != std::string::npos && pos == IntermediatePos[0])
					{
						return 1;
					}
					if (IntermediatePos[1] != std::string::npos && pos == IntermediatePos[1])
					{
						return 2;
					}
					if (IntermediatePos[2] != std::string::npos && pos == IntermediatePos[2])
					{
						return 3;
					}
					return -1;
			}() });
		}
	}
	return result;
}
