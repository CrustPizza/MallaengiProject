#pragma once
#include "BlockBase.h"

/// <summary>
/// ���� �ı��Ǿ��� �� ����Ʈ�� ���� ��ü
/// 
/// 2022. 02. 21
/// </summary>
class BlockFragment : public GameObject
{
	Vector2D		mDirection;		// �̵� ����
	float			mEffectSpeed;	// �̵� �ӵ�
	float			mDuration;		// ��� �ð�
	float			mGravity;		// �߷�

public:
	BlockFragment(Vector2D pos);
	virtual ~BlockFragment();

	virtual void Update() override;

	void SetDirection(Vector2D direction);
};

/// <summary>
/// �μ� �� �ִ� ���
/// 
/// �÷��̾ �ؿ��� ���� �̵��ϴ� �̵� ��� �� (== ����) �浹�� �������� �� �ı��ȴ�.
/// 
/// 2022. 02. 08. Comet
/// </summary>
class BreakableBlock : public BlockBase
{
public:
	BreakableBlock();
	virtual ~BreakableBlock();

public:
	virtual void FixedUpdate() override;
	
	virtual void Update() override;

	void BreakBlock();
};