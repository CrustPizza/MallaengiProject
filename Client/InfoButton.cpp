#include "PreCompiledHeader.h"
#include "InfoButton.h"

InfoButton::InfoButton()
{
	// ���� ��ư �̹��� ������ ��¥ ��ư�� �����ϰ� �ִ� ������ ����
	mSize = { 70.f, 70.f };
}

InfoButton::~InfoButton()
{
}

void InfoButton::FixedUpdate()
{
	if (mIsActivated != true)
		return;

	if (IsCursorOnButton())
	{
		// �̹��� �ٲ�� ~
		// GetSpriteRenderer()->SetSprite();

		// �ϴ� ���� ����
		GetSpriteRenderer()->SetOpacity(0.5f);
	}
	else
		GetSpriteRenderer()->SetOpacity(1.f);
}

void InfoButton::OnButtonClicked()
{
	if (mIsActivated != true)
		return;

	ButtonBase::OnButtonClicked();

	// �ƹ��͵� ���Ѵ�.
	Game::GetInstance().OptionToggle();
}