#pragma once
#include "CheckPoint.h"
#include "ItemBox.h"

#define CREATE_TIME 3.f
#define MOVE_DELTA 1.5f


/**
 * \brief : �Ӹ��� �ؿ��� ������ ������� ������ Ƣ���. (������ ����)
 */
 class SavePointBoxTrap : public ItemBox
{
 public:
	 // ų�� ������ ������ �� ���̴�.
	 SavePointBoxTrap(GameObject* object);
	 virtual ~SavePointBoxTrap();

	 bool mTrigger;

	// ���� �ð��� ������ ���̺� ����Ʈ�� �����ϰڽ��ϴ�.
	 float mTimer;

	 float mMoveDist;

	 bool mIsCreated;

	 bool mIsDone;

	 CheckPoint* mCheckPoint;

 public:
	virtual void TriggerEvent() override;

	virtual void Update() override;

	virtual void Release() override;
};