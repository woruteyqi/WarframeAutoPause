#include <iostream>
#include <fstream>
#include <Windows.h>
#include <vector>
#include <future>
#include <mutex>
#include <thread>
#include "Logger.h"
#include "KeyManager.h"
#include "EEparser.h"
int main(int argc, char* argv[]) {
	Logger::info("�������������ʼ�������豸\n");
	KeyManager::InitKeyborad();
	Logger::info("�������������ƶ�λ�ó�ʼ������豸\n");
	KeyManager::InitMouse();
	Logger::info("��ݼ���\n"
		"U->��ѯ���Σ���ѯ����������ʾ��\n"
		"I->�Զ������ң������-�س���ѭ��\n"
		"O->�������λ�� ��������������Ϊ��һ������λ�ã���ֹ����ƶ���\n"
		"P->�˳�����\n");

	while (1)
	{
		UINT key{ KeyManager::LastKeyUp() };
		static bool flag{ false };
		static int exitcount{ 0 };

		switch (key)
		{
		case 'U':
			Logger::debug("̧��U\n");
			std::thread([] {
				Logger::info("��ʼѰ����յ���\n");
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
			flag = !flag;
			flag ? Logger::info("�������λ��\n") : Logger::info("�������λ��\n");
			KeyManager::MoveAbsolute(390, 280);
			KeyManager::LockMousePosition(flag);
			break;
		case 'P':
			Logger::debug("̧��P\n");
			exitcount++;
			if (exitcount > 1)
			{
				exit(1);
			}
			Logger::warning("ȷ���˳����ٰ�P\n");
			break;
		default:
			exitcount = 0;
			Logger::debug(std::format("̧��{:#X}\n",key));
			break;
		}
	}
}