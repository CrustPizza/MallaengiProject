#pragma once

/// <summary>
/// ����Ƽ�� �̹��� ����� ���� ��ü
/// 
/// 2022. 02. 23
/// </summary>
class NarrativeSprite : public GameObject
{
	double mDelay;			// ���� �� ���� �ð�
	double mDuration;		// ��� �ð�
	int mAlphaSpeed;		// ���� ��ȯ �ӵ�
	float mAlpha;			// ��� ���İ�
	bool  mNext;			// ��� �� ��ȯ�ÿ� ����� ����

public:
	NarrativeSprite(Sprite* sprite, double delay, double duration, int speed);
	virtual ~NarrativeSprite();

	virtual void Update() override;
};