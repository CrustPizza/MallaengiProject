#pragma once
#include "BlockPlatform.h"

/**
 * \brief : ������ ������ ���� distance���� ���� �������� ���ƿ�
 *
 * 2022. 02. 22. Comet
 */
class ShurikenPlatform : public BlockPlatform
{
public:
	ShurikenPlatform();
	virtual ~ShurikenPlatform();

	bool mTrigger;

public:
	virtual void Update() override;
};