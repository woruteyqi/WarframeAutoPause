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
	KeyManager::PressKey(VK_ESCAPE);
}