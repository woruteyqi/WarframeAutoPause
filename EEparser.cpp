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
		Logger::debug(std::format("%localappdata% Ϊ��\"{}\"��·�����ȣ�{} �ֽ�\n", localappdada, length));
		this->EElogPath = std::move(localappdada);
		this->EElogPath.append("\\Warframe\\EE.log");
		Logger::debug(std::format("EE ·��Ϊ��\"{}\"��·������ {} �ֽ�\n", EElogPath, EElogPath.size()));
	}
	else {
		Logger::fatal("�޷���ȡ�������� LOCALAPPDATA ��ֵ�������˳�������\n");
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
		Logger::fatal("��EEʧ�ܣ����˳����򣡣���\n");
		exit(-1);
	}
	std::stringstream EE{};
	EElog.seekg(0, std::ios::end);
	Logger::debug(std::format("EE��С��{}�ֽڣ�{:.2f}MB\n", (long)EElog.tellg(), (long)EElog.tellg() / 1024.0f / 1024.0f));
	EElog.seekg(0, std::ios::beg);
	auto begin = std::chrono::high_resolution_clock::now();
	EE << EElog.rdbuf();
	auto end = std::chrono::high_resolution_clock::now();
	Logger::debug(std::format("��ȡEE�ļ���ʱ��{}\n", std::chrono::duration_cast<std::chrono::milliseconds>(end - begin)));
	auto str = EE.str();
	auto lastpos = str.rfind("Sys [Info]: Generated layout in");
	lastpos = str.rfind("\n", lastpos);
	Logger::debug(std::format("���һ�ε������ɿ�ʼλ�ã�{}\n", lastpos));
	auto endpos = str.find("Sys [Info]: \n", lastpos + 100);
	endpos = str.find("\n", endpos);
	Logger::debug(std::format("���һ�ε���������ֹλ�ã�{}\n", endpos));
	auto sub = str.substr(lastpos, endpos - lastpos);
	Logger::debug(std::format("���һ�ε������ɵ���Ϣ��{}\n", sub));
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
			result.push_back(std::string(it.first).append(pos < 250 ? "���ڵ�һ�������" : "�����ڵ�һ�������"));
		}
	}
	return result;
}
