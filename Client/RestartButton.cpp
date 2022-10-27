#include "PreCompiledHeader.h"
#include "RestartButton.h"

RestartButton::RestartButton(std::string sceneName):
mSceneName(sceneName)
{
	mSize = { 38.f, 38.f };

	// ���� ��ư�̴�.
	mIsSquare = false;

	GetSpriteRenderer()->SetSprite(Game::GetInstance().GetResource()->GetSprite("DIE_RESTART"));
}

RestartButton::~RestartButton()
{
}

void RestartButton::FixedUpdate()
{
	if (mIsActivated != true)
		return;

	ButtonBase::FixedUpdate();

	if (IsCursorOnButton())
	{
		GetSpriteRenderer()->SetOpacity(0.5f);
	}
	else
		GetSpriteRenderer()->SetOpacity(1.f);

}

void RestartButton::OnButtonClicked()
{
	if (mIsActivated != true)
		return;

	ButtonBase::OnButtonClicked();

	Game::GetInstance().ReserveLoadScene(mSceneName);
	/*SoundManager::GetInstance()->Play();*/
}