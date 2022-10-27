#pragma once

#include "BlockBase.h"
#include "FiveFalling.h"

/**
 * \brief : Ʈ���� ����. 3-5 ���������� ��� ������Ʈ �߶� �� �����ϴ�.
 *
 * 2022. 02. 25. Comet
 */
class TriggerDoor : public BlockBase
{
public:
	TriggerDoor();
	virtual ~TriggerDoor();

	float mTotalWait;

public:
	virtual void Update() override;
};