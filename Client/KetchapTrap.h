#pragma once

/// <summary>
/// 2022. 02. 21. MG
/// 
/// ���� ���ع� �����
/// </summary>
class KetchapTrap : public GameObject
{
public:
	KetchapTrap();
	virtual ~KetchapTrap();

private:
	// ���� �÷��̾�
	Player* mGamePlayer;

	bool mTrigger;

	// �ʴ� ��� ������ �Ÿ��� �̵��ϴ°�
	Vector2D mVelocity;

	// ���� ������ ������ ������
	float mWeight;

	//	�� �ð��� ������, ������ ��ġ���� ������ �����س���.
	float mCreateTime;

	float mTimer;
	
	// �߷�
	float mGravity;

public:
	virtual void Update() override;
};