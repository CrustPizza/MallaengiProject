#pragma once
#include "ButtonBase.h"

/// <summary>
/// ���� ũ�⸦ ���� ���� Progress Bar Ŭ����
/// 
/// 2022. 02. 24 JK
/// </summary>
class ProgressBar : public ButtonBase
{
	// ���� ���� ����
	float mValue;
	float mMax;

	// Front Bar���� Ȯ��
	bool mFront;

	// Bar�� ������� �� ȣ���� �Լ� ������
	void (*mFunc)(float Percent);

public:
	ProgressBar(Sprite* sprite, bool isFront, void (*func)(float Percent) = nullptr, float percent = 1.0f);
	~ProgressBar();
	
	virtual void Update() override;

	void SetPercent(float percent);

	void SetIncrease(float rate);
	void SetDecrease(float rate);
	virtual void OnButtonClicked() override;
};