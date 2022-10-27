#include "PreCompiledHeader.h"
#include "CheckPoint.h"

#include "Player.h"



CheckPoint::CheckPoint():
	mTrigger(false)
{
	AddAABBCollision();

	// �������̴ϱ� ..!
	AddSpriteRenderer()->SetSortOrder(6);

	GetSpriteRenderer()->SetSprite(new Sprite(20103, Vector2D(0.5f, 0.5f), { TILE_WIDTH, TILE_HEIGHT }, 0.f, Vector2D(240.f, 1120.f)));
}

CheckPoint::~CheckPoint()
{
}

void CheckPoint::Update()
{
	GameObject::Update();

	if (Game::GetInstance().GetPlayer()->GetStagePlayTime() <= 5.f)
		return;

	if (!mTrigger)
	{
		std::vector<GameObject*> collisionInfo = GetCollision()->GetCollisionInfo();

		for (GameObject* object : collisionInfo)
		{
			if (object->GetTag() == "Player")
			{
				Player* player = Game::GetInstance().GetPlayer();
				
				//checkPoint sound
				SoundManager::GetInstance()->Play(500033);

				// üũ����Ʈ ���� �� �پ��� ������
				// �������� ��, �� �϶��� �ǽ��Ѵ� !
				if ((Game::GetInstance().GetCurrentScene()->GetSceneName() == "StageOne") || (Game::GetInstance().GetCurrentScene()->GetSceneName() == "StageTwo"))
					player->PlusBodyWeight(-WEIGHT_DELTA);

				Game::GetInstance().SetSaveBodyWeight(player->GetBodyWeight());

				Game::GetInstance().SetSavePosition(this->GetTransform()->GetPosition() + 60.f * Vector2D::Up);

				// ���� ���� ����
				mTrigger = true;
			}
		}
	}
}