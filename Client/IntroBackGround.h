#pragma once


/**
 * ������������ ��ũ�� �ӵ�, ��ũ�� ���� �ٸ� ������ ������ ..
 *
 * 2022. 02. 16. Comet
 */
class IntroBackGround : public GameObject
{
public:
	IntroBackGround();
	virtual ~IntroBackGround();

public:
	virtual void LateUpdate() override;
};