#pragma once
#include "TriggerTrap.h"

/**
 * �������� Ʈ���� Ʈ��. Falling�̴�. ��Ʈ������ ����.
 */
class FallingTriggerTrap : public TriggerTrap
{
public:
	FallingTriggerTrap(float speed);
	
	virtual ~FallingTriggerTrap();

	Player* mGamePlayer;

	bool mTrigger;

	float mSpeed;

public:
	virtual void Update() override;
};