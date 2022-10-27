#pragma once
#include "BoxBase.h"

/// <summary>
/// �ؿ��� ġ�� �������� ������ �ڽ�
/// </summary>
class ItemBox : public BoxBase
{
public:
	ItemBox(GameObject* reserveObject, int stage);
	virtual ~ItemBox();

	// �� ģ���� Ʈ���Ű� �Ǿ��� �� ������� ������Ʈ�Դϴ�.
	GameObject* mReserveObject;

	int mStage;

public:

	virtual void TriggerEvent() override;
	virtual void Release() override;
};