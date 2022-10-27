#pragma once
#include "FreeEnemy.h"

/**
 * \brief : �׳� �ؿ� �ִٰ� �÷��̾ �ֺ��� ���� Ƣ��ͼ� �������� ���� �޷����� �༮
 *
 * �÷��̾ �ֺ��� �ִ��� üŷ�ϴٰ�, �ֺ��� ���� �»������ �����ϴ� ���� ..?
 */
class UpperEnemy : public GameObject
{
public:
	UpperEnemy(std::string type);
	virtual ~UpperEnemy();

	Player* mGamePlayer;

	bool mTrigger;

	static float mTime;

	float mMyTime;

	std::string mType;
	
public:
	virtual void Update();
};