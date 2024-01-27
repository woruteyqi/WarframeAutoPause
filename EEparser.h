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
		{"���Բ��","Sys [Info]: I: /Lotus/Levels/Orokin/CircularHub1.level"},
		{"��յ���","Sys [Info]: I: /Lotus/Levels/Orokin/LargeTieredIntermediate.level"}
	};

	std::string QueryForLastGenerate() const;
	std::string EElogPath;
};

