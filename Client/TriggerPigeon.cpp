#include "PreCompiledHeader.h"
#include "TriggerPigeon.h"

#include "ItemBox.h"
#include "PlatformBase.h"
#include "Player.h"

#define PIGEON_SEARCHING 200.f
#define PIGEON_MOVE_SPEED 350.f

TriggerPigeon::TriggerPigeon(std::string color):
mColor(color),
mTrigger(false),
mAnimationClips(std::vector<AnimationClip*>(2)),	// ����� 2���� �ִϸ��̼��� ����
mGamePlayer(Game::GetInstance().GetPlayer()),
mTimer(0.f)
{
	AddAABBCollision()->SetSize({60.f, 20.f});

	AddSpriteRenderer()->SetSortOrder(3);

	LoadAnimation();

	AddAnimator()->SetAnimationClip(mAnimationClips[0]);
}

TriggerPigeon::~TriggerPigeon()
{
}

void TriggerPigeon::LoadAnimation()
{
	if (mColor == "Gray")
	{
		mAnimationClips[0] = Game::GetInstance().GetResource()->GetAnimationClip("BIRD_GRAY_IDLE");
		mAnimationClips[1] = Game::GetInstance().GetResource()->GetAnimationClip("BIRD_GRAY_RUN");
	}
	else if (mColor == "Black")
	{
		mAnimationClips[0] = Game::GetInstance().GetResource()->GetAnimationClip("BIRD_BLACK_IDLE");
		mAnimationClips[1] = Game::GetInstance().GetResource()->GetAnimationClip("BIRD_BLACK_RUN");
	}
}

void TriggerPigeon::FixedUpdate()
{
	GameObject::FixedUpdate();
}

void TriggerPigeon::Update()
{
	GameObject::Update();

	Vector2D playerPos = mGamePlayer->GetTransform()->GetPosition();

	Vector2D myPos = mTransform->GetPosition();

	float distance = (playerPos - myPos).GetMagnitude();

	if ((!mTrigger) && (abs(playerPos.mX - myPos.mX) <= TILE_WIDTH / 2 + PLAYER_WIDTH * PLAYER_SCALE / 2))
	{
		std::vector<GameObject*> info = mGamePlayer->GetCollision()->GetCollisionInfo();

		for (GameObject* object : info)
		{
			// ���� �ִ� ������ �ڽ��� ������ �߳��մϴ�.
			if ((object->GetTag() == "ItemBox") && (dynamic_cast<ItemBox*>(object)->GetCollisionDir() == CollisionDir::DOWN))
			{
				mTrigger = true;

				SoundManager::GetInstance()->Play(500027);

				GetAnimator()->SetAnimationClip(mAnimationClips[1]);

				mTransform->SetRotation(90.f);		// 90����
				
				break;
			}
		}
	}

	// Ʈ���Ű� ���� �ִٸ� Ÿ�̸Ӹ� �����鼭 2�� ������ 
	if (mTrigger)
	{
		float delta =  Timer::GetInstance().GetDeltaTime() / 1000.f;

		mTimer += delta;

		// �������� �̵�
		if ((mColor == "Gray") && (mTimer <= 1.5f))
		{
			// if (mTransform->GetRotation().mX >= -0.99f)		// 180���� �� �Ǿ��ٸ�.
				mTransform->SetRotation(delta * 90.f);		// �ʴ� 30��

			// ���� ���� �������� �̵�, 1.5�� �̻�
			mTransform->SetPosition(mTransform->GetPosition() +  delta * PIGEON_MOVE_SPEED * mTransform->GetRotation());
		}
		else if ((mColor == "Gray") && (mTimer > 1.5f))
		{
			Vector2D dir = (playerPos - myPos).Normalize();

			mTransform->SetRotation(dir);
			mTransform->SetPosition(mTransform->GetPosition() + delta * PIGEON_MOVE_SPEED * dir);
		}
		else if ((mColor == "Black") && (mTimer <= 1.5f))
		{
			// if (mTransform->GetRotation().mX >= 0.01f)		// 180���� �� �Ǿ��ٸ�.
				mTransform->SetRotation(delta * (-90.f));		// �ʴ� 30��

			// ���� ���� �������� �̵�, 1.5�� �̻�
			mTransform->SetPosition(mTransform->GetPosition() + delta * PIGEON_MOVE_SPEED * mTransform->GetRotation());
		}
		else if ((mColor == "Black") && (mTimer > 1.5f))
		{
			Vector2D dir = (playerPos - myPos).Normalize();

			mTransform->SetRotation(dir);
			mTransform->SetPosition(mTransform->GetPosition() + delta * PIGEON_MOVE_SPEED * dir);
		}

		Vector2D lastRota = mTransform->GetRotation();

		if (((lastRota.mX <= 0) && ((lastRota.mY <= 1.f) && (lastRota.mY >= 0))) ||
			((lastRota.mX <= 0) && ((lastRota.mY <= 0) && (lastRota.mY >= -1))))
		{
			mTransform->SetScale(Vector2D(1.f, -1.f));
		}
		else
			mTransform->SetScale(Vector2D(1.f, 1.f));

		std::vector<GameObject*> info = GetCollision()->GetCollisionInfo();

		for (GameObject* object : info)
		{
			if (object->GetTag() == "Player")
			{
				mGamePlayer->SetHp(0);
			}
		}
	}
}
