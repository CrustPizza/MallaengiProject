#pragma once

/**
 * Intro / Loading Scene���� ���� ī�޶��Դϴ�. (���߾� ����)
 *
 * 2022. 02. 12. Comet
 */
class SubCamera : public GameObject
{
public:
	SubCamera();
	virtual ~SubCamera();

public:
	virtual void LateUpdate() override;
};