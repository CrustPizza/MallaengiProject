//#pragma once
//
///// <summary>
///// Win32 ���� �κ�
///// </summary>
//class Window
//{
//public:
//	Window();
//	~Window();
//
//
//private:
//	HWND mHwnd;			// ���� �������� �ڵ�
//
//public:
//	// ���� ���α׷����� ���� �ʱ�ȭ�� ����
//	void Initialize();
//
//	// ������ ���α׷��� �ȿ����� ���ѷ���
//	void LoopAll();
//
//	bool MessageLoop(bool& isQuitMessage);
//
//
//	// ������ ���α׷����� ���� �� ����� �ϴ� ��
//	void Finalize();
//
//	// �޼����� �ޱ� ���� �ݹ� �Լ�
//	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
//
//	HWND GetWindowHandle() const { return mHwnd; }
//};

// Comet : ��� ������ Ŭ������� �Ǵܵ˴ϴ� ..