#pragma once

/**
 * ���������� �÷��̾��� ��ġ�� ��� �����ϴٰ� ������ x��ǥ �αٿ� ���� ����¡�Ѵ� ..! ������ �״´�.
 *
 * 2022. 02. 18. Comet
 */
class RisingTrap : public GameObject
{
public:
	RisingTrap();
	virtual ~RisingTrap();

private:
	Player* mGamePlayer;

	bool mTrigger;

public:
	virtual void Update() override;
};