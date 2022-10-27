#pragma once

/**
 * ���� Load�ϰ� Build�� �� Fade-In�� ����մϴ�.
 *
 * 2022. 02. 16. Comet
 */
class FadeInObject : public GameObject
{
public:
	FadeInObject();
	virtual ~FadeInObject();

private:
	// ���̵� �ð�
	float mFadeTime;

public:
	virtual void Update() override;
};