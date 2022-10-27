#pragma once

enum class KEYSTATE
{
	NONE,
	HOLD,
	DOWN,
	UP
};

/// <summary>
/// 2022. 02. 08. MG
/// 
/// ������ â ������ Ű �Է��� ���� Ŭ����. (Ű����, ���콺)
/// </summary>
class Input
{
public:
	Input();
	Input(const Input& ref) {};
	Input& operator=(const Input& ref) {};
	~Input();

	static Input& GetInstance()
	{
		static Input instance;
		return instance;
	}

	void Initialize(HWND hWnd);

private:
	Vector2D mCurrentMousePos;		// ���� ���콺 ��ġ

	HWND mHwnd;

	bool mCurrentKey;		// ���� Ű����
	bool mPrevKey;			// ���� Ű����

public:
	void Update();					// Ű�Է� �ʱ�ȭ

	KEYSTATE GetKeyState(int vk);
	Vector2D GetCurrentMousePos() { return mCurrentMousePos; }
};

static Input mKeyStateArr[256];		// Ű�Է� ���� �迭