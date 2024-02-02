#include "ImageProc.h"
#include "KeyManager.h"
#include "thread"
void ImageProc::AutoPause()
{
	auto sp1{ nor2scr(0.026041666f,0.046296295f) }, sp2{nor2scr(0.078125f,0.231481481f)};
	Logger::debug(std::format("检测区域p1-> X{}，Y{}，p2-> X{}，Y{}\n",sp1.first, sp1.second, sp2.first, sp2.second));
	int count{0};
	while (1)
	{
		long x{}, y{}, findret{};
		OP->FindColor(sp1.first, sp1.second, sp2.first, sp2.second, L"881010-181010|B00808-100808",0.9, 0, &x, &y, &findret);
		if (findret)
		{
			Logger::debug(std::format("匹配到颜色，位于X{},Y{}\n", x, y));
		}
		while (findret) 
		{
			long cmpret{};
			OP->CmpColor(x, y, L"881010-281010|B00808-100808", 0.3, &cmpret);
			if (cmpret)
			{
				count++;
				Logger::debug(std::format("连续红氧计数：{}\n", count));
				if (count > 2)
				{
					Beep(880, 1000);
					Logger::warning("连续红氧！尝试暂停\n");
					KeyManager::SendKey(VK_LBUTTON); Logger::debug("发送VK_LBUTTON\n");
					std::this_thread::sleep_for(std::chrono::milliseconds(500));
					KeyManager::SendKey(VK_ESCAPE); Logger::debug("发送VK_ESCAPE\n");
					std::this_thread::sleep_for(std::chrono::seconds(1));
				}
			}
			else
			{
				count = 0;
				findret = 0;
				//Logger::debug("未找到\n");
			}
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
