#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <random>
#include <cmath>
#include <Windows.h>
#include "Logger.h"
#include "KeyManager.h"
#include "EEparser.h"
#include "ImageProc.h"
int main(int argc, char* argv[]) {
	Logger::info("�������������ʼ�������豸\n");
	KeyManager::InitKeyborad();
	Logger::info("�������������ƶ�λ�ó�ʼ������豸\n");
	KeyManager::InitMouse();
	std::thread(ImageProc::AutoPause).detach();
	Logger::info("������ͣ�ѿ������뽫��ͼģʽ����Ϊȫ����ͼ��ѡ��>����>�Ե����ͼ��ʾ������Ĭ�ϰ�M���л���\n"
		"ע�⣺��ϷӦΪ�ޱ߿�ȫ��ģʽ"
		"ע�⣺��⵽�����ᷢ������������������3�δ�����ͣ\n\n"
		"��ݼ���\n"
		"U->��ѯ���Σ���ѯ����������ʾ��\n"
		"I->�Զ������ң������-�س���ѭ����ʹ������ӳ�\n"
		"O->�������λ�ã�������������Ϊ��һ������λ�ã���ֹ����ƶ���ͬʱ�ᱣ����Ϸ���ڼ�����ڶ��㣩\n"
		"P->�˳�����\n\n");
	while (1)
	{
		UINT key{ KeyManager::LastKeyUp() };
		static bool flag[3]{false};
		static int exitcount{ 0 };

		switch (key)
		{
		case 'U':
			//Logger::debug("̧��U\n");
			std::thread([] {
				Logger::info("��ʼѰ����յ���\n");
				static EEparser ep; auto test = ep.CheckVoidTerrain();
				if (test.empty())
				{
					Logger::info("δ�ҵ�����\n");
				}
				int count{ 0 };
				for (const auto& it : test)
				{
					
					Logger::warning(std::format("�ҵ�{}\n", it));
					const auto& GetFreq = [](int HalfSteps)
					{
						const int A4{ 440 };
						int Freq{ static_cast<int>(std::pow((float)2, (float)HalfSteps / (float)12) * (float)A4) };
						Logger::debug(std::format("HalfSteps: {} Ƶ�ʣ�{}\n", HalfSteps, Freq));
						return Freq;
					};
					Beep(GetFreq(3 + count), 500);
					Beep(GetFreq(7 + count), 500);
					Beep(GetFreq(10 + count), 500);
					count ++;
				}
			}).detach();
			break;
		case 'I':
			//Logger::debug("̧��I\n");
			flag[0] = !flag[0];
			flag[0] ? Logger::info("��ʼ������\n") : Logger::info("ֹͣ������\n");

			static auto onece = [&] {
				std::thread([&] {
					std::random_device rd;
					while (1)
					{
						if (flag[0])
						{
							KeyManager::SendKey(VK_LBUTTON, std::uniform_int_distribution<>(20, 50)(rd)); Logger::debug("����VK_LBUTTON\n");
							std::this_thread::sleep_for(std::chrono::milliseconds(std::uniform_int_distribution<>(300, 500)(rd)));
							KeyManager::SendKey(VK_RETURN, std::uniform_int_distribution<>(20, 50)(rd)); Logger::debug("����VK_RETURN\n");
							std::this_thread::sleep_for(std::chrono::milliseconds(std::uniform_int_distribution<>(1500, 3000)(rd)));
						}
					}
				}).detach();
				return 0;
			}();
			break;
		case 'O':
			//Logger::debug("̧��O\n");
			flag[2] = !flag[2];
			flag[2] ? Logger::info("�������λ��\n") : Logger::info("�������λ��\n");
			KeyManager::MoveAbsolute(390, 280);
			KeyManager::LockMousePosition(flag[2]);
			break;
		case 'P':
			//Logger::debug("̧��P\n");
			exitcount++;
			if (exitcount > 1)
			{
				Logger::info("���˳�\n");
				exit(1);
			}
			Logger::warning("ȷ���˳����ٰ�P��������ǿ�ݼ������ü���\n");
			break;
		default:
			exitcount = 0;
			//Logger::debug(std::format("̧��{:#X}\n",key));
			break;
		}
	}
}