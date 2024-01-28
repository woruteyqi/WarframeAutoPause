#include "ImageProc.h"
#include "KeyManager.h"
#include "thread"
void ImageProc::AutoPause()
{
	auto sp1{ nor2scr(0.026041666f,0.046296295f) }, sp2{nor2scr(0.078125f,0.23148148f)};
	Logger::debug(std::format("�������p1-> X{}��Y{}��p2-> X{}��Y{}\n",sp1.first, sp1.second, sp2.first, sp2.second));
	int count{0};
	while (1)
	{
		long x{}, y{}, ret{};
		OP->FindColor(sp1.first, sp1.second, sp2.first, sp2.second, L"B81010-181010",0.7,7,&x,&y, &ret);
		if (ret)
		{
			Logger::debug(std::format("ƥ�䵽��ɫ��λ��X{},Y{}\n", x, y));
			Beep(880, 500);
			count++; 
			Logger::debug(std::format("��������������{}\n", count));
			if (count > 2)
			{
				Logger::warning("����������������ͣ\n");
				KeyManager::SendKey(VK_LBUTTON); Logger::debug("����VK_LBUTTON\n");
				std::this_thread::sleep_for(std::chrono::milliseconds(500));
				KeyManager::SendKey(VK_ESCAPE); Logger::debug("����VK_ESCAPE\n");
			}
		}
		else
		{
			count = 0;
			//Logger::debug("δ�ҵ�\n");
		}
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

void ImageProc::ActiveWindow()
{
	long ret{};
	OP->SetWindowState(WfGameWindow, 1, &ret);
}

std::pair<float, float> ImageProc::scr2nor(int X, int Y, int screenWidth, int screenHeight)
{
	return {static_cast<float>(X) / static_cast<float>(screenWidth) ,static_cast<float>(Y) / static_cast<float>(screenHeight)};
}

std::pair<int, int> ImageProc::nor2scr(float X, float Y, int screenWidth, int screenHeight)
{
	return {static_cast<int> (X * (float)screenWidth ),static_cast<int>(Y * (float)screenHeight)};
}
