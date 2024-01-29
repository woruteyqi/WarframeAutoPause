#pragma once
#include <vector>
#include <string>
class EEparser
{
public:
	EEparser();
	EEparser(std::string Path) :EElogPath(Path) {};
	~EEparser();
	std::vector<std::string> CheckVoidTerrain() const;
private:
	const std::vector<std::pair<std::string, std::string>> VoidTerrains
	{
		{"虚空圆树","Sys [Info]: I: /Lotus/Levels/Orokin/CircularHub1.level"},
		{"虚空电梯","Sys [Info]: I: /Lotus/Levels/Orokin/LargeTieredIntermediate.level"}
	};

	std::string QueryForLastGenerate() const;
	std::string EElogPath;
};

