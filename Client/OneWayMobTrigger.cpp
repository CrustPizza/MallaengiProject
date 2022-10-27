#include "PreCompiledHeader.h"
#include "OneWayMobTrigger.h"

#include "OneWayMob.h"

OneWayMobTrigger::OneWayMobTrigger(std::string type, float moveSpeed, Vector2D Dir)
{
	mReserve = new OneWayMob(type, moveSpeed, Dir);

	mDirrection = Dir;

	mGamePlayer = Game::GetInstance().GetPlayer();

	mIsDone = false;
}

OneWayMobTrigger::~OneWayMobTrigger()
{
}

void OneWayMobTrigger::Update()
{
	GameObject::Update();

	Vector2D myPos = mTransform->GetPosition();

	Vector2D playerPos = mGamePlayer->GetTransform()->GetPosition();

	if ((myPos.mY <= playerPos.mY) &&						// ������ �ؿ� �����鼭
		(playerPos.mX - myPos.mX >= mDirrection.mX * 100.f))	// �̵� ������� 100��ŭ ������ ������
	{
		mTrigger = true;
	}

	if ((mTrigger) && (!mIsDone))
	{
		mReserve->GetTransform()->SetPosition(mTransform->GetPosition());

		mReserve->SetVelocity(Vector2D(0.f, 500.f));

		Game::GetInstance().GetCurrentScene()->AddGameObject(mReserve);

		mIsDone = true;
	}
}