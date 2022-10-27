#pragma once

#define DETECTING 30.f


/// <summary>
/// �÷��̾ ���� ����, ���� x��ǥ�� ������ ����
/// 
/// 2022. 02. 24. Comet
/// </summary>
class RedUp : public GameObject
{
public:
	RedUp(Vector2D dir, float moveSpeed);
	RedUp(Vector2D dir, float moveSpeed, float lengthDetection);

	virtual ~RedUp();

	Vector2D mDirrection;

	Player* mGamePlayer;

	// �ʴ� �� ��ǥ �����̽�
	float mMoveSpeed;

	bool mHasLength;

	float mLengthDetection;

	bool mTrigger;

public:
	virtual void Update() override;
};