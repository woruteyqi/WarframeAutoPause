#include <iostream>
#include <Windows.h>
#include "KeyManager.h"

int main(int argc, char* argv[]) {
	std::cout << "���������ʼ�������豸\n";
	KeyManager::InitKeyborad();
	std::cout << "��ʼ�������豸���\n";
	std::this_thread::sleep_for(std::chrono::seconds(1));
	KeyManager::PushKey(VK_CONTROL);
	KeyManager::PushKey(VK_MENU);
	KeyManager::PushKey('A');
	KeyManager::PopKey('A');
	KeyManager::PopKey(VK_MENU);
	KeyManager::PopKey(VK_CONTROL);
	std::this_thread::sleep_for(std::chrono::seconds(1));
	KeyManager::SendKey(VK_ESCAPE);

	std::cout << "�ƶ����������ʼ������豸\n";
	KeyManager::InitMouse();
	std::cout << "��ʼ������豸���\n";
	
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