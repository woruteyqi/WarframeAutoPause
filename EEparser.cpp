#include <sstream>
#include <fstream>
#include <chrono>
#include <initializer_list>
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
		Logger::fatal("无法获取系统环境变量 %LocalAppData% 的值，将退出程序！！！\n");
		exit(-1);
	}
}

EEparser::~EEparser()
{
}

std::string EEparser::QueryForLastGenerate() const 
{
	std::ifstream EElog(EElogPath);
	if (!EElog)
	{
		Logger::fatal("打开EE失败，将退出程序！！！\n");
		exit(-1);
	}
	std::stringstream EE{};
	EElog.seekg(0, std::ios::end);
	Logger::debug(std::format("EE大小：{}字节，{:.2f}MB\n", (long)EElog.tellg(), (long)EElog.tellg() / 1024.0f / 1024.0f));
	EElog.seekg(0, std::ios::beg);
	auto begin = std::chrono::high_resolution_clock::now();
	EE << EElog.rdbuf();
	auto end = std::chrono::high_resolution_clock::now();
	Logger::debug(std::format("读取EE文件耗时：{}\n", std::chrono::duration_cast<std::chrono::milliseconds>(end - begin)));
	auto str = EE.str();
	auto lastpos = str.rfind("Sys [Info]: Generated layout in");
	lastpos = str.rfind("\n", lastpos);
	Logger::debug(std::format("最后一次地形生成开始位置：{}\n", lastpos));
	auto endpos = str.find("Sys [Info]: \n", lastpos + 100);
	endpos = str.find("\n", endpos);
	Logger::debug(std::format("最后一次地形生成终止位置：{}\n", endpos));
	auto sub = str.substr(lastpos, endpos - lastpos);
	Logger::debug(std::format("最后一次地形生成的信息：{}\n", sub));
	EElog.close();
	return sub;
}


std::vector<std::string> EEparser::CheckVoidTerrain() const
{
	const std::string lastGenerate{ QueryForLastGenerate() };
	std::vector<std::string> result{ };
	for (const auto& it : VoidTerrains)
	{
		auto pos = lastGenerate.find(it.second);
		if (pos != std::string::npos)
		{
			result.push_back(std::string(it.first).append(pos < 250 ? "，在第一个大地形" : "，不在第一个大地形"));
		}
	}
	return result;
}
