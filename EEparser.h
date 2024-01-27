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
		{"Ðé¿ÕÔ²Ê÷","Sys [Info]: I: /Lotus/Levels/Orokin/CircularHub1.level"},
		{"Ðé¿ÕµçÌÝ","Sys [Info]: I: /Lotus/Levels/Orokin/LargeTieredIntermediate.level"}
	};

	std::string QueryForLastGenerate() const;
	std::string EElogPath;
};

