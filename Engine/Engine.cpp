#include "pch.h"
#include "Engine.h"

#include "Input.h"
#include "Graphics.h"
#include "Timer.h"

Engine::Engine()
{
	
}

Engine::~Engine()
{
	// ������ ����Ǹ�, ������ ���� ��
}

void Engine::Initialize(HWND hWnd)
{
	// Input �ʱ�ȭ
	GetInstance<Input>().Initialize(hWnd);

	// Graphics �ʱ�ȭ
	GetInstance<Graphics>().Initialize(hWnd);

	// Timer �ʱ�ȭ
	GetInstance<Timer>();
}