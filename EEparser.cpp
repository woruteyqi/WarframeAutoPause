#include <sstream>
#include <fstream>
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

std::string EEparser::QueryForLastGenerate() const 
{
	std::ifstream EElog(EElogPath);
	if (!EElog)
	{
		Logger::error("打开EE失败\n");
		return "";
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


std::vector<std::pair<std::string, int>> EEparser::CheckTerrain() const
{
	const std::string lastGenerate{ QueryForLastGenerate() };
	std::vector<std::pair<std::string, int>> result{ };
	size_t IntermediatePos[3]{std::string::npos};
	IntermediatePos[0] = lastGenerate.find("[Info]: I:");
	IntermediatePos[1] = lastGenerate.find("[Info]: I:", IntermediatePos[0] == std::string::npos ? IntermediatePos[0] : IntermediatePos[0] + 50);
	IntermediatePos[2] = lastGenerate.find("[Info]: I:", IntermediatePos[1] == std::string::npos ? IntermediatePos[1] : IntermediatePos[1] + 50);
	Logger::debug(std::format("大地形1位置{}，大地形2位置{}，大地形3位置{}\n", IntermediatePos[0], IntermediatePos[1], IntermediatePos[2]));
	for (const auto& it : VoidTerrains)
	{
		auto pos = lastGenerate.find(it.second);
		if (pos != std::string::npos)
		{
			Logger::debug(std::format("找到地形位置{}\n", pos));
			result.push_back({
				it.first,
				[&] {
					if (IntermediatePos[0] != std::string::npos && pos < IntermediatePos[0])
					{
						return 1;
					}
					if (IntermediatePos[1] != std::string::npos && pos < IntermediatePos[1] && pos > IntermediatePos[0])
					{
						return 2;
					}
					if (IntermediatePos[2] != std::string::npos && pos < IntermediatePos[2] && pos > IntermediatePos[1])
					{
						return 3;
					}
					return -1;
			}() });
		}
	}
	return result;
}
