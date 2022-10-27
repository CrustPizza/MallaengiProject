#pragma once
#include "Graphics.h"
#include "Input.h"
#include "Timer.h"

 /// <summary>
 /// 
 /// ��� ������ ����� ����� �� �ְ� �ϴ� Ŭ����
 /// �׷���, �̹� ��� ���� �ھ��� ���� ��Ұ� �̱������� �Ǿ��ִµ� .. �̰� �ǹ̰� �ֳ� ����.
 /// </summary>
class Engine
{
public:

	// Singlton 
	Engine();
	Engine(const Engine& ref) {};
	Engine& operator=(const Engine& ref) {};
	~Engine();

	static Engine& GetInstance()
	{
		static Engine instance;
		return instance;
	}

public:
	void Initialize(HWND hWnd);

	// �̰ɷ� ���� ����� �̾Ƽ� ������
	template <typename T>
	T& GetInstance()
	{
		return T::GetInstance();
	}
};