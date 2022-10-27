#include "pch.h"
#include "AABBCollision.h"
#include "Transform.h"
#include "GameObject.h"


// Ÿ�� ũ�⿡ �°� ����� �ٲ� �ʿ䰡 �ֽ��ϴ�.
AABBCollision::AABBCollision(GameObject* object) :
	Collision(object)
{
	// Ÿ�� ũ�⿡ �°� ����ξ����ϴ�.
	mSize.width = 80.f;
	mSize.height = 77.f;
}

AABBCollision::~AABBCollision()
{

}

bool AABBCollision::CheckCollision(Collision* collision)
{
	// ����
	Transform* transform = this->GetGameObject()->GetTransform();

	Vector2D scale = transform->GetScale();

	// �ٸ� �༮
	Transform* otherTransform = collision->GetGameObject()->GetTransform();

	Vector2D otherScale = otherTransform->GetScale();

	// � Collision ���� üũ
	AABBCollision* isAABB = dynamic_cast<AABBCollision*>(collision);

	// ���� �ڽ��� üũ �� ��
	CircleCollision* isCircle = dynamic_cast<CircleCollision*>(collision);

	// �ڽ��� �ڽ��� �浹����
	if (isAABB != nullptr)
	{
		/// �ʿ� ���� ��ġ���� ������ �ݱ��̸� ���� ���� �� Ŭ�� �浹!
		if (abs(transform->GetPosition().GetX() - otherTransform->GetPosition().GetX()) <
			((this->mSize.width / 2) * abs(scale.mX)) + ((isAABB->mSize.width / 2) * abs(otherScale.mX)) &&
			abs(transform->GetPosition().GetY() - otherTransform->GetPosition().GetY()) <
			((this->mSize.height / 2) * abs(scale.mY)) + ((isAABB->mSize.height / 2) * abs(otherScale.mY)))
		{
			this->SetIsCollision(true);
			isAABB->SetIsCollision(true);

			AddCollisionInfo(isAABB->GetGameObject());				// ������Ʈ�� �浹 ���� ����
			isAABB->AddCollisionInfo(this->GetGameObject());

			return true;
		}
		else
			return false;
	}
	// ���� �ڽ��� �浹 ����
	else if (isCircle != nullptr)
	{
		// �ڽ�, �� �浹 �� ũ�Ⱑ �þ �簢���� ���� rect
		D2D_RECT_F rect = { transform->GetPosition().GetX() - (this->mSize.width / 2),
							transform->GetPosition().GetY() - (this->mSize.height / 2),
							transform->GetPosition().GetX() + (this->mSize.width / 2),
							transform->GetPosition().GetY() + (this->mSize.height / 2)
		};

		// ���� ��ǥ�� ������
		float centerX = static_cast<int>(isCircle->GetGameObject()->GetTransform()->GetPosition().GetX());
		float centerY = static_cast<int>(isCircle->GetGameObject()->GetTransform()->GetPosition().GetY());
		float radius = static_cast<int>(isCircle->GetRadius());

		// ���� ��ǥ�� �簢���� ���������� ��
		if ((rect.left <= centerX && centerX <= rect.right) || (rect.top <= centerY && centerY <= rect.bottom))
		{
			// �簢���� �� �� �� �Ϸ� ���� ��������ŭ ũ�⸦ �ø� �簢�� �ȿ� ���� �߽��� �����°�? -> �浹�̴�!
			if (
				((rect.left - isCircle->GetRadius()) <= centerX &&		// ��
				centerX <= (rect.right + isCircle->GetRadius())) &&		// ��
				((rect.top - isCircle->GetRadius()) <= centerY &&		// ��
				centerY <= (rect.bottom + isCircle->GetRadius()))		// ��
				)
			{
				this->SetIsCollision(true);
				isCircle->SetIsCollision(true);

				this->AddCollisionInfo(this->GetGameObject());
				isCircle->AddCollisionInfo(isCircle->GetGameObject());

				return true;
			}
		}

		// ���� �߽���ǥ
		Vector2D centerOfCircle = Vector2D(centerX, centerY);

		// �簢�� �𼭸� 4���� ���� �� �ȿ� �ִ��� üũ -> ���� ���������� �簢���� �� �������� �Ÿ��� ������? �浹�̴� !
		if ((Vector2D(rect.left, rect.top).DistanceTo(centerOfCircle) <= radius) ||
			(Vector2D(rect.right, rect.top).DistanceTo(centerOfCircle) <= radius) ||
			(Vector2D(rect.left, rect.bottom).DistanceTo(centerOfCircle) <= radius) ||
			(Vector2D(rect.right, rect.bottom).DistanceTo(centerOfCircle) <= radius))
		{
			this->SetIsCollision(true);
			isCircle->SetIsCollision(true);

			this->AddCollisionInfo(this->GetGameObject());		
			isCircle->AddCollisionInfo(isCircle->GetGameObject());

			return true;
		}
	}

	return false;
}