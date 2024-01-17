#include <iostream>
#include <Windows.h>
#include "KeyManager.h"

int main(int argc, char* argv[]) {
	std::cout << "按任意键初始化键盘设备\n";
	KeyManager::InitKeyborad();
	std::cout << "初始化键盘设备完成\n";
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