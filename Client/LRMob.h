#pragma once
#include "EnemyBase.h"
#include "OneWayMob.h"

/// <summary>
/// �������� ���ٰ� �� ������ ���������� �̵�
/// 
/// 2022. 02. 24. Comet
/// </summary>
class LRMob : public OneWayMob
{
public :
	LRMob(std::string type, float moveSpeed);

	virtual ~LRMob();

public:
	virtual void Update() override;
};