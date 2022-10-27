#include "pch.h"
#include "CircleCollision.h"
#include "Transform.h"
#include "GameObject.h"

CircleCollision::CircleCollision(GameObject* object) :
	Collision(object)
{
	mRadius = 10.0f;
}

CircleCollision::~CircleCollision()
{

}

bool CircleCollision::CheckCollision(Collision* collision)
{
	// ����
	Transform* transform = this->GetGameObject()->GetTransform();

	// �ٸ� �༮
	Transform* otherTransform = collision->GetGameObject()->GetTransform();

	// � Collision ���� üũ
	CircleCollision* isCircle = dynamic_cast<CircleCollision*>(collision);

	// üũ�� Collision�� ���� ��!
	if (isCircle != nullptr)
	{
		// ���� ������+���ǹ������� < ������ �Ÿ����� ª�� ��
		if (this->GetRadius() + isCircle->GetRadius() < (((transform->GetPosition()) - (otherTransform->GetPosition())).GetMagnitude()))
		{
			// �浹 ����
			return false;
		}
		else
		{
			// �浹 ��
			this->SetIsCollision(true);
			isCircle->SetIsCollision(true);

			// �浹 �� ������Ʈ ������ �����.
			this->AddCollisionInfo(isCircle->GetGameObject());

			return true;
		}
	}
	return false;
}