#include "pch.h"
#include "Renderer.h"

Renderer::Renderer(GameObject* object):
	mGameObject(object),
	// 1 : �÷��̾� / 2 : ���� / 3: ������ / 4 : ��, �÷��� / 5 ~ : ��� �� ��Ÿ ..?
	mSortOrder(4) // ���� 1, 2, 3 / 5, 6, 7 .. �� �������ִ� ��찡 �ƴ϶�� 4���� �о�ֽ��ϴ�.
{

}

Renderer::~Renderer()
{

}

void Renderer::Render()
{
	Graphics::GetInstance().SetTransform(mGameObject->GetTransform()->GetWorldTransformMatrix());

	D2D1_SIZE_F size = { 100.f, 100.f };

	Graphics::GetInstance().DrawRectangle(Vector2D::Zero, size, D2D1::ColorF(1.f, 0.f, 0.f, 1));
}