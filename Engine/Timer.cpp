#include "pch.h"
#include "Timer.h"

Timer::Timer()
	: mSecondsPerCount(0.0), mDeltaCount(0), mPreviousCount(0), mCurrentCount(0)
	,mDeltaTime(0)
{
	// ī���� üũ. 1�ʴ� �� ���� ���� ���ΰ�?
	QueryPerformanceFrequency((LARGE_INTEGER*)&mCountsPerSec);

	QueryPerformanceCounter((LARGE_INTEGER*)&mPreviousCount);

	// 1ī��Ʈ�� �� �ʳ� �ɸ���?
	mSecondsPerCount = 1.0 / (double)mCountsPerSec;
}

Timer::~Timer()
{
}

float Timer::GetEngineTime()
{
	// ó�� ���� ���� ����
	static ULONGLONG oldTime = GetTickCount64();

	// ������ �ð�
	ULONGLONG nowTime = GetTickCount64();

	float dTime = (nowTime - oldTime) * 0.001f;		// ��ƽī��Ʈ�� 1000�̹Ƿ� �����ֱ�

	oldTime = nowTime;

	return dTime;
}

float Timer::GetDeltaTime()
{
	mDeltaTime = mDeltaCount * mSecondsPerCount;

	// ���� double���� float�� �߸����� �װ��� �߿��� �Լ��� �ƴϴ�.
	// �и�������� ��ȯ �� �ش�.
	return (float)mDeltaTime * 1000.0f;
}

float Timer::GetFPS()
{
	if (mDeltaTime != 0)
	{
		return (float)(1.0 / mDeltaTime);
	}
	return 0;
}

void Timer::Update()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&mCurrentCount);

	// Time difference between this frame and the previous.
	mDeltaCount = (mCurrentCount - mPreviousCount);

	// ī��Ʈ�� �ð��� �и��Ѵ�.
	mDeltaTime = mDeltaCount * mSecondsPerCount;

	// Prepare for next frame.
	mPreviousCount = mCurrentCount;

	// Force nonnegative.  The DXSDK's CDXUTTimer mentions that if the 
	// processor goes into a power save mode or we get shuffled to another
	// processor, then mDeltaTime can be negative.
	if (mDeltaCount < 0)
	{
		mDeltaCount = 0;
	}
}