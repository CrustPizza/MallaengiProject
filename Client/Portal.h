#pragma once

/// <summary>
/// ���� ���������� �̵��� ��Ű�� ��Ż
/// 
/// Next Stage�ε� ������ �̵��ϸ� �ɵ� ? (����Ƽ��)
/// 
/// 2022. 02. 22. Comet
/// </summary>
class Portal : public GameObject
{
public:
	Portal();
	virtual ~Portal();

	bool mTrigger;

	float mTimer;

	bool mIsDone;

	Player* mGamePlayer;

public:
	virtual void Update() override;
};