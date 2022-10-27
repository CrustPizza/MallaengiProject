#include "pch.h"
#include "Camera.h"

Camera::Camera(GameObject* gameObject)
	: mGameObject(gameObject), 
	mSize(Graphics::GetInstance().GetScreenSize()),
	mLeftTop(Vector2D::Zero)
{
}

Camera::~Camera()
{

}

void Camera::UpdateCamera()
{
	// ȭ�� ������ �޾ƿ���
	mSize = Graphics::GetInstance().GetScreenSize();

	// ������Ʈ�� �߽ɿ� �ΰ� �����ӿ� ���� �ٴϴ� ī�޶�
	Transform* transform = mGameObject->GetTransform();

	// ī�޶��� pos�� ������ǥ�� �ֱ�? Ÿ�� �ٸ�
	Vector2D cameraPos = transform->GetPosition();

	// ī�޶��� �»�� -> ȭ�� ��ü�� �߾Ӱ� -> ĳ������ �߾����� ��ƾ� ��
	Vector2D leftTop = cameraPos - Vector2D(mSize.width / 2, mSize.height / 2);

	// ī�޶��� �»�ܿ� �� ����
	mLeftTop = leftTop;
}