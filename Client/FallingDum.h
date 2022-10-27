#pragma once
#include "EnemyBase.h"
#include "FreeEnemy.h"

/**
 * \brief : �������� ����. EnemyBase�� ��ӹ����� ���� �ʴ� .. �׷� �� ..?
 *
 * 2022. 02. 23. Comet
 */
class FallingDum : public FreeEnemy
{
public:
	FallingDum(std::string type, int isFalling = -1);
	virtual ~FallingDum();

public:

	virtual void FixedUpdate() override;
	virtual void Update() override;
};