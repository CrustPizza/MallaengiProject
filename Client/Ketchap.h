#pragma once
#include <random>
#include "Player.h"

#define KETCHAP_PLUS 0.5f

/**
 * \brief : ����. ����ٴѴ�.
 */
class Ketchap : public GameObject
{
public:
	Ketchap();
	virtual ~Ketchap();

	Vector2D mVelocity;

	bool mTrigger;

	float mGravity;

	Player* mGamePlayer;
	
	float mTimer;

	static std::random_device mRd;

	static std::mt19937 mMt;

	static std::uniform_int_distribution<int> mDistVelocityX;

	static std::uniform_int_distribution<int> mDistVelocityY;

public:
	virtual void Update() override;
};

// �갡 ������ ��, ������ �ӵ� (x,y : ������ ���� ������) �� ������ ������

// �׸��� �� ������Ʈ���� �� �ӵ��� ���� ������

// �ӵ� �� y value�� �Ź� �߷��� ������ ����. �̷��� ���������� ��� �׸��鼭

// �ٵ�, ����, �÷��̾�� Ʈ���Ű��Ǹ�

// ���������鼭 �������, �÷��̾��� �����Ը� ������ �ø����.