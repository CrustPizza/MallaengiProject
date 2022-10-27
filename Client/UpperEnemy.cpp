#include "PreCompiledHeader.h"
#include "UpperEnemy.h"

float UpperEnemy::mTime = 0.f;

UpperEnemy::UpperEnemy(std::string type):
	mGamePlayer(Game::GetInstance().GetPlayer()),
	mType(type),
	mTrigger(false)
{
	mMyTime = mTime;
	mTime += 2.f;	// ���� ���̶� 2���� ������ ����.
}

UpperEnemy::~UpperEnemy()
{
}

void UpperEnemy::Update()
{
	GameObject::Update();

	if (!mTrigger)
	{
		Vector2D myPos = mTransform->GetPosition();

		Vector2D playerPos = mGamePlayer->GetTransform()->GetPosition();
		
		if (myPos.mX - playerPos.mX <= 500.f)
		{
			mMyTime += Timer::GetInstance().GetDeltaTime() / 1000.f;

			if (mMyTime >= mTime - 2.f)
			{
				FreeEnemy* free = new FreeEnemy(mType, true);

				free->GetTransform()->SetPosition(myPos);

				// �������� ���������
				free->SetVelocity(Vector2D(-500.f, -2000.f));

				Game::GetInstance().GetCurrentScene()->AddGameObject(free);

				mTrigger = true;
			}
		}
	}
}