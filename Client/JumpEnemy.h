#pragma once
#include "FreeEnemy.h"

/**
 * \brief : �����ϸ鼭 �Դٰ��� �ϰ� �ش޷�
 *
 * 2022. 02. 22. Comet
 */
class JumpEnemy : public FreeEnemy
{
public:
	JumpEnemy();
	virtual ~JumpEnemy();

public:
	virtual void Update() override;
};