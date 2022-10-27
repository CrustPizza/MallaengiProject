#pragma once

class Player;

/**
 * \brief ����ũ Ʈ���� ���� .. ������ ������Ű�� ������
 *
 * // �÷��̾� �Ҹ��Ű�� �������� ����. 3������ ��, SetOpacity �ϰ� -> SetHp(0)
 *
 * // 2022. 02. 21. Comet
 */
class FakeTriggerBuilding : public GameObject
{
public:
	FakeTriggerBuilding();
	virtual ~FakeTriggerBuilding();

	Player* mGamePlayer;

	bool mTrigger;

	float mTimer;

	bool mLastLoop;
public:
	virtual void Update() override;
};