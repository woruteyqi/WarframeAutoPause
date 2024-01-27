#pragma once
#include "lib/Interception/interception.h"
#include <chrono>
#include <thread>
#include <future>
#include <functional>
#include <Windows.h>

class KeyManager
{
public:
	//�����̣߳���ʼ�������豸
	static void InitKeyborad();

	//�����̣߳���ʼ������豸
	static void InitMouse();

	//���°���
	static void PushKey(const UINT VK);

	//̧�𰴼�
	static void PopKey(const UINT VK);

	//���Ͱ���
	static void SendKey(const UINT VK, const UINT interval_milliseconds = 0);

	//�����̣߳�ֻ�����¸��������󷵻�true
	static bool WaitKeyDown(const UINT VK);

	//�����̣߳�ֻ��̧����������󷵻�true
	static bool WaitKeyUp(const UINT VK);

	//�����̣߳����ص�һ�����µİ���
	static UINT LastKeyDown();

	//�����̣߳����ص�һ��̧��İ���
	static UINT LastKeyUp();

	//����ƶ�����������
	static void MoveAbsolute(const UINT x, const UINT y);

	//����ƶ����������
	static void MoveRelative(const int x, const int y);

	//�������λ��
	static void LockMousePosition(const bool isLock);
private:
	class Keyborad
	{
	public:
		Keyborad();
		~Keyborad();
		void PushKey(const UINT VSC);
		void PopKey(const UINT VSC);
		std::pair<USHORT, USHORT> GetKeyState();
		bool WaitKey(const UINT VSC, const bool pushORpop);
	private:
		InterceptionContext Context;
		InterceptionDevice  Device;
	}; 
	static Keyborad* KeyboradApp;

	class Mouse
	{
	public:
		Mouse();
		~Mouse();
		void PushKey(const USHORT MouseKey);
		void PopKey(const USHORT MouseKey);
		void MovePosition(const int x, const int y, const bool absolutORrelative);
		void LockPosition(const bool isLock);
	private:
		InterceptionContext Context;
		InterceptionDevice  Device;
	}; 
	static Mouse* MouseApp;
};