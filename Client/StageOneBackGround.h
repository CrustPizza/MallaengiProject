#pragma once

/**
 * �� ������������ ���ȭ���� ����ϴµ� ����� Object
 *
 * 2022. 02. 15. Comet. �׳� �������.
 */
class StageOneBackGround : public GameObject
{
public:
	StageOneBackGround();
	virtual ~StageOneBackGround();

public:
	virtual void LateUpdate() override;
};