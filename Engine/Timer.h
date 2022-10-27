#pragma once

#define TIMER_ID	1000

/// <summary>
/// 2022. 02. 08. MG
/// 
/// ��Ÿ Ÿ��. �ð��� ��� ���� Ŭ����
/// </summary>
class Timer
{
private:
	Timer();
	Timer(const Timer& ref) {};
	Timer& operator=(const Timer& ref) {};
	~Timer();

public:
	static Timer& GetInstance()
	{
		static Timer instance;
		return instance;
	}


	int mStartTime;			// ���� ���� �ð�
	int mSpriteCount;		// ī����
	float mFps;				// fps
	const int mN = 60;		// ����� ��� ���� ���� ��
	const int mFPS = 60;	// �����Ϸ��� fps

	// Quert . �����Բ� 
private:
	__int64 mCountsPerSec;
	double mSecondsPerCount;

	__int64 mPreviousCount;
	__int64 mCurrentCount;
	__int64 mDeltaCount;
	double mDeltaTime;

public:
	float GetEngineTime();

	float GetDeltaTime();
	float GetFPS();

	void Update();
};