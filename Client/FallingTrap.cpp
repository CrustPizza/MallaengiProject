#include "PreCompiledHeader.h"
#include "FallingTrap.h"

#include "Platform.h"

// �߶��� ������ ��ӿ���� �������ϴ�.
#define MAX_FALLING_SPEED 6.f
#define PLUS_SPEED_PER_FRAME 0.18f
#define LR_COLUMN_COUNT 4

FallingTrap::FallingTrap(Vector2D startPos) :
	GameObject(),
	mIsTriggered(false),
	mPlatforms(std::vector<Platform*>(LR_COLUMN_COUNT)),
	mPlayer(nullptr)
{
	for (int i = 0; i < LR_COLUMN_COUNT; i++)
	{
		mPlatforms[i] = new Platform();

		Transform* transform = mPlatforms[i]->GetTransform();

		transform->SetPosition(startPos + Vector2D(TILE_WIDTH * i, 0.f));
	}
}

FallingTrap::~FallingTrap()
{
	
}

void FallingTrap::FixedUpdate()
{
	GameObject::FixedUpdate();

}

void FallingTrap::Update()
{
	GameObject::Update();

	if (mIsTriggered)
	{
		if (mFallingSpeed < MAX_FALLING_SPEED)
			mFallingSpeed += PLUS_SPEED_PER_FRAME;

		for (int i = 0; i < LR_COLUMN_COUNT; i++)
		{
			Platform* platform = mPlatforms[i];

			platform->GetTransform()->SetPosition(platform->GetTransform()->GetPosition() + mFallingSpeed * Vector2D::Down);
		}

		// Player�� ���� �� ģ����� �浹 ���̸� ��ǥ�� �Ȱ��� �ٿ��ش�.
		for (int i = 0; i < LR_COLUMN_COUNT; i++)
		{
			Platform* platform = mPlatforms[i];

			std::vector<GameObject*> info = platform->GetCollision()->GetCollisionInfo();

			bool isChecked = false;

			for (GameObject* object : info)
			{
				// �÷��̾�� ���� �浹 ���̶�� ..
				if (object->GetTag() == "Player")
				{
					mPlayer->GetTransform()->SetPosition(mPlayer->GetTransform()->GetPosition() + mFallingSpeed * Vector2D::Down);

					isChecked = true;

					break;
				}
			}

			if (isChecked)
				break;
		}
	}
	else
	{
		for (int i = 0; i < LR_COLUMN_COUNT; i++)
		{
			Platform* platform = mPlatforms[i];

			std::vector<GameObject*> info = platform->GetCollision()->GetCollisionInfo();

			for (GameObject* object : info)
			{
				// �÷��̾�� �÷����� ���ǿ��� �浹�� �����̶� �־��ٸ�
				if ((object->GetTag() == "Player") && (platform->GetCollisionDir() == CollisionDir::UP))
				{
					// Ʈ���Ÿ� ���ش�.
					mIsTriggered = true;

					mPlayer = object;
				}
			}
		}
	}
}