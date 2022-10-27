#pragma once
#include "EnemyBase.h"
#include "random"

#define ENEMY_MOVE_SPEED 150.f

/// <summary>
/// ������ ���� FSM, �÷��̾� �߿� �Ӹ��� ������ ����.
///
///	2022. 02. 20. Comet
/// </summary>
class FreeEnemy : public EnemyBase
{
public:
	FreeEnemy(std::string type);
	FreeEnemy(std::string type,bool IsUpper);
	FreeEnemy(std::string type, int IsFalling, float MoveSpeed = 150.f);
	virtual ~FreeEnemy();

	// �ܹ��ų� ������̳�
	std::string mType;

	// -1 : ���� / +1 : ������
	int mDirrection;

	// -1 : falling
	int mIsFalling;

	// Idle ������ �� �������� �ð�
	float mPauseTime;

	// Run ������ �� ������ �Ÿ�
	float mDistance;

	// ó������ �� ���¿� ���Դ°�.
	bool mIsFirstLoop;

	bool mIsUpper;

	// FAlling, Upper
	float mMoveSpeed;

	std::mt19937 mMt;

	std::uniform_int_distribution<int> mPauseDist;

	std::uniform_int_distribution<int> mDistanceDist;

private:
	virtual void LoadAnimationProcess() override;

	virtual void Idle() override;

	virtual void Run() override;

	virtual void Die() override;

public:
	std::string GetType() { return mType; }
};