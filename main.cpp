#include <iostream>
#include <Windows.h>
#include "Logger.h"
#include "KeyManager.h"

int main(int argc, char* argv[]) {

	for (size_t i = 0; i < 3; i++)
	{
		Logger::debug(std::format("{}\n", i));
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		Logger::info(std::format("{}\n", i));
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		Logger::warning(std::format("{}\n", i));
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		Logger::error(std::format("{}\n", i));
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		Logger::fatal(std::format("{}\n", i));
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	Logger::info("���������ʼ�������豸\n");
	KeyManager::InitKeyborad();
	Logger::info("��ʼ�������豸���\n");
	std::this_thread::sleep_for(std::chrono::seconds(1));
	KeyManager::PushKey(VK_CONTROL);
	KeyManager::PushKey(VK_MENU);
	KeyManager::PushKey('A');
	KeyManager::PopKey('A');
	KeyManager::PopKey(VK_MENU);
	KeyManager::PopKey(VK_CONTROL);
	std::this_thread::sleep_for(std::chrono::seconds(1));
	KeyManager::SendKey(VK_ESCAPE);
	Logger::info("�ƶ����������ʼ������豸\n");
	KeyManager::InitMouse();
	Logger::info("��ʼ������豸���\n");
	
	KeyManager::MoveAbsolute(500,500);

	for (size_t i = 0; i < 500; i++)
	{
		KeyManager::MoveRelative(-1, -1);
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
	}
	KeyManager::SendKey(VK_LBUTTON);
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	KeyManager::SendKey(VK_RBUTTON);
}