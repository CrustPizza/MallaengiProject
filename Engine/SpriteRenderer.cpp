#include "pch.h"
#include "SpriteRenderer.h"

SpriteRenderer::SpriteRenderer(GameObject* gameObject)
	: Renderer(gameObject),										// �θ� Ŭ������ ������ ���� ȣ��
	mOpacity(1.f),
	mSprite(nullptr)
{

}

SpriteRenderer::~SpriteRenderer()
{
	//if (mSprite != nullptr)
	//	delete mSprite;
}

void SpriteRenderer::SetSprite(Sprite* sprite)
{
	//if (mSprite != nullptr && mSprite->mSpriteSheetIndex > 10000)
	//{
	//	delete mSprite;
	//}

	mSprite = sprite;
}

void SpriteRenderer::Render()
{
	if (mSprite == nullptr)
		return;

	// Tag�� UI�̸� Ʈ������ �ڱⲨ�� (ȭ�� �� ��ġ�Ǿ� �ִ� ������) �ϸ� ��
	if (mGameObject->GetTag() == "UI")
		Graphics::GetInstance().SetTransformForUI(mGameObject->GetTransform()->GetWorldTransformMatrix());
	else
		// ����ϰ� ���� ��������Ʈ�� ��� ��ġ�� �����Ѵ�
		Graphics::GetInstance().SetTransform(mGameObject->GetTransform()->GetWorldTransformMatrix());

	// ���� ���õ� ��������Ʈ�� ����Ѵ�
	Graphics::GetInstance().DrawSprite(mSprite, Vector2D::Zero, mOpacity);
}

void SpriteRenderer::Release()
{
	if (mSprite != nullptr && mSprite->mSpriteSheetIndex > 10000)
		delete mSprite;
}