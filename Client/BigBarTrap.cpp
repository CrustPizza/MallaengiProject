#include "PreCompiledHeader.h"
#include "BigBarTrap.h"

#include "DecoObject.h"
#include "Player.h"
#include "Portal.h"

BigBarTrap::BigBarTrap(GameObject* reserve):
	ItemBox(reserve, 2),
	mIsChecked(false),
	mUpdated(false),
	mBigBar(nullptr),
	mPortal(nullptr)
{
	GameObject* BigBar = new DecoObject();

	BigBar->GetSpriteRenderer()->SetSortOrder(4);

	BigBar->GetSpriteRenderer()->SetSprite(new Sprite(20108, Vector2D(0.5f, 0.5f), {320.f, 157.f}, 0.f, Vector2D(720.f, 160.f)));

	mBigBar = BigBar;

	mBigBar->GetSpriteRenderer()->SetOpacity(0.f);

	mGamePlayer = Game::GetInstance().GetPlayer();

	mPortal = new Portal();

	mPortal->GetSpriteRenderer()->SetSprite(new Sprite(20108, Vector2D(0.5f, 0.5f), { 320.f, 397.f }, 0.f, Vector2D(400.f, 803.f)));

	//Vector2D(24000.f - 4 * TILE_WIDTH, 4 * TILE_HEIGHT); ���� ���� �ϴ� �ּ�
}

BigBarTrap::~BigBarTrap()
{
}

void BigBarTrap::Update()
{
	ItemBox::Update();

	if (!mUpdated)
	{
		mPortal->GetTransform()->SetPosition(mTransform->GetPosition() + Vector2D(27.5 * TILE_WIDTH, 4 * TILE_HEIGHT));

		Game::GetInstance().GetCurrentScene()->AddGameObject(mPortal);

		mUpdated = true;
	}

	// Ʈ���Ű� �Ǿ��ٸ� �Ŵ� �����ٸ� ����߸���, ��Ż�� �߶���Ų��.
	if ((mTrigger) && (!mIsChecked))
	{
		mBigBar->GetTransform()->SetPosition(mTransform->GetPosition() + Vector2D(TILE_WIDTH * 27.5f, - TILE_HEIGHT * 8.f));

		// �÷��̾��� y��ǥ�� BigBar ��ǥ�� ��������� ���°ž�.
		if (abs(mBigBar->GetTransform()->GetPosition().mY - mGamePlayer->GetTransform()->GetPosition().mY) <= 50.f)
		{
	 		mIsChecked = true;	// üŷ �Ϸ�. ��������.

			Game::GetInstance().GetCurrentScene()->AddGameObject(mBigBar);
		}
	}

	if (mIsChecked)
	{
		static float timer = 0.f;

		mBigBar->GetSpriteRenderer()->SetOpacity(mBigBar->GetSpriteRenderer()->GetOpacity() + 0.02f);

		if (mBigBar->GetSpriteRenderer()->GetOpacity() >= 1.f)
		{
			float delta = Timer::GetInstance().GetDeltaTime() / 1000.f;

			timer += delta;

			Transform* bigTransform = mBigBar->GetTransform();

			bigTransform->SetPosition(bigTransform->GetPosition() + delta * 1000.f* Vector2D::Down);

			// �� ������, ��Ż�� �߶���Ų��.
			if (timer >= 0.8f)
			{
				mPortal->GetTransform()->SetPosition(mPortal->GetTransform()->GetPosition() + delta * 1000.f * Vector2D::Down);
			}
		}
	}
}

void BigBarTrap::Release()
{
	ItemBox::Release();

	if (mBigBar != nullptr)
		delete mBigBar;
}