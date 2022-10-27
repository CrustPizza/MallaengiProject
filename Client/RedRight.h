#pragma once

#define DETECTING 30.f

/**
 * \brief : �÷��̾ ���� ����, ���� y��ǥ�� ���� �� ���ư�
 */
class RedRight : public GameObject
{
public:
	RedRight(Vector2D dir, float moveSpeed);
	virtual ~RedRight();

	Player* mGamePlayer;

	Vector2D mDirrection;

	// �ʴ� �� ��ǥ �����̽�
	float mMoveSpeed;

	bool mTrigger;

public:
	virtual void Update() override;
};