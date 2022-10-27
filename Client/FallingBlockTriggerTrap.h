#pragma once


class BlockBase;
/**
 * \brief : �� N���� �̷���� �ִµ� ����. �� ������ ���� �� ����.
 *
 * �÷��̾ �Ʒ��� ���� �¾� ����. 2022. 02. 21. Comet
 */
class FallingBlockTriggerTrap : public GameObject
{
public:
	FallingBlockTriggerTrap(Vector2D startPos, int column);
	virtual ~FallingBlockTriggerTrap();

	std::vector<BlockBase*> mBlocks;

	int mColumn;

	bool mTrigger;

	float mTimer;

	float mFallingSpeed;

	Player* mGamePlayer;
public:
	virtual void Update() override;
};