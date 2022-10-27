#include "PreCompiledHeader.h"
#include "Ketchap.h"

std::random_device Ketchap::mRd;

std::mt19937 Ketchap::mMt = std::mt19937(mRd());

std::uniform_int_distribution<int> Ketchap::mDistVelocityX = std::uniform_int_distribution<int>(-300, 300);
std::uniform_int_distribution<int> Ketchap::mDistVelocityY = std::uniform_int_distribution<int>(500.f, 1500.f);

Ketchap::Ketchap():
mGravity(GRAVITY),
mTimer(0.f),
mTrigger(false)
{
	AddAABBCollision();

	//103002
	AddSpriteRenderer()->SetSortOrder(4);

	AddSpriteRenderer()->SetSprite(new Sprite(20103, Vector2D(0.5f, 0.5f), { 80.f, 77.f }, 0.f, Vector2D(1360.f, 563.f)));

	mVelocity = Vector2D(mDistVelocityX(mMt), -mDistVelocityY(mMt));

	mGamePlayer = Game::GetInstance().GetPlayer();
}

Ketchap::~Ketchap()
{
}

void Ketchap::Update()
{
	GameObject::Update();

	float delta = Timer::GetInstance().GetDeltaTime() / 1000.f;

	mTimer += delta;

	// �������� 6�ʿ� �ѹ��� ..
	// ���� �÷��׵Ǹ� 2�ʿ� �ѹ��� ? �����°ɷ�
	if (mTimer >= 3.f)
	{
		Game::GetInstance().GetCurrentScene()->ReserveDestroy(this);

		return;
	}

	// ������ ��¥ ������Ʈ
	if (mVelocity.mY <= MAX_FALLING_SPEED)
		mVelocity.mY = mVelocity.mY + mGravity * delta;

	// �̵� ó��
	mTransform->SetPosition(mTransform->GetPosition() + delta * mVelocity);

	if (!mTrigger)
	{
		std::vector<GameObject*> info = GetCollision()->GetCollisionInfo();

		for (GameObject* object : info)
		{
			if (object->GetTag() == "Player")
			{
				mGamePlayer->PlusBodyWeight(KETCHAP_PLUS);

				mTrigger = true;
			}
		}
	}
	else
	{
		// �ڱ� �ڽ��� ������ ���簡�鼭 �������� �Ұ���
		GetSpriteRenderer()->SetOpacity(GetSpriteRenderer()->GetOpacity() - 0.02f);

		if (GetSpriteRenderer()->GetOpacity() <= 0.f)
			Game::GetInstance().GetCurrentScene()->ReserveDestroy(this);
	}
}