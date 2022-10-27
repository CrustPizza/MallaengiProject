#include "pch.h"
#include "Input.h"

Input::Input()
	: mCurrentKey(false),
	mPrevKey(false)
{
}

Input::~Input()
{
}

void Input::Initialize(HWND hWnd)
{
	mHwnd = hWnd;
}

void Input::Update()
{
	// 
	for (int i = 0; i < 255; i++)
	{
		mKeyStateArr[i].mPrevKey = mKeyStateArr[i].mCurrentKey;
		mKeyStateArr[i].mCurrentKey = HIWORD(GetAsyncKeyState(i));
	}

	POINT currentMousePos;

	// ���� ȭ���� ���콺 �ޱ� 
	GetCursorPos(&currentMousePos);
	ScreenToClient(mHwnd, &currentMousePos);

	D2D1_SIZE_F temp = Graphics::GetInstance().GetOffScreenSize();
	float temp2 = Graphics::GetInstance().GetScreenSize().width;

	float scale = temp.height / 1080.0f;

	// ���콺 ���� ��ġ �ޱ�
	mCurrentMousePos.mX = (temp2 - temp.width / scale) / 2  + currentMousePos.x / scale;
	mCurrentMousePos.mY = (currentMousePos.y) / scale;
}

KEYSTATE Input::GetKeyState(int vk)
{
	// �װ��� ��Ȳ ��ȯ
	if (!mKeyStateArr[vk].mPrevKey && mKeyStateArr[vk].mCurrentKey)
		return KEYSTATE::DOWN;
	else if (!mKeyStateArr[vk].mPrevKey && !mKeyStateArr[vk].mCurrentKey)
		return KEYSTATE::NONE;
	else if (mKeyStateArr[vk].mPrevKey && !mKeyStateArr[vk].mCurrentKey)
		return KEYSTATE::UP;
	else
		return KEYSTATE::HOLD;
}