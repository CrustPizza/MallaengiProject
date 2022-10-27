#include "PreCompiledHeader.h"
#include "FallingBlock.h"

#include "BlockBase.h"
#include "Platform.h"
#include "Player.h"

// �߶��� ������ ��ӿ���� �������ϴ�.
#define MAX_FALLING_SPEED 3000.f
#define PLUS_SPEED_PER_FRAME 1000.f

FallingBlock::FallingBlock(Vector2D startPos, int columnCount) :
	GameObject(),
	mIsTriggered(false),
	mBlocks(std::vector<BlockBase*>(columnCount)),
	mGamePlayer(nullptr),
	mColumnCount(columnCount),
	mFallingSpeed(0.0f),
	mTimer(0.f)
{
	AddSpriteRenderer()->SetSortOrder(6);

	for (int i = 0; i < mColumnCount; i++)
	{
		mBlocks[i] = new BlockBase();

		Transform* transform = mBlocks[i]->GetTransform();

		transform->SetPosition(startPos + Vector2D(TILE_WIDTH * i, 0.f));

		Game::GetInstance().GetCurrentScene()->AddGameObject(mBlocks[i]);
	}

	mGamePlayer = Game::GetInstance().GetPlayer();
}

FallingBlock::~FallingBlock()
{

}

void FallingBlock::FixedUpdate()
{
	GameObject::FixedUpdate();
}

void FallingBlock::Update()
{

}

void FallingBlock::LateUpdate()
{
	GameObject::LateUpdate();

	if (mIsTriggered)
	{
		float deltaTime = Timer::GetInstance().GetDeltaTime() / 1000.f;
		mTimer += deltaTime;

		if (mTimer >= 10.f)
		{
			for (int i = 0; i < mColumnCount; i++)
			{
				Game::GetInstance().GetCurrentScene()->ReserveDestroy(mBlocks[i]);
			}

			Game::GetInstance().GetCurrentScene()->ReserveDestroy(this);

			return;
		}

		if (mFallingSpeed < MAX_FALLING_SPEED)
			mFallingSpeed += PLUS_SPEED_PER_FRAME * deltaTime;

		mTransform->SetPosition(mTransform->GetPosition() + mFallingSpeed * Vector2D::Down * deltaTime);

		for (int i = 0; i < mColumnCount; i++)
		{
			BlockBase* platform = mBlocks[i];

			platform->GetTransform()->SetPosition(platform->GetTransform()->GetPosition() + mFallingSpeed * Vector2D::Down * deltaTime);
		}

		// Player�� ���� �� ģ����� �浹 ���̸� ��ǥ�� �Ȱ��� �ٿ��ش�.
		for (int i = 0; i < mColumnCount; i++)
		{
			BlockBase* block = mBlocks[i];

			std::vector<GameObject*> info = block->GetCollision()->GetCollisionInfo();

			for (GameObject* object : info)
			{
				// �÷��̾�� ���� �浹 ���̶�� ..
				if (object->GetTag() == "Player")
				{
					//std::vector<GameObject*> info = mGamePlayer->GetCollision()->GetCollisionInfo();

					//for (GameObject* object2 : info)
					//{
					//	std::string tag = object2->GetTag();

					//	if (((tag == "BlockBase") || (tag == "Platform")) && (object2 != object))
					//	{
					//		isChecked = true;
					//	}
					//}

					/*if (!isChecked)
					{

					}*/

					mGamePlayer->GetTransform()->SetPosition(Vector2D(mGamePlayer->GetTransform()->GetPosition().mX, block->GetTransform()->GetPosition().mY - TILE_HEIGHT / 2 - (PLAYER_HEIGHT * PLAYER_SCALE / 2)));

					goto Escape;
				}
			}
		}

	Escape:;
	}
	else	// ��ҿ��� �浹 �˻縸 �ǽ��Ѵ�.
	{
		for (int i = 0; i < mColumnCount; i++)
		{
			BlockBase* block = mBlocks[i];

			std::vector<GameObject*> info = block->GetCollision()->GetCollisionInfo();

			for (GameObject* object : info)
			{
				// �÷��̾�� �÷����� ���ǿ��� �浹�� �����̶� �־��ٸ�
				if ((object->GetTag() == "Player") && (block->GetCollisionDir() == CollisionDir::UP))
					// Ʈ���Ÿ� ���ش�.
					mIsTriggered = true;
			}
		}
	}
}