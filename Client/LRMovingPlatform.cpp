#include "PreCompiledHeader.h"
#include "LRMovingPlatform.h"

// Platforms
#include "Platform.h"
#include "BlockBase.h"

LRMovingPlatform::LRMovingPlatform(Vector2D startPos, int moveTile, int column, float idleTime):
	mPlatforms(std::vector<BlockBase*>(column)),
	mMoveTime(0.f),
	mDirControl(1),
	mMoveTile(moveTile),
	mIdleTime(idleTime),
	mColumn(column)
{
	AddSpriteRenderer()->SetSortOrder(5);

	for (int i = 0; i < mColumn; i++)
	{
		mPlatforms[i] = new BlockBase();

		Transform* transform = mPlatforms[i]->GetTransform();

		transform->SetPosition(startPos + Vector2D(TILE_WIDTH * i, 0.f));

		Game::GetInstance().GetCurrentScene()->AddGameObject(mPlatforms[i]);
	}
}

LRMovingPlatform::~LRMovingPlatform()
{

}

void LRMovingPlatform::FixedUpdate()
{

}

void LRMovingPlatform::Update()
{
	float delta = Timer::GetInstance().GetDeltaTime() / 1000.f;

	mIdleTime -= delta;

	if (mIdleTime >= 0)
		return;

	mMoveTime += delta;

	if (LR_MOVING_SPEED * mMoveTime >= TILE_WIDTH * mMoveTile)
	{
		mMoveTime = 0;
		mDirControl *= -1;
	}

	// ��� �÷������� �̸� ������ �ʴ� �̵��ӵ��� �°� �̵������ݴϴ�.
	for (BlockBase* platform : mPlatforms)
	{
		Transform* transform = platform->GetTransform();

		transform->SetPosition(transform->GetPosition() + delta * mDirControl * Vector2D(LR_MOVING_SPEED, 0.f));
	}

	mTransform->SetPosition(mTransform->GetPosition() + delta * mDirControl * Vector2D(LR_MOVING_SPEED, 0.f));


	// ����, �÷��� ��, �÷��̾�� ��� �浹 ���� �༮�� �ִٸ�, �÷��̾��� ��ġ�� �Ű��ݴϴ�.
	for (BlockBase* platform : mPlatforms)
	{
		std::vector<GameObject*> info = platform->GetCollision()->GetCollisionInfo();

		bool isChecked = false;

		for (GameObject* object : info)
		{
			// �÷��̾�� �浹�� �����̶� �־��ٸ�
			if ((object->GetTag() == "Player") && (platform->GetCollisionDir() == CollisionDir::UP))
			{
				object->GetTransform()->SetPosition(object->GetTransform()->GetPosition() + delta * mDirControl * Vector2D(LR_MOVING_SPEED, 0.f));

				isChecked = true;

				break;
			}
		}

		if (isChecked)
			break;
	}
}