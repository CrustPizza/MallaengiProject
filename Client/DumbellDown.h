#pragma once


class FreeEnemy;
/**
 * \brief : �����ٿ� ����. �÷��̾ x��ǥ ���� ��ó�� ���� ������ ����߸���.
 */
class DumbellDown : public GameObject
{
public:
	DumbellDown(std::string type);

	virtual ~DumbellDown();

	bool mTrigger;

	Player* mGamePlayer;

	// �� �༮�� ���������� ~
	FreeEnemy* mReserve;

public:
	virtual void Update();
	virtual void Release();
};