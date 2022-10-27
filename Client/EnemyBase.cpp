#include "PreCompiledHeader.h"
#include "Player.h"


//Blocks
#include "BlockBase.h"
#include "BreakableBlock.h"
#include "TransBox.h"

// Platforms
#include "Platform.h"
#include "BlockPlatform.h"

// Enemy
#include "EnemyBase.h"

#include "ItemBox.h"

EnemyBase::EnemyBase() :
	mDirVector(Vector2D::Zero),
	mBeforePosition(Vector2D::Zero),
	mCurrentState(EnemyState::Idle),
	mPrevState(EnemyState::Idle),
	mHp(1),
	mVelocity(Vector2D::Zero),
	mGravity(GRAVITY),
	mAnimationClips(std::vector<AnimationClip*>())
{
	// ���� �߿��� �±� ����
	SetTag("Enemy");

	// ������ �� �� ������Ʈ�� �ʿ��� ������Ʈ�� �غ� ��� ���־�� �մϴ�.
	AddSpriteRenderer()->SetSortOrder(4); // �������� 1�� �� ���� !

	// �̰Ŵ� ������ Sprite Tool�� �ǰ� ������ ���� �����Դϴ�.
	AddAABBCollision()->SetSize({ ENEMY_WIDTH, ENEMY_HEIGHT });

	Animator* animator = AddAnimator();

	GetTransform()->SetScale(Vector2D(ENEMY_SCALE, ENEMY_SCALE));
}

EnemyBase::~EnemyBase()
{
}

void EnemyBase::Idle()
{
}

void EnemyBase::Run()
{
}

void EnemyBase::Attack()
{
}

void EnemyBase::Jump()
{
}

void EnemyBase::Die()
{
	
}

void EnemyBase::LoadAnimationProcess()
{
	// �������̵��ؼ� ������ ������ �´� �ִϸ��̼��� ��������.
}

void EnemyBase::StateChangeProcess()
{
	if (mPrevState != mCurrentState)
		GetAnimator()->SetAnimationClip(mAnimationClips[static_cast<int>(mCurrentState)]);
}

void EnemyBase::FixedUpdate()
{
	GameObject::FixedUpdate();

	// 0. ������Ʈ �� ���� ��ġ�� ����صӴϴ�.
	mBeforePosition = mTransform->GetPosition();

	// 1-1. �߷� ó�� (�ִ� �������� �ӵ��� ��Ʈ���մϴ�.)
	if (mVelocity.mY <= MAX_FALLING_SPEED)
	{
		mVelocity.mY = mVelocity.mY + GRAVITY * Timer::GetInstance().GetDeltaTime() / 1000.f;
	}

	// 1. Update ��, PrevState�� PrevStep�� ��ü�� �ǽ��մϴ�.
	mPrevState = mCurrentState;
	
	// Enemy State�� ���� �з�
	switch (mCurrentState)
	{
		case EnemyState::Idle:
		{
			Idle();
			break;
		}

		case EnemyState::Run:
		{
			Run();
			break;
		}

		case EnemyState::Attack:
		{
			Attack();
			break;
		}

		case EnemyState::Jump:
		{
			Jump();
			break;
		}

		case EnemyState::Die:
		{
			Die();
			break;
		}

		default:
			break;
	}

	mTransform->SetPosition(mTransform->GetPosition() + (Timer::GetInstance().GetDeltaTime() / 1000.f) * mVelocity);

	mVelocity.mX = 0.f;

	mIsJumpable = false;
}

void EnemyBase::Update()
{
	GameObject::Update();

	// 1. �̹� �����ӿ����� �浹�� Ȯ����
	// �÷����� ���� ������ �����Ѵ�. �� ������Ʈ ���� ������ �ٸ� ..!
	std::vector<GameObject*> collisionInfo = mCollision->GetCollisionInfo();

	for (GameObject* object : collisionInfo)
	{
		// BreakableBlock�� �浹�� ���
		if (object->GetTag() == "BreakableBlock")
		{
			BreakableBlock* breakableBlock = dynamic_cast<BreakableBlock*>(object);

			BreakableBlockCollisionProcess(breakableBlock);
		}
		else if (object->GetTag() == "BlockBase")
		{
			BlockBase* block = dynamic_cast<BlockBase*>(object);

			BlockCollisionProcess(block);
		}

		// Platform
		else if (object->GetTag() == "Platform")
		{
			Platform* platform = dynamic_cast<Platform*>(object);

			PlatformCollisionProcess(platform);
		}
		else if (object->GetTag() == "BlockPlatform")
		{
			BlockPlatform* platform = dynamic_cast<BlockPlatform*>(object);

			BlockPlatformCollisionProcess(platform);
		}

		// Box
		else if (object->GetTag() == "BoxBase")
		{
			BoxBase* box = dynamic_cast<BoxBase*>(object);

			BoxCollisionProcess(box);
		}
		// TransBox�� �浹�� ���
		else if (object->GetTag() == "TransBox")
		{
			TransBox* transBox = dynamic_cast<TransBox*>(object);

			TransBoxCollisionProcess(transBox);
		}
		// ItemBox
		else if (object->GetTag() == "ItemBox")
		{
			ItemBox* itemBox = dynamic_cast<ItemBox*>(object);

			ItemBoxCollisionProcess(itemBox);
		}
	}

	if (mHp <= 0)
		mCurrentState = EnemyState::Die;

	StateChangeProcess();
}


void EnemyBase::BlockCollisionProcess(BlockBase* block)
{
	if (block != nullptr)
	{
		Transform* blockBaseTransform = block->GetTransform();
		// ���� ��ġ
		Vector2D blockBasePos = blockBaseTransform->GetPosition();
		// ���� ������
		D2D1_SIZE_F blockBaseSize = block->GetSize();
		// Player�� ������
		D2D1_SIZE_F playerSize = dynamic_cast<AABBCollision*>(this->mCollision)->GetSize();

		// -- ��� ���⿡�� �÷��̾�� �浹�ߴ°� + ��� ������ �浹���� �˻��Ѵ�.
		float vertTovertTangent = ((blockBaseSize.height / 2) + (playerSize.height * PLAYER_SCALE / 2)) / ((blockBaseSize.width / 2) + (playerSize.width * PLAYER_SCALE / 2));

		Vector2D centerTocenter = this->mTransform->GetPosition() - blockBasePos;

		float curTangent = centerTocenter.mY / centerTocenter.mX;

		Vector2D velo = this->GetVelocity();

		// �÷��̾�� ���� ���鿡 �ִ�.
		// ���⼭ �� ����ؾ��� ���� �ִٸ�, �÷��̾ ��� �߿��� ��ġ ���� X ..!
		// ����, ���� �鿡�� ���� �� .. ���߰��ϰ� ���� �� ������ �� �� �ִ�.
		if ((centerTocenter.mY < -(TILE_HEIGHT / 2) - (PLAYER_HEIGHT * PLAYER_SCALE / 2) + ADJUST_Y) && ((curTangent >= vertTovertTangent) || (curTangent <= -vertTovertTangent)))
		{
			// �߶� �� �Ǵ� �÷��� ���� �ִ�.
			if (mDirVector.mY >= 0.f)
			{
				// �÷��̾ ��� �ִ� �ظ��� �ϳ��� �ִٴ� ���̹Ƿ� ���� �����ϰ� �ٲ��ݴϴ�.
				mIsJumpable = true;

				velo.mY = 0.f;

				// �÷��̾� ĳ������ ��ġ�� ���� ������� �������ش�.
				mTransform->SetPosition(Vector2D(mTransform->GetPosition().mX,
					blockBasePos.mY + 0.005 - (blockBaseSize.height / 2) - (playerSize.height * PLAYER_SCALE / 2)));
			}
		}
		// �÷��̾�� ���� �ظ鿡 �ִ�.
		else if ((centerTocenter.mY >= 0) && ((curTangent >= vertTovertTangent) || (curTangent <= -vertTovertTangent)))
		{
			// ��� �� �÷����� �ظ鿡�� �浹�Ͽ��ٸ�.
			if (mDirVector.mY < 0.f)
			{
				velo.mY = 0.f;

				// �÷��̾� ĳ������ ��ġ�� ���� �ϴ����� �������ش�.
				mTransform->SetPosition(Vector2D(mTransform->GetPosition().mX,
					blockBasePos.mY + (blockBaseSize.height / 2) + (playerSize.height * PLAYER_SCALE / 2)));
			}
		}
		// �÷��̾�� ���� ������ �鿡 �ִ�.
		else if ((centerTocenter.mX >= 0) && ((curTangent < vertTovertTangent) || (curTangent > -vertTovertTangent)))
		{
			// velo.mX = 0.f;

			// �÷��̾� ĳ������ ��ġ�� ���� �������� �������ش�.
			mTransform->SetPosition(Vector2D(blockBasePos.mX + (blockBaseSize.width / 2) + (playerSize.width * PLAYER_SCALE / 2),
				mTransform->GetPosition().mY));
		}
		// �÷��̾�� ���� ���� �鿡 �ִ�.
		else if ((centerTocenter.mX < 0) && ((curTangent < vertTovertTangent) || (curTangent > -vertTovertTangent)))
		{
			// velo.mX = 0.f;

			// �÷��̾� ĳ������ ��ġ�� ���� �������� �������ش�.
			mTransform->SetPosition(Vector2D(blockBasePos.mX - (blockBaseSize.width / 2) - (playerSize.width * PLAYER_SCALE / 2),
				mTransform->GetPosition().mY));
		}

		// ��ȯ�� �ӵ��� �������ش�.
		this->SetVelocity(velo);
	}
}

void EnemyBase::BreakableBlockCollisionProcess(BreakableBlock* breakableBlock)
{
	if (breakableBlock != nullptr)
	{
		Transform* blockBaseTransform = breakableBlock->GetTransform();
		// ���� ��ġ
		Vector2D blockBasePos = blockBaseTransform->GetPosition();
		// ���� ������
		D2D1_SIZE_F blockBaseSize = breakableBlock->GetSize();
		// Player�� ������
		D2D1_SIZE_F playerSize = dynamic_cast<AABBCollision*>(this->mCollision)->GetSize();

		// -- ��� ���⿡�� �÷��̾�� �浹�ߴ°� + ��� ������ �浹���� �˻��Ѵ�.
		float vertTovertTangent = ((blockBaseSize.height / 2) + (playerSize.height * ENEMY_SCALE / 2)) / ((blockBaseSize.width / 2) + (playerSize.width * ENEMY_SCALE / 2));

		Vector2D centerTocenter = this->mTransform->GetPosition() - blockBasePos;

		float curTangent = centerTocenter.mY / centerTocenter.mX;

		Vector2D velo = this->GetVelocity();

		// �÷��̾�� ���� ���鿡 �ִ�.
		// ���⼭ �� ����ؾ��� ���� �ִٸ�, �÷��̾ ��� �߿��� ��ġ ���� X ..!
		// ����, ���� �鿡�� ���� �� .. ���߰��ϰ� ���� �� ������ �� �� �ִ�.
		if ((centerTocenter.mY < -(TILE_HEIGHT / 2) - (ENEMY_SCALE * ENEMY_HEIGHT / 2) + ADJUST_Y) && ((curTangent >= vertTovertTangent) || (curTangent <= -vertTovertTangent)))
		{
			// �߶� �� �Ǵ� �÷��� ���� �ִ�.
			if (mDirVector.mY >= 0.f)
			{
				// �÷��̾ ��� �ִ� �ظ��� �ϳ��� �ִٴ� ���̹Ƿ� ���� �����ϰ� �ٲ��ݴϴ�.
				mIsJumpable = true;

				velo.mY = 0.f;

				// �÷��̾� ĳ������ ��ġ�� ���� ������� �������ش�.
				mTransform->SetPosition(Vector2D(mTransform->GetPosition().mX,
					blockBasePos.mY + 0.005 - (blockBaseSize.height / 2) - (playerSize.height * ENEMY_SCALE / 2)));
			}
		}
		// �÷��̾�� ���� �ظ鿡 �ִ�.
		else if ((centerTocenter.mY >= 0) && ((curTangent >= vertTovertTangent) || (curTangent <= -vertTovertTangent)))
		{
			// ��� �� �÷����� �ظ鿡�� �浹�Ͽ��ٸ�.
			if (mDirVector.mY < 0.f)
			{
				velo.mY = 0.f;

				// �÷��̾� ĳ������ ��ġ�� ���� �ϴ����� �������ش�.
				mTransform->SetPosition(Vector2D(mTransform->GetPosition().mX,
					blockBasePos.mY + (blockBaseSize.height / 2) + (playerSize.height * ENEMY_SCALE / 2)));
			}
		}
		// �÷��̾�� ���� ������ �鿡 �ִ�.
		else if ((centerTocenter.mX >= 0) && ((curTangent < vertTovertTangent) || (curTangent > -vertTovertTangent)))
		{
			// velo.mX = 0.f;

			// �÷��̾� ĳ������ ��ġ�� ���� �������� �������ش�.
			mTransform->SetPosition(Vector2D(blockBasePos.mX + (blockBaseSize.width / 2) + (playerSize.width * ENEMY_SCALE / 2),
				mTransform->GetPosition().mY));
		}
		// �÷��̾�� ���� ���� �鿡 �ִ�.
		else if ((centerTocenter.mX < 0) && ((curTangent < vertTovertTangent) || (curTangent > -vertTovertTangent)))
		{
			// velo.mX = 0.f;

			// �÷��̾� ĳ������ ��ġ�� ���� �������� �������ش�.
			mTransform->SetPosition(Vector2D(blockBasePos.mX - (blockBaseSize.width / 2) - (playerSize.width * ENEMY_SCALE / 2),
				mTransform->GetPosition().mY));
		}

		// ��ȯ�� �ӵ��� �������ش�.
		this->SetVelocity(velo);
	}
}

void EnemyBase::TransBoxCollisionProcess(TransBox* transBox)
{
	if (transBox != nullptr)
	{
		Transform* blockBaseTransform = transBox->GetTransform();
		// ���� ��ġ
		Vector2D blockBasePos = blockBaseTransform->GetPosition();
		// ���� ������
		D2D1_SIZE_F blockBaseSize = transBox->GetSize();
		// Player�� ������
		D2D1_SIZE_F playerSize = dynamic_cast<AABBCollision*>(this->mCollision)->GetSize();

		// -- ��� ���⿡�� �÷��̾�� �浹�ߴ°� + ��� ������ �浹���� �˻��Ѵ�.
		float vertTovertTangent = ((blockBaseSize.height / 2) + (playerSize.height * ENEMY_SCALE / 2)) / ((blockBaseSize.width / 2) + (playerSize.width * ENEMY_SCALE / 2));

		Vector2D centerTocenter = this->mTransform->GetPosition() - blockBasePos;

		float curTangent = centerTocenter.mY / centerTocenter.mX;

		Vector2D velo = this->GetVelocity();

		// �÷��̾�� ���� ���鿡 �ִ�.
		// ���⼭ �� ����ؾ��� ���� �ִٸ�, �÷��̾ ��� �߿��� ��ġ ���� X ..!
		if ((centerTocenter.mY < 0) && ((curTangent >= vertTovertTangent) || (curTangent <= -vertTovertTangent)))
		{
			// �ش� �������� Jump�� ����� �������̸� �Ѿ��.
			if (((mPrevState == EnemyState::Run) && (mCurrentState == EnemyState::Jump))
				|| ((mPrevState == EnemyState::Idle) && (mCurrentState == EnemyState::Jump)))
				return;
		}
		// �÷��̾�� ���� �ظ鿡 �ִ�.
		else if ((centerTocenter.mY > 0) && ((curTangent >= vertTovertTangent) || (curTangent <= -vertTovertTangent)))
		{
			// ��� �� �÷����� �ظ鿡�� �浹�Ͽ��ٸ�.
			if (mDirVector.mY < 0.f)
			{
				transBox->TriggerOn();	// Ʈ���Ÿ� ���ش�

				velo.mY = BLOCKING_POWER;

				// �÷��̾� ĳ������ ��ġ�� ���� �ϴ����� �������ش�.
				mTransform->SetPosition(Vector2D(mTransform->GetPosition().mX,
					blockBasePos.mY + (blockBaseSize.height / 2) + (playerSize.height * ENEMY_SCALE / 2)));
			}
		}
		// �÷��̾�� ���� ������ �鿡 �ִ�.
		else if ((centerTocenter.mX > 0) && ((curTangent < vertTovertTangent) || (curTangent > -vertTovertTangent)))
		{

		}
		// �÷��̾�� ���� ���� �鿡 �ִ�.
		else if ((centerTocenter.mX < 0) && ((curTangent < vertTovertTangent) || (curTangent > -vertTovertTangent)))
		{

		}

		// ��ȯ�� �ӵ��� �������ش�.
		this->SetVelocity(velo);
	}
}

void EnemyBase::ItemBoxCollisionProcess(ItemBox* itemBox)
{
	if (itemBox != nullptr)
	{
		Transform* blockBaseTransform = itemBox->GetTransform();
		// ���� ��ġ
		Vector2D blockBasePos = blockBaseTransform->GetPosition();
		// ���� ������
		D2D1_SIZE_F blockBaseSize = itemBox->GetSize();
		// Player�� ������
		D2D1_SIZE_F playerSize = dynamic_cast<AABBCollision*>(this->mCollision)->GetSize();

		// -- ��� ���⿡�� �÷��̾�� �浹�ߴ°� + ��� ������ �浹���� �˻��Ѵ�.
		float vertTovertTangent = ((blockBaseSize.height / 2) + (playerSize.height * ENEMY_SCALE / 2)) / ((blockBaseSize.width / 2) + (playerSize.width * ENEMY_SCALE / 2));

		Vector2D centerTocenter = this->mTransform->GetPosition() - blockBasePos;

		float curTangent = centerTocenter.mY / centerTocenter.mX;

		Vector2D velo = this->GetVelocity();

		// �÷��̾�� ���� ���鿡 �ִ�.
		// ���⼭ �� ����ؾ��� ���� �ִٸ�, �÷��̾ ��� �߿��� ��ġ ���� X ..!
		// ����, ���� �鿡�� ���� �� .. ���߰��ϰ� ���� �� ������ �� �� �ִ�.
		if ((centerTocenter.mY < -(TILE_HEIGHT / 2) - (ENEMY_HEIGHT * ENEMY_SCALE / 2) + ADJUST_Y) && ((curTangent >= vertTovertTangent) || (curTangent <= -vertTovertTangent)))
		{
			// �߶� �� �Ǵ� �÷��� ���� �ִ�.
			if (mDirVector.mY >= 0.f)
			{
				// �÷��̾ ��� �ִ� �ظ��� �ϳ��� �ִٴ� ���̹Ƿ� ���� �����ϰ� �ٲ��ݴϴ�.
				mIsJumpable = true;

				velo.mY = 0.f;

				// �÷��̾� ĳ������ ��ġ�� ���� ������� �������ش�.
				mTransform->SetPosition(Vector2D(mTransform->GetPosition().mX,
					blockBasePos.mY + 0.005 - (blockBaseSize.height / 2) - (playerSize.height * ENEMY_SCALE / 2)));
			}
		}
		// �÷��̾�� ���� �ظ鿡 �ִ�.
		else if ((centerTocenter.mY >= 0) && ((curTangent >= vertTovertTangent) || (curTangent <= -vertTovertTangent)))
		{
			// ��� �� �÷����� �ظ鿡�� �浹�Ͽ��ٸ�.
			if (mDirVector.mY < 0.f)
			{
				velo.mY = BLOCKING_POWER;

				itemBox->TriggerOn();	// Ʈ���Ÿ� ���ش�

				// �÷��̾� ĳ������ ��ġ�� ���� �ϴ����� �������ش�.
				mTransform->SetPosition(Vector2D(mTransform->GetPosition().mX,
					blockBasePos.mY + (blockBaseSize.height / 2) + (playerSize.height * ENEMY_SCALE / 2)));
			}
		}
		// �÷��̾�� ���� ������ �鿡 �ִ�.
		else if ((centerTocenter.mX >= 0) && ((curTangent < vertTovertTangent) || (curTangent > -vertTovertTangent)))
		{
			// velo.mX = 0.f;

			// �÷��̾� ĳ������ ��ġ�� ���� �������� �������ش�.
			mTransform->SetPosition(Vector2D(blockBasePos.mX + (blockBaseSize.width / 2) + (playerSize.width * ENEMY_SCALE / 2),
				mTransform->GetPosition().mY));
		}
		// �÷��̾�� ���� ���� �鿡 �ִ�.
		else if ((centerTocenter.mX < 0) && ((curTangent < vertTovertTangent) || (curTangent > -vertTovertTangent)))
		{
			// velo.mX = 0.f;

			// �÷��̾� ĳ������ ��ġ�� ���� �������� �������ش�.
			mTransform->SetPosition(Vector2D(blockBasePos.mX - (blockBaseSize.width / 2) - (playerSize.width * ENEMY_SCALE / 2),
				mTransform->GetPosition().mY));
		}

		// ��ȯ�� �ӵ��� �������ش�.
		this->SetVelocity(velo);
	}
}

void EnemyBase::PlatformCollisionProcess(Platform* platform)
{
	if (platform != nullptr)
	{
		Transform* blockBaseTransform = platform->GetTransform();
		// ���� ��ġ
		Vector2D blockBasePos = blockBaseTransform->GetPosition();
		// ���� ������
		D2D1_SIZE_F blockBaseSize = platform->GetSize();
		// Player�� ������
		D2D1_SIZE_F playerSize = dynamic_cast<AABBCollision*>(this->mCollision)->GetSize();

		// -- ��� ���⿡�� �÷��̾�� �浹�ߴ°� + ��� ������ �浹���� �˻��Ѵ�.
		float vertTovertTangent = ((blockBaseSize.height / 2) + (playerSize.height * ENEMY_SCALE / 2)) / ((blockBaseSize.width / 2) + (playerSize.width * ENEMY_SCALE / 2));

		Vector2D centerTocenter = this->mTransform->GetPosition() - blockBasePos;

		float curTangent = centerTocenter.mY / centerTocenter.mX;

		Vector2D velo = this->GetVelocity();

		// �÷��̾�� �÷����� ���鿡 �ִ�.
		// ���⼭ �� ����ؾ��� ���� �ִٸ�, �÷��̾ ��� �߿��� ��ġ ���� X ..!
		// ���⼭ �� ó�� ������ �ǹ��ϴ� �ٴ� �÷����� ĳ���Ͱ� �ǹ��ִ� �Ÿ� �̻� ������ �־�߸� �÷��� ������ �ǽ��ϵ��� ������ ���Դϴ�.
		if ((centerTocenter.mY < -(TILE_HEIGHT / 2) - (ENEMY_HEIGHT * ENEMY_SCALE / 2) + ADJUST_Y) && ((curTangent >= vertTovertTangent) || (curTangent <= -vertTovertTangent)))
		{
			// �ش� �������� Jump�� ����� �������̸� �Ѿ��.
			if (((mPrevState == EnemyState::Run) && (mCurrentState == EnemyState::Jump))
				|| ((mPrevState == EnemyState::Idle) && (mCurrentState == EnemyState::Jump)))
				return;

			if ((mDirVector.mY >= 0.f))
			{
				// �÷��̾ ��� �ִ� �ظ��� �ϳ��� �ִٴ� ���̹Ƿ� ���� �����ϰ� �ٲ��ݴϴ�.
				mIsJumpable = true;

				velo.mY = 0.f;

				// �÷��̾� ĳ������ ��ġ�� �÷����� ������� �������ش�. ������ ����� �񱳰����� �� �۾����Ƿ� ������ �����˴ϴ�.
				mTransform->SetPosition(Vector2D(mTransform->GetPosition().mX,
					blockBasePos.mY + 0.005f - (blockBaseSize.height / 2) - (playerSize.height * ENEMY_SCALE / 2)));

				platform->SetCollisionDir(CollisionDir::UP);
			}
		}
		// �÷��̾�� ���� �ظ鿡 �ִ�.
		else if ((centerTocenter.mY > 0) && ((curTangent >= vertTovertTangent) || (curTangent <= -vertTovertTangent)))
		{
			platform->SetCollisionDir(CollisionDir::DOWN);
		}
		// �÷��̾�� �÷����� ������ �鿡 �ִ�.
		else if ((centerTocenter.mX > 0) && ((curTangent < vertTovertTangent) || (curTangent > -vertTovertTangent)))
		{
			platform->SetCollisionDir(CollisionDir::RIGHT);
		}
		// �÷��̾�� �÷����� ���� �鿡 �ִ�.
		else if ((centerTocenter.mX < 0) && ((curTangent < vertTovertTangent) || (curTangent > -vertTovertTangent)))
		{
			platform->SetCollisionDir(CollisionDir::LEFT);
		}

		// ��ȯ�� �ӵ��� �������ش�.
		this->SetVelocity(velo);
	}
}

void EnemyBase::BlockPlatformCollisionProcess(BlockPlatform* blockplatform)
{
	if (blockplatform != nullptr)
	{
		Transform* blockBaseTransform = blockplatform->GetTransform();
		// ���� ��ġ
		Vector2D blockBasePos = blockBaseTransform->GetPosition();
		// ���� ������
		D2D1_SIZE_F blockBaseSize = blockplatform->GetSize();
		// Player�� ������
		D2D1_SIZE_F playerSize = dynamic_cast<AABBCollision*>(this->mCollision)->GetSize();

		// -- ��� ���⿡�� �÷��̾�� �浹�ߴ°� + ��� ������ �浹���� �˻��Ѵ�.
		float vertTovertTangent = ((blockBaseSize.height / 2) + (playerSize.height * ENEMY_SCALE / 2)) / ((blockBaseSize.width / 2) + (playerSize.width * ENEMY_SCALE / 2));

		Vector2D centerTocenter = this->mTransform->GetPosition() - blockBasePos;

		float curTangent = centerTocenter.mY / centerTocenter.mX;

		Vector2D velo = this->GetVelocity();

		// �÷��̾�� ���� ���鿡 �ִ�.
		// ���⼭ �� ����ؾ��� ���� �ִٸ�, �÷��̾ ��� �߿��� ��ġ ���� X ..!
		// ����, ���� �鿡�� ���� �� .. ���߰��ϰ� ���� �� ������ �� �� �ִ�.
		if ((centerTocenter.mY < -(TILE_HEIGHT / 2) - (ENEMY_HEIGHT * ENEMY_SCALE / 2) + ADJUST_Y) && ((curTangent >= vertTovertTangent) || (curTangent <= -vertTovertTangent)))
		{
			// �߶� �� �Ǵ� �÷��� ���� �ִ�.
			if (mDirVector.mY >= 0.f)
			{
				// �÷��̾ ��� �ִ� �ظ��� �ϳ��� �ִٴ� ���̹Ƿ� ���� �����ϰ� �ٲ��ݴϴ�.
				mIsJumpable = true;

				velo.mY = 0.f;

				// �÷��̾� ĳ������ ��ġ�� ���� ������� �������ش�.
				mTransform->SetPosition(Vector2D(mTransform->GetPosition().mX,
					blockBasePos.mY + 0.005 - (blockBaseSize.height / 2) - (playerSize.height * ENEMY_SCALE / 2)));
			}
		}
		// �÷��̾�� ���� �ظ鿡 �ִ�.
		else if ((centerTocenter.mY >= 0) && ((curTangent >= vertTovertTangent) || (curTangent <= -vertTovertTangent)))
		{
			//// ��� �� �÷����� �ظ鿡�� �浹�Ͽ��ٸ�.
			//if (mDirVector.mY < 0.f)
			//{
			//	velo.mY = 0.f;

			//	// �÷��̾� ĳ������ ��ġ�� ���� �ϴ����� �������ش�.
			//	mTransform->SetPosition(Vector2D(mTransform->GetPosition().mX,
			//		blockBasePos.mY + (blockBaseSize.height / 2) + (playerSize.height * PLAYER_SCALE / 2)));
			//}
		}
		// �÷��̾�� ���� ������ �鿡 �ִ�.
		else if ((centerTocenter.mX >= 0) && ((curTangent < vertTovertTangent) || (curTangent > -vertTovertTangent)))
		{
			// velo.mX = 0.f;

			// �÷��̾� ĳ������ ��ġ�� ���� �������� �������ش�.
			mTransform->SetPosition(Vector2D(blockBasePos.mX + (blockBaseSize.width / 2) + (playerSize.width * ENEMY_SCALE / 2),
				mTransform->GetPosition().mY));
		}
		// �÷��̾�� ���� ���� �鿡 �ִ�.
		else if ((centerTocenter.mX < 0) && ((curTangent < vertTovertTangent) || (curTangent > -vertTovertTangent)))
		{
			// velo.mX = 0.f;

			// �÷��̾� ĳ������ ��ġ�� ���� �������� �������ش�.
			mTransform->SetPosition(Vector2D(blockBasePos.mX - (blockBaseSize.width / 2) - (playerSize.width * ENEMY_SCALE / 2),
				mTransform->GetPosition().mY));
		}

		// ��ȯ�� �ӵ��� �������ش�.
		this->SetVelocity(velo);
	}
}

void EnemyBase::BoxCollisionProcess(BoxBase* box)
{
	if (box != nullptr)
	{
		Transform* blockBaseTransform = box->GetTransform();
		// ���� ��ġ
		Vector2D blockBasePos = blockBaseTransform->GetPosition();
		// ���� ������
		D2D1_SIZE_F blockBaseSize = box->GetSize();
		// Player�� ������
		D2D1_SIZE_F playerSize = dynamic_cast<AABBCollision*>(this->mCollision)->GetSize();

		// -- ��� ���⿡�� �÷��̾�� �浹�ߴ°� + ��� ������ �浹���� �˻��Ѵ�.
		float vertTovertTangent = ((blockBaseSize.height / 2) + (playerSize.height * ENEMY_SCALE / 2)) / ((blockBaseSize.width / 2) + (playerSize.width * ENEMY_SCALE / 2));

		Vector2D centerTocenter = this->mTransform->GetPosition() - blockBasePos;

		float curTangent = centerTocenter.mY / centerTocenter.mX;

		Vector2D velo = this->GetVelocity();

		// �÷��̾�� ���� ���鿡 �ִ�.
		// ���⼭ �� ����ؾ��� ���� �ִٸ�, �÷��̾ ��� �߿��� ��ġ ���� X ..!
		// ����, ���� �鿡�� ���� �� .. ���߰��ϰ� ���� �� ������ �� �� �ִ�.
		if ((centerTocenter.mY < -(TILE_HEIGHT / 2) - (ENEMY_HEIGHT * ENEMY_SCALE / 2) + ADJUST_Y) && ((curTangent >= vertTovertTangent) || (curTangent <= -vertTovertTangent)))
		{
			// �߶� �� �Ǵ� �÷��� ���� �ִ�.
			if (mDirVector.mY >= 0.f)
			{
				// �÷��̾ ��� �ִ� �ظ��� �ϳ��� �ִٴ� ���̹Ƿ� ���� �����ϰ� �ٲ��ݴϴ�.
				mIsJumpable = true;

				velo.mY = 0.f;

				// �÷��̾� ĳ������ ��ġ�� ���� ������� �������ش�.
				mTransform->SetPosition(Vector2D(mTransform->GetPosition().mX,
					blockBasePos.mY + 0.005 - (blockBaseSize.height / 2) - (playerSize.height * ENEMY_SCALE / 2)));
			}
		}
		// �÷��̾�� ���� �ظ鿡 �ִ�.
		else if ((centerTocenter.mY >= 0) && ((curTangent >= vertTovertTangent) || (curTangent <= -vertTovertTangent)))
		{
			// ��� �� �÷����� �ظ鿡�� �浹�Ͽ��ٸ�.
			if (mDirVector.mY < 0.f)
			{
				velo.mY = BLOCKING_POWER;

				// �÷��̾� ĳ������ ��ġ�� ���� �ϴ����� �������ش�.
				mTransform->SetPosition(Vector2D(mTransform->GetPosition().mX,
					blockBasePos.mY + (blockBaseSize.height / 2) + (playerSize.height * ENEMY_SCALE / 2)));
			}
		}
		// �÷��̾�� ���� ������ �鿡 �ִ�.
		else if ((centerTocenter.mX >= 0) && ((curTangent < vertTovertTangent) || (curTangent > -vertTovertTangent)))
		{
			// velo.mX = 0.f;

			// �÷��̾� ĳ������ ��ġ�� ���� �������� �������ش�.
			mTransform->SetPosition(Vector2D(blockBasePos.mX + (blockBaseSize.width / 2) + (playerSize.width * ENEMY_SCALE / 2),
				mTransform->GetPosition().mY));
		}
		// �÷��̾�� ���� ���� �鿡 �ִ�.
		else if ((centerTocenter.mX < 0) && ((curTangent < vertTovertTangent) || (curTangent > -vertTovertTangent)))
		{
			// velo.mX = 0.f;

			// �÷��̾� ĳ������ ��ġ�� ���� �������� �������ش�.
			mTransform->SetPosition(Vector2D(blockBasePos.mX - (blockBaseSize.width / 2) - (playerSize.width * ENEMY_SCALE / 2),
				mTransform->GetPosition().mY));
		}

		// ��ȯ�� �ӵ��� �������ش�.
		this->SetVelocity(velo);
	}
}