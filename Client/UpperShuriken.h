#pragma once

/**
 * \brief �����ϴٰ� �÷��̾ ���� ���� �������� ���ư� ~
 */
class UpperShuriken : public GameObject
{
public:
	UpperShuriken();
	virtual ~UpperShuriken();

private:
	Player* mGamePlayer;

	bool mTrigger;

public:
	virtual void Update() override;
};