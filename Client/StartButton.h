#pragma once
#include "ButtonBase.h"

class FadeOutObject;

/**
 * Intro ȭ�鿡�� �� ��ư�� ������ ������ ���۵˴ϴ�.
 *
 * 2022. 02. 12. Comet
 */
class StartButton : public ButtonBase
{
public:
	StartButton();
	virtual ~StartButton();

private:
	// ��ư�� ������, Fade-In�� �ϸ鼭 ���� !
	FadeOutObject* mFadeOut;

	Sprite* mNormalSprite;

	Sprite* mOverSprite;

public:

	virtual void FixedUpdate() override;
	virtual void OnButtonClicked() override;
};