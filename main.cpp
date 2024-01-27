#include <iostream>
#include <fstream>
#include <Windows.h>
#include <vector>
#include <future>
#include <thread>
#include "Logger.h"
#include "KeyManager.h"
#include "EEparser.h"
int main(int argc, char* argv[]) {
	Logger::info("�������������ʼ�������豸\n");
	KeyManager::InitKeyborad();
	Logger::info("�������������ƶ�λ�ó�ʼ������豸\n");
	KeyManager::InitMouse();
	Logger::info("��ݼ���\nU->��ѯ����\nI->�Զ������ң������-�س���ѭ��\nO->�������λ�� ��������������Ϊ��һ������λ�ã���ֹ����ƶ���\nP->�˳�����\n");
	while (1)
	{
		auto key = KeyManager::LastKeyUp();
		switch (key)
		{
		case 'U':
			Logger::debug("̧��U\n");
			std::thread([] {
				static EEparser ep; auto test = ep.CheckVoidTerrain();
				if (test.empty())
				{
					Logger::info("δ�ҵ�����\n");
				}
				for (const auto& it : test)
				{
					Logger::warning(std::format("�ҵ�{}\n", it));
					Beep(523, 500);//C
					Beep(659, 500);//E
					Beep(783, 500);//G
				}
				}).detach();
			break;
		case 'I':
			Logger::debug("̧��I\n");
			break;
		case 'O':
			Logger::debug("̧��O\n");
			break;
		case 'P':
			Logger::warning("̧��P\n");
			break;
		default:
			Logger::debug(std::format("̧��{:#X}\n",key));
			break;
		}
	}
}