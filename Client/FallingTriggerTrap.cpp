#include "PreCompiledHeader.h"
#include "FallingTriggerTrap.h"

#include "Player.h"

#define SPEED_OF_FALLING 1300.f

FallingTriggerTrap::FallingTriggerTrap(float speed):
TriggerTrap(),
mTrigger(false),
mSpeed(speed)
{
	// �÷��̾��� ��ġ�� �����ϱ� ���ؼ�.
	mGamePlayer = Game::GetInstance().GetPlayer();

	// �������� ���̴ϱ� ..
	GetSpriteRenderer()->SetSortOrder(8);
}

FallingTriggerTrap::~FallingTriggerTrap()
{
}

void FallingTriggerTrap::Update()
{
	// ���� �ߵ��� �ȵǾ��ٸ� �÷��̾��� ��ġ�� �����Ѵ�. �÷��̾��� ��ġ�� �������ٰ� ������ �¾Ƽ� �ߵ��Ǹ� ���� ..!
	if (!mTrigger)
	{
		// y ��ǥ�� ������ �Ʒ��� �־�� �ϸ� => �÷��̾� y�� �� Ŀ����
		// x ��ǥ�� ���ľ���. => �׷��� Ʈ���ŵǼ� ��������.
		Vector2D playerPos = mGamePlayer->GetTransform()->GetPosition();

		Vector2D myPos = GetTransform()->GetPosition();

		D2D1_SIZE_F playerSize = dynamic_cast<AABBCollision*>(mGamePlayer->GetCollision())->GetSize();

		D2D1_SIZE_F mySize = dynamic_cast<AABBCollision*>(this->GetCollision())->GetSize();

		if ((playerPos.mY >= myPos.mY) && (abs(myPos.mX - playerPos.mX) <= playerSize.width / 2 + mySize.width / 2))
			mTrigger = true;
	}
	else if (mTrigger)
	{
		// ���ϸ� �ǽ��Ѵ�.
		mTransform->SetPosition(mTransform->GetPosition() + (Timer::GetInstance().GetDeltaTime() / 1000.f) * mSpeed * Vector2D::Down);

		TriggerTrap::Update();
	}
}