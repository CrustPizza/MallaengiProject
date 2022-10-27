#include "PreCompiledHeader.h"
#include "FadeInObject.h"

#define FADE_IN_SECOND 1.5

FadeInObject::FadeInObject() :
mFadeTime(0.f)
{
	SetTag("UI");

	D2D1_SIZE_F size = Graphics::GetInstance().GetScreenSize();

	mTransform->SetPosition(Vector2D(size.width / 2, size.height / 2));

	AddSpriteRenderer()->SetSortOrder(1);

	GetSpriteRenderer()->SetSprite(Game::GetInstance().GetResource()->GetSprite("FADE_INOUT"));
}

FadeInObject::~FadeInObject()
{

}

void FadeInObject::Update()
{
	GameObject::Update();

	mFadeTime += Timer::GetInstance().GetDeltaTime() / 1000.f;

	// 2.5�� ���̵�
	if (GetSpriteRenderer()->GetOpacity() >= 0)
		GetSpriteRenderer()->SetOpacity(1.f - mFadeTime / FADE_IN_SECOND);
	else
	{
		// ������Ʈ�� ������ �Ϸ��Ͽ����Ƿ� ���� ����
		Game::GetInstance().GetCurrentScene()->ReserveDestroy(this);
	}
}