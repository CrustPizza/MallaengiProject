#pragma once

#define WEIGHT_DELTA 10.f

/**
 * CheckPoint, ���⸦ ������ �÷��̾��� �����԰� 5 ����˴ϴ�.
 *
 * ���� �װ� �ȴٸ�, ���⼭ �ٽ� �һ��ؼ� �÷��̸� ��� �����մϴ�.
 *
 * 2022. 02. 15. Comet
 */
class CheckPoint : public GameObject
{
public:
	CheckPoint();
	virtual ~CheckPoint();

	bool mTrigger;

public:
	virtual void Update() override;
};