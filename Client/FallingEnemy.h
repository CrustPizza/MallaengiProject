#pragma once
#include "BlockPlatform.h"


/**
 * \brief : ������ �����Ͽ� ����, ������ ������ ���ʹ̸� �߶���Ų��.
 */
class FallingEnemy : public BlockPlatform
{
public:
	FallingEnemy();
	virtual ~FallingEnemy();

	bool mTrigger;

public:
	virtual void Update() override;
};