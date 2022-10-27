#include "PreCompiledHeader.h"
#include "StartButton.h"

#include "FadeOutObject.h"

StartButton::StartButton() :
mFadeOut(nullptr)
{
	// ��ŸƮ ��ư �̹��� ������ ��¥ ��ư�� �����ϰ� �ִ� ������ ����
	mSize = { 429.f, 82.f };

	mNormalSprite = Game::GetInstance().GetResource()->GetSprite("INTRO_START");
	mOverSprite = Game::GetInstance().GetResource()->GetSprite("INTRO_START2");
}

StartButton::~StartButton()
{
	
}

void StartButton::FixedUpdate()
{
	if (mIsActivated != true)
		return;

	// ��ư�� ���ȴٸ�
	if (mIsClicked)
	{
		if ((mFadeOut == nullptr) || (mFadeOut->GetSpriteRenderer()->GetOpacity() >= 1.0f))
		{
			
		}

		return;
	}

	if (IsCursorOnButton())
	{
		GetSpriteRenderer()->SetSprite(mOverSprite);
	}
	else
		GetSpriteRenderer()->SetSprite(mNormalSprite);
}

// ��ư�� Ŭ���Ǿ��� �� �ε� !
void StartButton::OnButtonClicked()
{
	if (mIsActivated != true)
		return;

	ButtonBase::OnButtonClicked();

	if (Game::GetInstance().GetIsOptionOn())
		return;

	mIsClicked = true;

	FadeOutObject* fadeOut = new FadeOutObject("StageOneOpeningLoadingScene");
	//FadeOutObject* fadeOut = new FadeOutObject("OneEndingLoadingScene");

	mFadeOut = fadeOut;

	Game::GetInstance().GetCurrentScene()->AddGameObject(fadeOut);
}