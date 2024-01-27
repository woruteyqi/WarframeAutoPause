#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <future>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <random>
#include <Windows.h>
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
		"I->�Զ������ң������-�س���ѭ����ʹ������ӳ�\n"
		"O->�������λ�� ��������������Ϊ��һ������λ�ã���ֹ����ƶ���\n"
		"P->�˳�����\n");
	while (1)
	{
		UINT key{ KeyManager::LastKeyUp() };
		static std::mutex mtx;
		static std::condition_variable cv;
		static bool flag[3]{false};
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
			flag[0] = !flag[0];
			flag[0] ? Logger::info("��ʼ������\n") : Logger::info("ֹͣ������\n");
			if (flag[0])
			{
				std::thread([&] {
					std::random_device rd;
					std::lock_guard<std::mutex> lock(mtx);
					flag[1] = false;
					while (flag[0])
					{
						KeyManager::SendKey(VK_LBUTTON, std::uniform_int_distribution<>(20, 50)(rd)); Logger::debug("����������\n");
						std::this_thread::sleep_for(std::chrono::milliseconds(std::uniform_int_distribution<>(300, 500)(rd)));
						KeyManager::SendKey(VK_RETURN, std::uniform_int_distribution<>(20, 50)(rd)); Logger::debug("������̻س�\n");
						std::this_thread::sleep_for(std::chrono::milliseconds(std::uniform_int_distribution<>(1500, 3000)(rd)));
					}
					flag[1] = true;
					cv.notify_one();
				}).detach();
			}
			break;
		case 'O':
			Logger::debug("̧��O\n");
			flag[2] = !flag[2];
			flag[2] ? Logger::info("�������λ��\n") : Logger::info("�������λ��\n");
			KeyManager::MoveAbsolute(390, 280);
			KeyManager::LockMousePosition(flag[2]);
			break;
		case 'P':
			Logger::debug("̧��P\n");
			exitcount++;
			if (exitcount > 1)
			{
				if (flag[0])
				{
					Logger::info("Ŀǰ�п��������񣡵ȴ��������߳̽���\n");
					flag[0] = false;
					std::unique_lock<std::mutex> lock(mtx);
					cv.wait(lock, [] {return flag[1]; });
				}
				Logger::info("���˳�\n");
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