#pragma once
#include "Player.h"


#define ENEMY_WIDTH 90.f
#define ENEMY_HEIGHT 125.f

// Character Scale Control
#define ENEMY_SCALE 1.2f

#define BLOCKING_POWER 100.f

// Box
class ItemBox;
class BoxBase;
class TransBox;

// Blocks
class BreakableBlock;

// Platforms
class Platform;
class BlockPlatform;

// ������ ������ �ٸ� ���¸� ������. (3���� �� ������ �ٸ� ����)
// ������ ���� ����
enum class EnemyState
{
	Idle = 0,			// ���
	Run = 1,			// �̵�
	Die = 2,			// ���
	Attack = 3,			// ����
	Jump = 4,			// ����
};

/// <summary>
/// 2022. 02. 16. MG
/// 
/// ������ �� FSM �����
/// ������, ������, ������ ����
/// </summary>
class EnemyBase : public GameObject
{
public:
	EnemyBase();
	~EnemyBase();

protected:
	// �̵� ���� ���� (�̵��� ���ٸ� Zero / �̵��� �ִٸ� Normal Vector)
	Vector2D mDirVector;

	// Update �� ��ġ
	Vector2D mBeforePosition;

	// ������ �� �� �ִ� �����ΰ� ..?
	bool mIsJumpable;

protected:
	// ---------------------------- Enemy FSM ----------------------------
	// ���� State
	EnemyState mCurrentState;

	// �� �����ӿ����� State
	EnemyState mPrevState;

	std::vector<AnimationClip*> mAnimationClips;

	// State Ʈ�������� �Ͼ �� �ִϸ��̼��� �˸°� �������ְ� �ٲ��
	virtual void Idle();
	virtual void Run();
	virtual void Attack();
	virtual void Jump();
	virtual void Die();

	// Enemy�� ������ �� ���ο� �ش��ϴ� �ִϸ��̼��� ���� �ִ´�.
	virtual void LoadAnimationProcess();

	// ���� ���¸� üũ�Ͽ� ���������� ���� ��� �̿� ���õ� �̺�Ʈ�� �����մϴ�.
	void StateChangeProcess();

	// Hp
	int mHp;

public:
	EnemyState GetCurrentState() { return mCurrentState; }

public:
	int GetHp() { return mHp; }
	void SetHp(int value) { mHp = value; }

protected:
	// ---------------------------- Simple Physics ----------------------------
	// �ʴ� ��� ������ �Ÿ��� �̵��ϴ°�
	Vector2D mVelocity;

	// �߷�
	float mGravity;

	// ������
	float mJumpPower;

	// Walk or Dash�� �Ѵٴ� ���� �ӵ��� x-value�� ����ϸ� �Ǵ� �� ..!
	float mMaxWalkSpeed;
	// ---------------------------- Simple Physics ----------------------------

public:
	// ���� ���� ..
	virtual void FixedUpdate() override;

	virtual void Update() override;

	// �÷��̾��� �̵� ���� ���͸� ��ȯ�Ѵ�.
	Vector2D GetDirVector() { return mDirVector; }

	// Velocity Getter, Setter
	Vector2D GetVelocity() { return mVelocity; }
	void SetVelocity(Vector2D value) { mVelocity = value; }

	// Block Process
	void BlockCollisionProcess(BlockBase* block);
	void BreakableBlockCollisionProcess(BreakableBlock* breakableBlock);

	// Platform Process (��, ��, �Ϸδ� ���� ���� / �������� ����)
	void PlatformCollisionProcess(Platform* platform);
	void BlockPlatformCollisionProcess(BlockPlatform* blockplatform);

	// Box Process
	void BoxCollisionProcess(BoxBase* box);
	void TransBoxCollisionProcess(TransBox* TransBox);
	void ItemBoxCollisionProcess(ItemBox* itemBox);
};