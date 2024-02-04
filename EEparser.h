#pragma once
#include <vector>
#include <string>
class EEparser
{
public:
	EEparser();
	EEparser(std::string Path) :EElogPath(Path) {};
	~EEparser();
	std::vector<std::pair<std::string,int>> CheckTerrain() const;
private:
	const std::vector<std::pair<std::string, std::string>> VoidTerrains
	{
		{" ⌈虚空⌋ 圆树","Sys [Info]: I: /Lotus/Levels/Orokin/CircularHub1.level"},
		{" ⌈虚空⌋ 电梯","Sys [Info]: I: /Lotus/Levels/Orokin/LargeTieredIntermediate.level"},
		{" ⌈天王星⌋ 海底植物实验室","Sys [Info]: I: /Lotus/Levels/GrineerOcean/GrineerOceanIntermediateBotanyLab.level"}
	};

	std::string QueryForLastGenerate() const;
	std::string EElogPath;
};

