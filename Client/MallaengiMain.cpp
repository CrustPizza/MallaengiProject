#include "PreCompiledHeader.h"
#include "Game.h"
#define _CRTDBG_MAP_ALLOC

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void SetFullScreen(HWND hWnd, bool fullScreen)
{
	if (fullScreen == true)
	{
		LONG wndStyle = GetWindowLong(hWnd, GWL_STYLE);
		LONG wndExStyle = GetWindowLong(hWnd, GWL_EXSTYLE);

		SetWindowLong(hWnd, GWL_STYLE, wndStyle & ~(WS_CAPTION | WS_THICKFRAME));
		SetWindowLong(hWnd, GWL_EXSTYLE,
			wndExStyle & ~
			(WS_EX_DLGMODALFRAME | WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE));

		MONITORINFO monitor_info;

		monitor_info.cbSize = sizeof(monitor_info);
		GetMonitorInfo(MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST), &monitor_info);
		
		RECT wndRect = monitor_info.rcMonitor;

		float x = wndRect.left;
		float y = wndRect.top;
		float width = wndRect.right - x;
		float height = wndRect.bottom - y;

		//float aspectRatio = width / height;

		//if (aspectRatio < 15.0f / 9.0f)
		//{
		//	float magnification = width / 16.0f;
		//	float yRatio = 9.0f / (height / magnification);
		//	y = (height * (1 - yRatio)) / 2;

		//	height *= yRatio;
		//}
		//else if (aspectRatio > 17.0f / 9.0f)
		//{
		//	float magnification = height / 9.0f;
		//	float xRatio = 16.0f / (width / magnification);
		//	x = (width * (1 - xRatio)) / 2;

		//	width *= xRatio;
		//}

		SetWindowPos(hWnd, nullptr, x, y, width, height, SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
	}
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR	lpCmdLine, int nCmdShow)
{
	TCHAR szAppName[] = _T("Mallaengi Project");
	HWND hWnd;
	MSG msg;
	WNDCLASS wndclass;

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;

	RegisterClass(&wndclass);

	RECT deskWnd;
	GetWindowRect(GetDesktopWindow(), &deskWnd);

	hWnd = CreateWindow(szAppName, szAppName, WS_OVERLAPPEDWINDOW,
		0, 0, 1920, 1080, NULL, NULL, hInstance, NULL);

	// �����̿�
	/*hWnd = CreateWindow(szAppName, szAppName, WS_OVERLAPPEDWINDOW,
		100, 100, 1920, 1080, NULL, NULL, hInstance, NULL);*/

	if (!hWnd) return FALSE;

	SetFullScreen(hWnd, true);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	Game::GetInstance().Initialize(hWnd);

	while (TRUE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;
			DispatchMessage(&msg);
		}
		else
		{
			// Game Loop (Option�� ������ �ʾ��� �� ���ư��� �Ǵ°��� ..?)
			if (Game::GetInstance().GameLoop())
				break;
		}
	}

	SoundManager::GetInstance()->DestroyInstance();

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC			hdc;
	PAINTSTRUCT ps;

	switch (message)
	{
		case WM_CREATE:
		{
			// Game Initialize !
		}
		break;

		case WM_SIZE:
		{
			// â ����� ����Ǹ� ����Ÿ���� �ٽ� �������ݴϴ�.
			Graphics::GetInstance().Resize();
		}
		break;

		case WM_ACTIVATE:
		{
			// wParam���� ������ â�� Ȱ��ȭ / ��Ȱ��ȭ �Ǿ��ٴ� �޽����� ���´�.
			// ���� ���� ������ �׻� ���� ������ ������ �ɼ��� ����� ������ �����Ѵ�.
			if (wParam != false)
				SetWindowPos(hWnd, HWND_TOPMOST, 100, 0, 1920, 1080, SWP_NOSIZE | SWP_NOMOVE);
			else
				SetWindowPos(hWnd, HWND_NOTOPMOST, 100, 0, 1920, 1080, SWP_NOSIZE | SWP_NOMOVE);
		}
		break;

		case WM_PAINT:
		{
			hdc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
		}
		break;

		case WM_DISPLAYCHANGE:
		{

		}
		break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		default:
			return DefWindowProc(hWnd,
				message, wParam, lParam);
	}

	return 0;
}