#include "pch.h"
#include "RectRenderer.h"

RectRenderer::RectRenderer(GameObject* object):
	Renderer(object),
	mSize({80.f, 77.f})
{

}

RectRenderer::~RectRenderer()
{

}

// ������ �� �༮�� ����׿��̱� ������ �̷��� �ۼ��մϴ�.
void RectRenderer::Render()
{
	Graphics::GetInstance().SetTransform(mGameObject->GetTransform()->GetWorldTransformMatrix());

	if ((mGameObject->GetCollision() != nullptr) && (mGameObject->GetCollision()->GetIsCollision()))
		Graphics::GetInstance().DrawRectangle(Vector2D::Zero, mSize, D2D1::ColorF(1.f, 0.f, 0.f, 1));
	else
		Graphics::GetInstance().DrawRectangle(Vector2D::Zero, mSize, D2D1::ColorF(0.f, 1.f, 0.f, 1));
}

void RectRenderer::Release()
{

}