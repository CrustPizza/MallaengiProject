#include "PreCompiledHeader.h"
#include "Title.h"

#include "FadeOutObject.h"

Title::Title()
{
	mSize = { 146.f, 45.f };

	GetSpriteRenderer()->SetSprite(Game::GetInstance().GetResource()->GetSprite("OPTION_TITLE"));
}

Title::~Title()
{
}

void Title::FixedUpdate()
{
	if (mIsActivated != true)
		return;

	ButtonBase::FixedUpdate();

	if (IsCursorOnButton())
		GetSpriteRenderer()->SetOpacity(0.5f);
	else
		GetSpriteRenderer()->SetOpacity(1.f);
}

void Title::OnButtonClicked()
{
	if (mIsActivated != true)
		return;

	ButtonBase::OnButtonClicked();

	mIsClicked = true;

	// ���̵� �ƿ��� �ϰ�
	FadeOutObject* fade = new FadeOutObject("IntroLoadingScene");

	Game::GetInstance().GetCurrentScene()->AddGameObject(fade);

	// ��� ������ �ʱ���·� ..! (FirstLoad ����)
	Game::GetInstance().InitScenes();

	Game::GetInstance().SetIsOptionClicked();
}