#include "PreCompiledHeader.h"
#include "UDMovingPlatform.h"

#include "Platform.h"
#include "BlockBase.h"

UDMovingPlatform::UDMovingPlatform(Vector2D startPos, int moveTile, int column, float idleTime, std::string type):
	mPlatforms(std::vector<GameObject*>(column)),
	mMoveTime(0.f),
	mDirControl(-1),
	mMoveTile(moveTile),
	mIdleTime(idleTime),
	mColumn(column),
	mType(type)
{
	AddSpriteRenderer()->SetSortOrder(5);

	for (int i = 0; i < mColumn; i++)
	{
		if (mType == "Block")
		{
			mPlatforms[i] = new BlockBase();

			Transform* transform = mPlatforms[i]->GetTransform();

			transform->SetPosition(startPos + Vector2D(TILE_WIDTH * i, 0.f));

			Game::GetInstance().GetCurrentScene()->AddGameObject(mPlatforms[i]);
		}
		else if (mType == "Platform")
		{
			mPlatforms[i] = new Platform();

			Transform* transform = mPlatforms[i]->GetTransform();

			transform->SetPosition(startPos + Vector2D(TILE_WIDTH * i, 0.f));

			Game::GetInstance().GetCurrentScene()->AddGameObject(mPlatforms[i]);
		}
	}
}

UDMovingPlatform::~UDMovingPlatform()
{

}

void UDMovingPlatform::FixedUpdate()
{

}

void UDMovingPlatform::Update()
{
	float delta = Timer::GetInstance().GetDeltaTime() / 1000.f;

	mIdleTime -= delta;

	if (mIdleTime >= 0)
		return;

	mMoveTime += delta;

	if (UD_MOVING_SPEED * mMoveTime >= TILE_HEIGHT * mMoveTile)
	{
		mMoveTime = 0;
		mDirControl *= -1;
	}

	// ��� �÷������� �̸� ������ �ʴ� �̵��ӵ��� �°� �̵������ݴϴ�.
	for (GameObject* platform : mPlatforms)
	{
		Transform* transform = platform->GetTransform();

		transform->SetPosition(transform->GetPosition() + delta * mDirControl * Vector2D(0.f, UD_MOVING_SPEED));
	}

	mTransform->SetPosition(mTransform->GetPosition() + delta * mDirControl * Vector2D(0.f, UD_MOVING_SPEED));

	// ����, �÷��� ��, �÷��̾�� ��� �浹 ���� �༮�� �ִٸ�, �÷��̾��� ��ġ�� �Ű��ݴϴ�.
	for (GameObject* platform : mPlatforms)
	{
		std::vector<GameObject*> info = platform->GetCollision()->GetCollisionInfo();

		bool isChecked = false;

		for (GameObject* object : info)
		{
			// �÷��̾�� �浹�� �����̶� �־��ٸ�
			if (mType == "Block")
			{
				BlockBase* block = dynamic_cast<BlockBase*>(platform);

				if ((object->GetTag() == "Player") && (block->GetCollisionDir() == CollisionDir::UP))
				{
					object->GetTransform()->SetPosition(object->GetTransform()->GetPosition() + delta * mDirControl * Vector2D(0.f, UD_MOVING_SPEED));

					isChecked = true;

					break;
				}
			}
			else if (mType == "Platform")
			{
				Platform* block = dynamic_cast<Platform*>(platform);

				if ((object->GetTag() == "Player") && (block->GetCollisionDir() == CollisionDir::UP))
				{
					object->GetTransform()->SetPosition(object->GetTransform()->GetPosition() + delta * mDirControl * Vector2D(0.f, UD_MOVING_SPEED));

					isChecked = true;

					break;
				}
			}
		}

		if (isChecked)
			break;
	}
}