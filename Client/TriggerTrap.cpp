#include "PreCompiledHeader.h"
#include "TriggerTrap.h"

#include "Player.h"

TriggerTrap::TriggerTrap() :
	mNextSprite(nullptr),
	mIsUp(false)
{
	AddAABBCollision();

	D2D1_SIZE_F size = GetAABBCollision()->GetSize();

	size.width -= 10;
	size.height -= 10;

	GetAABBCollision()->SetSize(size);

	// ���߿��� SpriteRenderer�� ����
	AddSpriteRenderer();
}

TriggerTrap::TriggerTrap(Sprite* nextSprite) :
	mIsUp(false),
	mNextSprite(nullptr)
{
	AddAABBCollision();

	D2D1_SIZE_F size = GetAABBCollision()->GetSize();

	size.width -= 10;
	size.height -= 10;

	GetAABBCollision()->SetSize(size);

	AddSpriteRenderer();

	mNextSprite = nextSprite;
}

TriggerTrap::TriggerTrap(bool IsUp) :
	mIsUp(IsUp),
	mNextSprite(nullptr)
{
	AddAABBCollision();

	AddSpriteRenderer()->SetSortOrder(7);
}

TriggerTrap::~TriggerTrap()
{
}

void TriggerTrap::FixedUpdate()
{
	GameObject::FixedUpdate();
}

void TriggerTrap::Update()
{
	std::vector<GameObject*> collisionInfo = GetCollision()->GetCollisionInfo();

	for (GameObject* object : collisionInfo)
	{
		// Player�� �浹�ϸ� Player�� HP�� 0���� ����ϴ�.
		if ((object->GetTag() == "Player"))
		{
			Game::GetInstance().GetPlayer()->SetHp(0);

			if (mNextSprite != nullptr)
			{
				Sprite* temp = GetSpriteRenderer()->GetSprite();
				GetSpriteRenderer()->SetSprite(mNextSprite);
				delete temp;
				mNextSprite = nullptr;
			}

			if (mIsUp)
				mTransform->SetPosition(mTransform->GetPosition() + 27.f * Vector2D::Up);
		}
	}
}

void TriggerTrap::Release()
{
	if (mNextSprite != nullptr)
		delete mNextSprite;
}