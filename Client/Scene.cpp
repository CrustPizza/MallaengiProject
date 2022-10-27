#include "PreCompiledHeader.h"
#include "Scene.h"

#include "FadeInObject.h"
#include "Player.h"

#include "CollisionGrid.h"

// Blocks
#include "BlockBase.h"
#include "BreakableBlock.h"
#include "TransBox.h"

// Platforms
#include "Platform.h"

// Traps
#include "FallingBlock.h"

// Structure
#include "StageOneBackGround.h"
#include "BlockPlatform.h"
#include "BodyScale.h"
#include "CheckPoint.h"
#include "DecoObject.h"
#include "FadeInObject.h"
#include "Food.h"
#include "JsonManager.h"
#include "LRMovingPlatform.h"
#include "MainCamera.h"
#include "TriggerTrap.h"
#include "UDMovingPlatform.h"

#include "ItemBox.h"
#include "RisingTrap.h"

#include "CollisionGrid.h"

// Enemy
#include "BigBarTrap.h"
#include "CatBoxTrap.h"
#include "DoorOpen.h"
#include "FakeTriggerBuilding.h"
#include "FallingBlockTriggerTrap.h"
#include "FallingEnemy.h"
#include "FallingPlatform.h"
#include "FreeEnemy.h"
#include "FallingTriggerTrap.h"
#include "KetchapTrap.h"
#include "KillMayo.h"
#include "Portal.h"
#include "SavePointBoxTrap.h"
#include "ShurikenPlatform.h"
#include "TriggerPigeon.h"
#include "UpperEnemy.h"
#include "UpperShuriken.h"
#include "WalkPad.h"
#include "DumbellDown.h"
#include "OneWayMob.h"
#include "OneWayMobTrigger.h"
#include "Niddle.h"
#include "RedRight.h"
#include "ExitPortal.h"
#include "RedUp.h"
#include "LRMob.h"
#include "BrokenHeart.h"
#include "SlowNiddle.h"
#include "TrapTile.h"
#include "FiveFalling.h"
#include "RidingNacho.h"

#include "SubCamera.h"
#include "TriggerDoor.h"

#define COLLISION_GRID_COUNT 52


Scene::Scene(std::string sceneName) :
	mSceneName(sceneName),
	mGameObjects(std::vector<GameObject*>()),
	mCollisions(std::vector<Collision*>()),
	mIsFirstLoad(true)
{

}

Scene::~Scene()
{

}

/// <summary>
/// ��������Ʈ ������ �ʿ��� ������ ���� �޾Ƽ� ������ ������ �ϰ� �����.
/// </summary>
Sprite* MakeSprite(const std::vector<TileData>::iterator& iter, Vector2D rectpos = {}, Vector2D pivot = Vector2D(0.5f, 0.5f), float delayTime = 0.f)
{
	D2D1_RECT_F rect = iter->imageRect;

	Sprite* sprite = new Sprite(iter->imageID, pivot,
		{ rect.right - rect.left + 1, rect.bottom - rect.top + 1 }, delayTime, Vector2D(rect.left, rect.top) + rectpos);

	return sprite;
}

/// <summary>
/// ���ڷ� ������Ʈ�� ��ġ��, �����ڿ� �ʿ��� �Ű� ������ ���� ���ڷ� �޴´�.
/// </summary>
template <class T, typename... argsT>
T* MakeObject(Vector2D pos, argsT ... args)
{
	T* newObject = new T(args...);
	newObject->GetTransform()->SetPosition(pos);

	return newObject;
}

void Scene::BuildScene()
{
	mGamePlayer->SetPlayerState(PlayerState::Idle);

	// �÷��̾��� �ð��� 0���� .. ������ �ʱ�ȭ�Ѵ� ..! �� ��������� ����.
	mGamePlayer->SetStagePlayTime(0.f);

	//for (int i = 0; i < COLLISION_GRID_COUNT; i++)
	//{
	//	CollisionGrid* grid = new CollisionGrid(Vector2D(-1000 + 2000 * (i % 12), 1000 + 1000 * static_cast<int>(i / 12)));

	//	mCollisionGrids.push_back(grid);

	//	AddGameObject(grid);
	//}
}

void Scene::PrePhase() const
{
	// �� �����ӿ��� ����� �浹 ���� / �浹 ���� ���� �ʱ�ȭ
	for (Collision* collision : mCollisions)
	{
		collision->SetIsCollision(false);
		collision->GetCollisionInfo().clear();
	}

	//for (CollisionGrid* grid : mCollisionGrids)
	//	grid->mCollisions.clear();
}

void Scene::DataPhase()
{
	// Timer Update
	Timer::GetInstance().Update();

	// Key Input Update
	Input::GetInstance().Update();

	// �̰� �۾�ǥ���� ������ �� �̻��� �������� �����ϱ� ���� �ڵ��ε� .. ���߿� ���ô�.
	if (Timer::GetInstance().GetDeltaTime() / 1000.f > 0.04f)
		Timer::GetInstance().Update();

	// ��Ÿ �ش� Scene���� �ʿ��� Data ���� Update
	BirthAndDeath();
}

void Scene::PhysicsPhase() const
{
#pragma region Collision
	//// ����ϸ� ����ȭ�� �� �� ������, ������ �ʿ䰡 �ִ�.
	//for (Collision* firCollision : mCollisions)
	//	for (Collision* SecCollision : mCollisions)
	//	{
	//		if (firCollision == SecCollision)
	//			continue;

	//		firCollision->CheckCollision(SecCollision);
	//	}

	//// 1. �׸���� ���� AABB üŷ
	//for (CollisionGrid* grid : mCollisionGrids)
	//{
	//	Vector2D gridPos = grid->GetTransform()->GetPosition();

	//	for (GameObject* object : mGameObjects)
	//	{
	//		// �� .. �̰� ����Ʈ �����ϰ� �ִ� ��� �˸� ����� �� Ȯ���ϰ� ����ȭ �����ѵ� ���� ������ �ð��� ���� ..
	//		
	//		Vector2D pos = object->GetTransform()->GetPosition();

	//		if ((abs(gridPos.GetX() - pos.GetX()) < GRID_HALF_WIDTH + 70.f)
	//			&& (abs(gridPos.GetY() - pos.GetY()) < GRID_HALF_HEIGHT + 70.f))
	//		{
	//			Collision* collision = object->GetCollision();

	//			if (collision != nullptr)
	//				grid->mCollisions.push_back(collision);
	//		}
	//	}
	//}

	//// 2. �׸��� �ȿ��� üũ
	//for (CollisionGrid* grid : mCollisionGrids)
	//{
	//	std::vector<Collision*> collisions = grid->mCollisions;

	//	for (Collision* firCollision : collisions)
	//	{
	//		for (Collision* secCollision : collisions)
	//		{
	//			// �÷��� ���� �浹�� ������ �޾ƾ� �ϴ� �༮�� (Player, Enemy, Item)
	//			// ���� ������ �׳� �� ���� ����ȭ�� �ϼ��ȴ� ..
	//			std::string tag = firCollision->GetGameObject()->GetTag();

	//			if ((tag == "Player") || (tag == "Enemy"))
	//				firCollision->CheckCollision(secCollision);
	//		}
	//	}
	//}
#pragma endregion
	// �ݴ��� ����ȭ ��ų ..
	for (Collision* firCollision : mCollisions)
	{
		// �÷��� ���� �浹�� ������ �޾ƾ� �ϴ� �༮�� (Player, Enemy, Item)
		// ���� ������ �׳� �� ���� ����ȭ�� �ϼ��ȴ� ..
		std::string tag = firCollision->GetGameObject()->GetTag();

		if ((tag == "Player") || (tag == "Enemy"))
			for (Collision* secCollision : mCollisions)
				firCollision->CheckCollision(secCollision);
	}
}

void Scene::FixedPhase() const
{
	if (Game::GetInstance().GetIsOptionOn())
	{
		for (GameObject* object : mGameObjects)
			if (object->GetTag() == "UI")
				object->FixedUpdate();
	}
	else if ((mGamePlayer->GetCurrentState() == PlayerState::Trans) || (mGamePlayer->GetCurrentState() == PlayerState::Die))
	{
		for (GameObject* object : mGameObjects)
			if ((object->GetTag() == "Player") || (object->GetTag() == "UI") || (object->GetTag() == "Camera") || (object->GetTag() == "BackGround"))
				object->FixedUpdate();
	}
	else
	{
		for (GameObject* object : mGameObjects)
			object->FixedUpdate();
	}
}

void Scene::LogicPhase() const
{
	// ���� Option�� �����ִٸ�
	// UI�� ������Ʈ�Ѵ�. (�÷��̾� ����, Ʈ�������� �ɼ��� ���� ����)
	if (Game::GetInstance().GetIsOptionOn())
	{
		for (GameObject* object : mGameObjects)
			if ((object->GetTag() == "UI") || (object->GetTag() == "Camera") || (object->GetTag() == "BackGround"))
				object->Update();

		for (GameObject* object : mGlobalGameObjects)
			if ((object->GetTag() == "UI") || (object->GetTag() == "Camera") || (object->GetTag() == "BackGround"))
				object->Update();

		for (GameObject* object : mGameObjects)
			if ((object->GetTag() == "UI") || (object->GetTag() == "Camera") || (object->GetTag() == "BackGround"))
				object->LateUpdate();

		for (GameObject* object : mGlobalGameObjects)
			if ((object->GetTag() == "UI") || (object->GetTag() == "Camera") || (object->GetTag() == "BackGround"))
				object->LateUpdate();
	}
	// �÷��̾ Trans Or Die ������ �� UI�� PLAYER�� Update�� �����մϴ�.
	else if ((mGamePlayer->GetCurrentState() == PlayerState::Trans) || (mGamePlayer->GetCurrentState() == PlayerState::Die))
	{
		for (GameObject* object : mGameObjects)
			if ((object->GetTag() == "Player") || (object->GetTag() == "UI") || (object->GetTag() == "Camera") || (object->GetTag() == "BackGround"))
				object->Update();

		for (GameObject* object : mGlobalGameObjects)
			if ((object->GetTag() == "Player") || (object->GetTag() == "UI") || (object->GetTag() == "Camera") || (object->GetTag() == "BackGround"))
				object->Update();

		for (GameObject* object : mGameObjects)
			if ((object->GetTag() == "Player") || (object->GetTag() == "UI") || (object->GetTag() == "Camera") || (object->GetTag() == "BackGround"))
				object->LateUpdate();

		for (GameObject* object : mGlobalGameObjects)
			if ((object->GetTag() == "Player") || (object->GetTag() == "UI") || (object->GetTag() == "Camera") || (object->GetTag() == "BackGround"))
				object->LateUpdate();
	}
	else
	{
		for (GameObject* object : mGameObjects)
			object->Update();

		for (GameObject* object : mGlobalGameObjects)
			object->Update();

		for (GameObject* object : mGameObjects)
			object->LateUpdate();

		for (GameObject* object : mGlobalGameObjects)
			object->LateUpdate();
	}
}

void Scene::RenderPhase() const
{
	Graphics::GetInstance().BeginRender();

	Vector2D cameraPos = mCamera->GetTransform()->GetPosition();

	for (Renderer* renderer : mRenderers)
	{
		/*Vector2D objectPos = renderer->GetGameObject()->GetTransform()->GetPosition();

		if ((abs(cameraPos.GetX() - objectPos.GetX()) < size.width / 2 * 1.3f + 200.f)
			&& (abs(cameraPos.GetY() - objectPos.GetY()) < size.height / 2 * 1.3f + 200.f))
		{
		}*/
		renderer->Render();
#pragma region DEBUG_MODE
		// Debug Mode
		if (Game::GetInstance().GetIsDebugMode())
		{
			GameObject* object = renderer->GetGameObject();

			Collision* collision = object->GetCollision();

			if (collision != nullptr)
			{
				AABBCollision* aabbcollision = dynamic_cast<AABBCollision*>(collision);

				D2D1_SIZE_F size = aabbcollision->GetSize();

				if (aabbcollision->GetIsCollision())
					Graphics::GetInstance().DrawRectangle(Vector2D(0, 0), size, D2D1::ColorF(1, 0, 0, 1));
				else
					Graphics::GetInstance().DrawRectangle(Vector2D(0, 0), size, D2D1::ColorF(0, 1, 0, 1));

				// ����׿��� Ÿ���� ID ���
				TCHAR buf[16];
				_stprintf_s(buf, _T("%d"), object->GetID());
				Graphics::GetInstance().DrawText(-size.width / 2, -size.height / 2, buf);
			}
		}
	}

	// �밡���� �� ������ �� �ڵ�
	// �浹�ڽ� ����׸�� �ܿ� ������
	// Debug Mode -> �ΰ��Ӿ��� �����. ���� �޶����� ��ġ ����
	if (Game::GetInstance().GetIsDebugMode())
	{
		// ��������3������ ���ڰ� ����ٴ� ��?
		D2D1_SIZE_F size = Graphics::GetInstance().GetScreenSize();

		Graphics::GetInstance().DrawText(size.width - 2100, size.height - 1000, "FPS : %.3f", Timer::GetInstance().GetDeltaTime() / 1000.f);
		//-70, -40
		Vector2D _pos = mGamePlayer->GetTransform()->GetPosition();

		Graphics::GetInstance().DrawText(size.width - 2100, size.height - 900, "Pos X : %f", _pos.mX);

		Graphics::GetInstance().DrawText(size.width - 2100, size.height - 800, "���� ������ �ֳ�");
	}

	// /* �������� 3 �� ����� ����?
	if (Game::GetInstance().GetIsDebugMode())
	{
		// ��,���ôϤþƤӸ���
		// ī�޶� ���� ī�޶�ιٲٱ� -> ����ī�޶�� �����̹Ƿ� ��ǥ�� ������ �ȴ� (0, 0, 19020, 1080) ����
		//SubCamera* subcamera = new SubCamera;
		
		//subcamera->GetCamera();
		//AddGameObject(new SubCamera());
		// D2D1_SIZE_F size = Graphics::GetInstance().SetCamera();
		//D2D1_SIZE_F size = Graphics::GetInstance().GetScreenSize();

		//Graphics::GetInstance().DrawText( 0, 0, "FPS : %.3f", Timer::GetInstance().GetDeltaTime() / 1000.f);
		
		//Vector2D _pos = mGamePlayer->GetTransform()->GetPosition();
		
		//Graphics::GetInstance().DrawText(0, 0, "Pos X : %f", _pos.mX);
		
		//Graphics::GetInstance().DrawText(0, 0, "�³� ���Ͷ�");

	}
	//*/

#pragma endregion

	Graphics::GetInstance().EndRender();
}

// ���� �÷��̾ �װų�, �ܰ谡 ��ȭ�ϴ� ��Ȳ�̶�� �ٸ� ģ������ Update�� ���߰�
// �÷��̾ Update �մϴ� ..!

// �÷��̾ �̹� �����ӿ��� �ܰ谡 ��ȭ�ߴ��� || PlayerState::Die ���� üũ�ؼ� �����ϴ�.
void Scene::SceneLoop()
{
	this->PrePhase();

	this->DataPhase();

	this->FixedPhase();

	this->PhysicsPhase();

	this->LogicPhase();

	this->RenderPhase();
}

void Scene::ClearScene()
{
	// �÷��̾ ������ Object���� ���� !
	for (auto& object : mGameObjects)
	{
		if ((object != mGamePlayer) && (object != nullptr))
		{
			object->Release();
			delete object;
		}
	}

	// �÷��� ���ο� �ִ� ģ���� Clear
	mGameObjects.clear();
	mRenderers.clear();
	mCollisions.clear();
	mCollisionGrids.clear();

	// ���� �÷��� ���ο� �ִ� ģ���� clear
	mCreateReserveList.clear();
	mDestroyReserveList.clear();
}

void Scene::LoadSceneData(const string& path, int mapCount)
{
	MapJson stageOne;

	GameObject* newObject = nullptr;

	float idleTimeControl = 0.f;

	for (int i = 0; i < mapCount; i++)
	{
		// ���� �ϳ��� ��� �ٷ� �ҷ����� �������� ��� ������� �ҷ��´�.
		if (mapCount == 1)
		{
			stageOne.DeserializeFromCArchive(path);
		}
		else
		{
			TCHAR buf[256];

			_stprintf_s(buf, path.c_str(), i + 1);

			stageOne.DeserializeFromCArchive(buf);
		}

		for (auto iter = stageOne.mTileList.begin(); iter != stageOne.mTileList.end(); iter++)
		{
			// 1. �� ��° Ÿ�Ͽ� �̰��� ���Դ��� üũ�մϴ�.
			float _yOriTile = (iter->index >> 16);
			float _xOriTile = (iter->index & 0xffff);

			// �׷� Ÿ���� ������ ��ü�� ������ ����մϴ�.
			float _yTile = (_yOriTile)+(1.0f + (iter->group.back() >> 16)) / 2;
			float _xTile = (_xOriTile)+(1.0f + (iter->group.back() & 0xffff)) / 2;

			// �� ������ ���� ��ǥ�� �������ݴϴ�.
			Vector2D objectPos = { (_xTile + 100 * i) * TILE_WIDTH, _yTile * TILE_HEIGHT };

			// �÷��� Ÿ�� (�ܵ� �ִ�, �ö� �� �ִ� ���̴�)
			if ((iter->ID == 10101001) || (iter->ID == 10101002) || (iter->ID == 10101003))
			{
				newObject = MakeObject<Platform>(objectPos);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			// �÷��� Ÿ�� (�ܵ� ����, �ö� �� ���� ���̴�)
			else if ((iter->ID == 10101004) || (iter->ID == 10101005) || (iter->ID == 10101006)
				|| (iter->ID == 10101007) || (iter->ID == 10101008) || (iter->ID == 10101009))
			{
				newObject = MakeObject<DecoObject>(objectPos);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			// �÷��� Ÿ�� (������ �Ұ�����, ������ �� �� �ִ�)
			else if ((iter->ID == 10101010) || (iter->ID == 10101011) || (iter->ID == 10101012) || (iter->ID == 10101013) || (iter->ID == 10101014)
				|| (iter->ID == 10101015))
			{
				newObject = MakeObject<BoxBase>(objectPos);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if ((iter->ID == 10101017) || (iter->ID == 10101018))
			{
				newObject = MakeObject<BoxBase>(objectPos);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			// ���� Ÿ�� ~
			else if ((iter->ID == 10102001) || (iter->ID == 10102002) || (iter->ID == 10102003))
			{
				newObject = MakeObject<Platform>(objectPos);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}

			// �ӵ� Ÿ��
			else if ((iter->ID == 10103001) || (iter->ID == 10103002) || (iter->ID == 10103003))
			{
				newObject = MakeObject<Platform>(objectPos);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			// �ӵ� Ÿ�� ���� �� �ϴ�
			else if ((iter->ID == 10103004) || (iter->ID == 10103005) || (iter->ID == 10103006) || (iter->ID == 10103007) || (iter->ID == 10103008) || (iter->ID == 10103009))
			{
				newObject = MakeObject<DecoObject>(objectPos);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}

			else if (iter->ID == 101001)	// Tree (apple X)
			{
				newObject = MakeObject<DecoObject>(objectPos);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 102002)	// Tree (apple O)
			{
				newObject = MakeObject<DecoObject>(objectPos);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 101003)	// Street Lamp (1)
			{
				newObject = MakeObject<DecoObject>(objectPos);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 101004)	// Street Lamp (2)
			{
				newObject = MakeObject<DecoObject>(objectPos);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 101005)	// Lamp (�������� ����)
			{
				newObject = MakeObject<FallingTriggerTrap>(objectPos, 1300.f);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 101006)	// Store
			{
				newObject = MakeObject<DecoObject>(objectPos);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 101007)	// Sign
			{
				newObject = MakeObject<DecoObject>(objectPos);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 101008)	// Cloud 1
			{
				newObject = MakeObject<DecoObject>(objectPos);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 101009)	// Cloud 2
			{
				newObject = MakeObject<DecoObject>(objectPos);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 101010)	// Cloud 3
			{
				newObject = MakeObject<DecoObject>(objectPos);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 101011)	// ���� (�������� 1 ��Ż)
			{
				newObject = MakeObject<Portal>(objectPos);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 101012)	// �ܹ���
			{
				newObject = MakeObject<FreeEnemy>(objectPos + 150.f * Vector2D::Up, "Hamburger");

				AddGameObject(newObject);
			}
			else if (iter->ID == 101013)	// �ܹ��� 2
			{

			}
			else if (iter->ID == 101014)	// ų Ŭ���� (������ �״°ǰ� ..?)
			{
				newObject = MakeObject<DecoObject>(objectPos);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 101015)	// ����, �׷���
			{
				newObject = MakeObject<DecoObject>(objectPos);

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 101016)	// ����, ��
			{
				newObject = MakeObject<DecoObject>(objectPos);

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 101017)	// ��� �ڽ�
			{
				newObject = MakeObject<Platform>(objectPos);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 101018)	// ���� �÷��� (��, �Ʒ� 5ĭ)
			{
				Vector2D startPos = { TILE_WIDTH / 2 + (_xOriTile + 100 * i) * (TILE_WIDTH), TILE_HEIGHT / 2 + (_yOriTile) * (TILE_HEIGHT) };

				newObject = MakeObject<UDMovingPlatform>(objectPos, startPos, 5, 2, idleTimeControl, "Block");

				idleTimeControl += TILE_HEIGHT * 5 / UD_MOVING_SPEED;

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				AddGameObject(newObject);
			}
			else if (iter->ID == 101019)	// ���� �÷��� (��, �� 7ĭ)
			{
				Vector2D startPos = { TILE_WIDTH / 2 + (_xOriTile + 100 * i) * (TILE_WIDTH), TILE_HEIGHT / 2 + (_yOriTile) * (TILE_HEIGHT) };

				newObject = MakeObject<LRMovingPlatform>(objectPos, startPos, 7, 3, 0.f);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				AddGameObject(newObject);
			}
			else if (iter->ID == 102001)	// Item Box - ���� 1
			{
				Food* reserve = new Food(-0.5f, 20106, Vector2D(80.f, 80.f), true);

				reserve->GetTransform()->SetPosition(objectPos);

				newObject = MakeObject<ItemBox>(objectPos, reserve, 1);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 102002)	// Item Box - ���� 2
			{
				Food* reserve = new Food(-0.5f, 20106, Vector2D(240.f, 80.f), true);

				reserve->GetTransform()->SetPosition(objectPos);

				newObject = MakeObject<ItemBox>(objectPos, reserve, 1);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 102003)	// Item Box - Ketchup
			{
				Food* reserve = new Food(-0.5f, 20103, Vector2D(1360.f, 563.f), true);

				reserve->GetTransform()->SetPosition(objectPos);

				newObject = MakeObject<ItemBox>(objectPos, reserve, 1);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 102004)	// �ֺ� �ڽ�
			{
				newObject = MakeObject<ItemBox>(objectPos, nullptr, 1);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 102005)	// Ĺ ���� ..? ���� �̰Ŵ�
			{
				newObject = MakeObject<ItemBox>(objectPos, nullptr, 1);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 102006 || iter->ID == 102011)	// Block
			{
				newObject = MakeObject<BreakableBlock>(objectPos);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 103001)	// Mayo
			{
				newObject = MakeObject<DecoObject>(objectPos);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 103002)	// ����
			{
				newObject = MakeObject<DecoObject>(objectPos);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 103003)	// ���� ���̽�
			{
				newObject = MakeObject<DecoObject>(objectPos);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 103004)	// ��ø ���̽�
			{
				newObject = MakeObject<DecoObject>(objectPos);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 104001)	// ����
			{
				newObject = MakeObject<DecoObject>(objectPos);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 104002)	// ����
			{
				newObject = MakeObject<DecoObject>(objectPos);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 104003)	// ĥ�� ����
			{
				newObject = MakeObject<DecoObject>(objectPos);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 104004)	// �ܳ���
			{
				newObject = MakeObject<DecoObject>(objectPos);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 104005)	// �������� .. ��������
			{
				newObject = MakeObject<DecoObject>(objectPos);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 104006)	// ���� ���̽�
			{
				newObject = MakeObject<DecoObject>(objectPos);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 104007)	// ���� ���� ����
			{
				newObject = MakeObject<DecoObject>(objectPos);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 104008)	// �׳� ���� (?)
			{
				newObject = MakeObject<DecoObject>(objectPos);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 104009)	// ����
			{
				newObject = MakeObject<DecoObject>(objectPos);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 104010)	// ���� ���
			{
				newObject = MakeObject<DecoObject>(objectPos);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 104011)	// ū ���
			{
				newObject = MakeObject<FallingTriggerTrap>(objectPos, 1300.f);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 104012)	// ������ 1
			{
				newObject = MakeObject<Food>(objectPos, Food::mMallaengBar, iter->imageID, Vector2D(iter->imageRect.left, iter->imageRect.top), false);

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 104013)	// ������ 2
			{
				newObject = MakeObject<Food>(objectPos, Food::mMallaengBar, iter->imageID, Vector2D(iter->imageRect.left, iter->imageRect.top), false);

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 104014)	// ���� ������
			{
				newObject = MakeObject<DecoObject>(objectPos);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 105001)	// ��� �ڽ�
			{
				newObject = MakeObject<DecoObject>(objectPos);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 105002)	// ����
			{
				newObject = MakeObject<DecoObject>(objectPos);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 105003)	// �ν�
			{
				newObject = MakeObject<DecoObject>(objectPos);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 106001)	// ����ũ �÷���
			{
				newObject = MakeObject<KetchapTrap>(objectPos);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 107001)	// ���� �÷���
			{
				newObject = MakeObject<CheckPoint>(objectPos);

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			// Trap (�����ϰ� �ϳ��� ���� �ʿ䰡 �ִ�
			else if (iter->ID == 109001)	// �κ����� �ڽ� (1) => ������ �ʷϻ� ����� �ڽ��� �ٲ�
			{
				newObject = MakeObject<TransBox>(objectPos);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter, Vector2D(160.f, 0.f)));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				newObject->GetSpriteRenderer()->SetOpacity(0.f);

				AddGameObject(newObject);
			}

			else if (iter->ID == 109003)	// �κ�����ڽ� 3 (3�������� �ڽ� ������ �װɷ�)
			{
				newObject = MakeObject<TransBox>(objectPos);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter, Vector2D(160.f, 0.f)));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				newObject->GetSpriteRenderer()->SetOpacity(0.f);

				AddGameObject(newObject);
			}
			else if (iter->ID == 109004)	// ���� (������ �״�)
			{
				newObject = MakeObject<TriggerTrap>(objectPos);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 109005)	// ���� (���� ����ϴ�)
			{
				newObject = MakeObject<RisingTrap>(objectPos);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 109006)	// ���� (���� ����ϴ�)
			{
				newObject = MakeObject<RisingTrap>(objectPos);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 109007)	// ���� (���� ����ϴ�)
			{
				newObject = MakeObject<RisingTrap>(objectPos);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 109008)	// ���� ��� (3)
			{
				Vector2D startPos = { TILE_WIDTH / 2 + (_xOriTile + 100 * i) * (TILE_WIDTH), TILE_HEIGHT / 2 + (_yOriTile) * (TILE_HEIGHT) };

				newObject = MakeObject<FallingBlock>(objectPos, startPos, 3);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 109009)	// ���� ��� (1)
			{
				Vector2D startPos = { TILE_WIDTH / 2 + (_xOriTile + 100 * i) * (TILE_WIDTH), TILE_HEIGHT / 2 + (_yOriTile) * (TILE_HEIGHT) };

				newObject = MakeObject<FallingBlock>(objectPos, startPos, 1);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 109010)	// ���� ������
			{
				newObject = MakeObject<UpperShuriken>(objectPos);

				AddGameObject(newObject);
			}
			else if (iter->ID == 109011)	// Ʈ���� ����
			{
				newObject = MakeObject<TriggerTrap>(objectPos, true);		// �̹��� ���� �����

				newObject->GetTransform()->SetPosition(newObject->GetTransform()->GetPosition() + 27.f * Vector2D::Down);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 109012)	// �߶� Ʈ���� ���� ��� (3)
			{
				Vector2D startPos = { TILE_WIDTH / 2 + (_xOriTile + 100 * i) * (TILE_WIDTH), TILE_HEIGHT / 2 + (_yOriTile) * (TILE_HEIGHT) };

				newObject = MakeObject<FallingBlockTriggerTrap>(objectPos, startPos, 3);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 109014)	// ������ Ŭ����
			{
				newObject = MakeObject<TriggerTrap>(objectPos, new Sprite(20105, Vector2D(0.5f, 0.5f), { 1270 - 960, 480 - 243 }, 0.f, Vector2D(960.f, 243.f)));

				dynamic_cast<AABBCollision*>(newObject->GetCollision())->SetSize({ TILE_WIDTH * 4, TILE_HEIGHT * 2 });

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 109015)	// ����ũ ��Ż ?
			{
				newObject = MakeObject<FakeTriggerBuilding>(objectPos);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 109016)	// Ʈ���� Ʈ��
			{
				newObject = MakeObject<DecoObject>(objectPos);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 109017)	// Ʈ���� ����
			{
				newObject = MakeObject<DecoObject>(objectPos);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 109018)	// ���� �÷���
			{
				Vector2D startPos = { TILE_WIDTH / 2 + (_xOriTile + 100 * i) * (TILE_WIDTH), TILE_HEIGHT / 2 + (_yOriTile) * (TILE_HEIGHT) };

				newObject = MakeObject<FallingPlatform>(objectPos, startPos);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 109020)	// ���� ���� Ÿ��
			{
				Vector2D startPos = { TILE_WIDTH / 2 + (_xOriTile + 100 * i) * (TILE_WIDTH), TILE_HEIGHT / 2 + (_yOriTile) * (TILE_HEIGHT) };

				newObject = MakeObject<FallingBlock>(objectPos, startPos, 3);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 109022)	// Upper Hamburger 
			{
				newObject = MakeObject<UpperEnemy>(objectPos, "Hamburger");

				AddGameObject(newObject);
			}
			else if (iter->ID == 109024)	// ����
			{
				newObject = MakeObject<TriggerPigeon>(objectPos, "Gray");

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 109025)	// ����
			{
				newObject = MakeObject<TriggerPigeon>(objectPos, "Black");

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 109026)	// ������ ���̺� ����Ʈ�� ������ �ڽ�
			{
				newObject = MakeObject<SavePointBoxTrap>(objectPos, new KillMayo());

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 109027)	// Ʈ���� ����
			{
				newObject = MakeObject<ShurikenPlatform>(objectPos);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 109028)	// ���� �÷���
			{
				Vector2D startPos = { TILE_WIDTH / 2 + (_xOriTile + 100 * i) * (TILE_WIDTH), TILE_HEIGHT / 2 + (_yOriTile) * (TILE_HEIGHT) };

				newObject = MakeObject<FallingBlockTriggerTrap>(objectPos, startPos, 1);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(4);

				AddGameObject(newObject);
			}
			else if (iter->ID == 109029)
			{
				Vector2D startPos = { TILE_WIDTH / 2 + (_xOriTile + 100 * i) * (TILE_WIDTH), TILE_HEIGHT / 2 + (_yOriTile) * (TILE_HEIGHT) };

				newObject = MakeObject<FallingBlockTriggerTrap>(objectPos, startPos, 3);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(4);

				AddGameObject(newObject);
			}
			// Falling Hamburger�� ��ü
			else if (iter->ID == 10104001)
			{
				newObject = MakeObject<FallingEnemy>(objectPos);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}

			// ---------------- Stage 2 --------------------
			// Tiles
			else if ((iter->ID == 20101001) || (iter->ID == 20101002) || (iter->ID == 20101003) || (iter->ID == 20101004) ||
				(iter->ID == 20101005) || (iter->ID == 20101006) || (iter->ID == 20101007) || (iter->ID == 20101008) || (iter->ID == 20101009) ||
				(iter->ID == 20101010) || (iter->ID == 20101011) || (iter->ID == 20101012) || (iter->ID == 20101013) || (iter->ID == 20101014) ||
				(iter->ID == 20101015) || (iter->ID == 20101016) || (iter->ID == 20101017) || (iter->ID == 20101018) || (iter->ID == 20101019) ||
				(iter->ID == 20101020) || (iter->ID == 20101021) || (iter->ID == 20101022) || (iter->ID == 20101023) || (iter->ID == 20101024) ||
				(iter->ID == 20101025) || (iter->ID == 20101026) || (iter->ID == 20101027))
			{
				newObject = MakeObject<BlockBase>(objectPos);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 109002)	// �κ�����ڽ� 2 (2�������� �ڽ� ������ �װɷ�)
			{
				newObject = MakeObject<TransBox>(objectPos);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter, Vector2D(320.f, 0.f)));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				newObject->GetSpriteRenderer()->SetOpacity(0.f);

				AddGameObject(newObject);
			}
			else if (iter->ID == 202001)	// Door Open
			{
				newObject = MakeObject<DecoObject>(objectPos);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 202002)	// Door Close
			{
				/*newObject = MakeObject<Portal>(objectPos);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);*/
			}
			else if (iter->ID == 202003)	// ��ġ������
			{
				newObject = MakeObject<DecoObject>(objectPos);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 202004)	// ����Ŭ
			{
				newObject = MakeObject<DecoObject>(objectPos);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 202005)	// ���̽��ӽ�	
			{
				newObject = MakeObject<DecoObject>(objectPos);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 202006)	// WalkPad Left
			{
				Vector2D startPos = { TILE_WIDTH / 2 + (_xOriTile + 100 * i) * (TILE_WIDTH), TILE_HEIGHT / 2 + (_yOriTile) * (TILE_HEIGHT) + 5 };

				for (int i = 0; i < 4; i++)
				{
					BlockBase* block = new BlockBase();

					block->GetTransform()->SetPosition(startPos + Vector2D(i * TILE_WIDTH, 0.f));

					Game::GetInstance().GetCurrentScene()->AddGameObject(block);
				}

				newObject = MakeObject<WalkPad>(objectPos, Vector2D(-1.f, 0.f));

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				dynamic_cast<AABBCollision*>(newObject->GetCollision())->SetSize({ TILE_WIDTH * 4, TILE_HEIGHT });

				AddGameObject(newObject);
			}
			else if (iter->ID == 202007)	// WalkPad Right
			{
				Vector2D startPos = { TILE_WIDTH / 2 + (_xOriTile + 100 * i) * (TILE_WIDTH), TILE_HEIGHT / 2 + (_yOriTile) * (TILE_HEIGHT) + 5 };

				for (int i = 0; i < 4; i++)
				{
					BlockBase* block = new BlockBase();

					block->GetTransform()->SetPosition(startPos + Vector2D(i * TILE_WIDTH, 0.f));

					Game::GetInstance().GetCurrentScene()->AddGameObject(block);
				}

				newObject = MakeObject<WalkPad>(objectPos, Vector2D(1.f, 0.f));

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				dynamic_cast<AABBCollision*>(newObject->GetCollision())->SetSize({ TILE_WIDTH * 4, TILE_HEIGHT });

				AddGameObject(newObject);
			}
			else if (iter->ID == 202008)	// ������
			{
				newObject = MakeObject<DecoObject>(objectPos);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 202009)	// �����
			{
				newObject = MakeObject<FreeEnemy>(objectPos + 200.f * Vector2D::Up, "SandBag");

				//newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 202011)	// ���� ����
			{
				newObject = MakeObject<FreeEnemy>(objectPos + 200.f * Vector2D::Up, "Dum");

				AddGameObject(newObject);
			}
			else if (iter->ID == 202012)	// �ϵ� ����
			{
				newObject = MakeObject<DecoObject>(objectPos);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 202013)	// �ϵ� ���� ����
			{
				newObject = MakeObject<DecoObject>(objectPos);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 202014)	// �ϵ���� ss
			{
				newObject = MakeObject<DecoObject>(objectPos);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 202015)	// �ϵ���� sl
			{
				newObject = MakeObject<DecoObject>(objectPos);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 202016)	// ���� 3
			{
				Vector2D startPos = { TILE_WIDTH / 2 + (_xOriTile + 100 * i) * (TILE_WIDTH), TILE_HEIGHT / 2 + (_yOriTile) * (TILE_HEIGHT) };

				newObject = MakeObject<UDMovingPlatform>(objectPos, startPos, 5, 3, idleTimeControl, "Block");

				idleTimeControl += TILE_HEIGHT * 5 / UD_MOVING_SPEED;

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				AddGameObject(newObject);
			}
			else if (iter->ID == 202017)	// ���� 4
			{
				Vector2D startPos = { TILE_WIDTH / 2 + (_xOriTile + 100 * i) * (TILE_WIDTH), TILE_HEIGHT / 2 + (_yOriTile) * (TILE_HEIGHT) };

				newObject = MakeObject<LRMovingPlatform>(objectPos, startPos, 7, 4, 0.f);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				AddGameObject(newObject);
			}
			else if (iter->ID == 202018)	// ����� ���� (���� ������)
			{
				newObject = MakeObject<OneWayMob>(objectPos + 200.f * Vector2D::Up, "SandBag", 300.f);

				AddGameObject(newObject);
			}
			else if (iter->ID == 202019)	// ����� ���� (���� ������, �����Ǵ� ��)
			{
				newObject = MakeObject<OneWayMobTrigger>(objectPos, "SandBag", 300.f, Vector2D(1.f, 0.f));

				AddGameObject(newObject);
			}
			else if (iter->ID == 202020)	// ���� ���� (���� ������)
			{
				newObject = MakeObject<OneWayMob>(objectPos + 200.f * Vector2D::Up, "Dum", 300.f);

				AddGameObject(newObject);
			}
			else if (iter->ID == 203001)	// ������ �ڽ� - ������ 2
			{
				Food* reserve = new Food(-0.5f, 20106, Vector2D(240.f, 80.f), true);

				reserve->GetTransform()->SetPosition(objectPos);

				newObject = MakeObject<ItemBox>(objectPos, reserve, 2);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 203002)	// ��
			{
				newObject = MakeObject<BreakableBlock>(objectPos);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 203003)	// �ֺ� �ڽ�
			{
				newObject = MakeObject<ItemBox>(objectPos, nullptr, 2);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 203004)	// ������ �ڽ� - Ĺ
			{
				CatBoxTrap* trap = new CatBoxTrap();

				// TODO: �ִϸ��̼ǵ� �ִ�
				newObject = MakeObject<ItemBox>(objectPos, trap, 2);

				trap->GetTransform()->SetPosition(objectPos + Vector2D(-40.f, 0.f));

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 203005)	// ������ �ڽ� - Rotten
			{
				Food* reserve = new Food(-0.5f, 20106, Vector2D(240.f, 80.f), true);

				reserve->GetTransform()->SetPosition(objectPos);

				newObject = MakeObject<BigBarTrap>(objectPos, reserve);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 203009)	// ���� ����
			{
				Vector2D startPos = { TILE_WIDTH / 2 + (_xOriTile + 100 * i) * (TILE_WIDTH), TILE_HEIGHT / 2 + (_yOriTile) * (TILE_HEIGHT) };

				newObject = MakeObject<DecoObject>(objectPos);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);

				TriggerTrap* trigger = new TriggerTrap();

				trigger->GetTransform()->SetPosition(startPos);

				Game::GetInstance().GetCurrentScene()->AddGameObject(trigger);
			}
			else if (iter->ID == 203011)	// ItemBox - Door Open
			{
				newObject = MakeObject<DoorOpen>(objectPos);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				std::vector<TriggerTrap*> traps = dynamic_cast<DoorOpen*>(newObject)->GetTriggerTraps();

				traps[0]->GetTransform()->SetPosition(newObject->GetTransform()->GetPosition() - Vector2D(0.f, TILE_HEIGHT * 5));

				Game::GetInstance().GetCurrentScene()->AddGameObject(traps[0]);

				traps[1]->GetTransform()->SetPosition(newObject->GetTransform()->GetPosition() - Vector2D(0.f, TILE_HEIGHT * 4));

				traps[1]->GetSpriteRenderer()->SetSprite(new Sprite(20115, Vector2D(0.25f, 0.5f), { 160.f, 385.f }, 0.f, Vector2D(800.f, 15.f)));

				traps[1]->GetSpriteRenderer()->SetSortOrder(6);

				Game::GetInstance().GetCurrentScene()->AddGameObject(traps[1]);

				traps[2]->GetTransform()->SetPosition(newObject->GetTransform()->GetPosition() - Vector2D(0.f, TILE_HEIGHT * 3));

				Game::GetInstance().GetCurrentScene()->AddGameObject(traps[2]);

				Game::GetInstance().GetCurrentScene()->AddGameObject(newObject);
			}
			else if (iter->ID == 205001)	// ����, ������ ���� ���.
			{
				newObject = MakeObject<Food>(objectPos, 1.0f, iter->imageID, Vector2D(iter->imageRect.left, iter->imageRect.top), false);

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 206001)	// �ϵ��
			{
				newObject = MakeObject<Niddle>(objectPos + Vector2D(0.f, TILE_HEIGHT), CollisionDir::UP);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(4);

				AddGameObject(newObject);
			}
			else if (iter->ID == 206002)	// ĳƲ�� ����
			{
				newObject = MakeObject<FallingTriggerTrap>(objectPos, 2800.f);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 206003)	// ���� ���� ..
			{
				newObject = MakeObject<RisingTrap>(objectPos);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

				AddGameObject(newObject);
			}
			else if (iter->ID == 206004)		// �ϵ� ���� �ٿ�
			{
				newObject = MakeObject<DumbellDown>(objectPos, "NiddleDown");

				AddGameObject(newObject);
			}
			else if (iter->ID == 206005)		// ���� �ٿ�
			{
				newObject = MakeObject<DumbellDown>(objectPos, "Down");

				AddGameObject(newObject);
			}
			else if (iter->ID == 206006)		// �ϵ� ����Ʈ
			{
				newObject = MakeObject<Niddle>(objectPos + Vector2D(TILE_WIDTH, 0.f), CollisionDir::LEFT);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(4);

				AddGameObject(newObject);
			}
			else if (iter->ID == 206007)		// �ϵ� ����Ʈ
			{
				newObject = MakeObject<Niddle>(objectPos - Vector2D(TILE_WIDTH, 0.f), CollisionDir::RIGHT);

				newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

				newObject->GetSpriteRenderer()->SetSortOrder(6);

				AddGameObject(newObject);
			}
			else if (iter->ID == 206008)
			{
				newObject = MakeObject<UpperEnemy>(objectPos, "SandBag");

				AddGameObject(newObject);
			}

			newObject->SetID(iter->ID);
		}

		stageOne.mTileList.clear();
	}
}

void Scene::LoadSceneDataStageThree(const string& path)
{
	MapJson stage;

	GameObject* newObject = nullptr;

	float idleTimeControl = 0.f;

	stage.DeserializeFromCArchive(path);

	for (auto iter = stage.mTileList.begin(); iter != stage.mTileList.end(); iter++)
	{
		// 1. �� ��° Ÿ�Ͽ� �̰��� ���Դ��� üũ�մϴ�.
		float _yOriTile = (iter->index >> 16);
		float _xOriTile = (iter->index & 0xffff);

		// �׷� Ÿ���� ������ ��ü�� ������ ����մϴ�.
		float _yTile = (_yOriTile)+(1.0f + (iter->group.back() >> 16)) / 2;
		float _xTile = (_xOriTile)+(1.0f + (iter->group.back() & 0xffff)) / 2;

		// �� ������ ���� ��ǥ�� �������ݴϴ�.
		Vector2D objectPos = { (_xTile)*TILE_WIDTH, _yTile * TILE_HEIGHT };

		// ------------------ Stage 3 ---------------------
		// Tiles : All Normal Block
		if ((iter->ID == 20102001) || (iter->ID == 20102002) || (iter->ID == 20102003) || (iter->ID == 20102004) ||
			(iter->ID == 20102005) || (iter->ID == 20102006) || (iter->ID == 20102007) || (iter->ID == 20102008) ||
			(iter->ID == 20102009) || (iter->ID == 20102010) || (iter->ID == 20102011) || (iter->ID == 20102012) ||
			(iter->ID == 20102013) || (iter->ID == 20102014) || (iter->ID == 20102015) || (iter->ID == 20102016) ||
			(iter->ID == 20102017) || (iter->ID == 20102018) || (iter->ID == 20102019) || (iter->ID == 20102020) ||
			(iter->ID == 20102021) || (iter->ID == 20102022) || (iter->ID == 20102023) || (iter->ID == 20102024))
		{
			newObject = MakeObject<BlockBase>(objectPos);

			newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

			newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

			AddGameObject(newObject);
		}
		else if (iter->ID == 102004)			// �ֺ� �ڽ�
		{
			newObject = MakeObject<ItemBox>(objectPos, nullptr, 1);

			newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

			newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

			AddGameObject(newObject);
		}
		else if (iter->ID == 109011)			// Ʈ���� ����
		{
			newObject = MakeObject<TriggerTrap>(objectPos);

			newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

			newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

			AddGameObject(newObject);
		}
		else if (iter->ID == 109031)		// ����¡ ���� (���ο�)
		{
			newObject = MakeObject<RedUp>(objectPos, Vector2D::Up, 1300.f);

			newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

			newObject->GetSpriteRenderer()->SetSortOrder(6);

			AddGameObject(newObject);
		}
		else if (iter->ID == 109032)		// ����¡ ���� (���)
		{
			newObject = MakeObject<RedUp>(objectPos, Vector2D::Up, 1800.f);

			newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

			newObject->GetSpriteRenderer()->SetSortOrder(6);

			AddGameObject(newObject);
		}
		else if (iter->ID == 109033)		// ����¡ ���� (�н�Ʈ)
		{
			newObject = MakeObject<RedUp>(objectPos, Vector2D::Up, 2500.f);

			newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

			newObject->GetSpriteRenderer()->SetSortOrder(6);

			AddGameObject(newObject);
		}
		else if (iter->ID == 206003)	// ���� ���� ..
		{
			newObject = MakeObject<RisingTrap>(objectPos);

			newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

			newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

			AddGameObject(newObject);
		}
		else if (iter->ID == 107001)	// ���� �÷���
		{
			newObject = MakeObject<CheckPoint>(objectPos);

			newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

			AddGameObject(newObject);
		}
		// Arrow
		else if ((iter->ID >= 210001) && (iter->ID <= 210024))
		{
			newObject = MakeObject<DecoObject>(objectPos);

			newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

			AddGameObject(newObject);
		}
		else if (iter->ID == 211001)		// ���ū ��Ʈ (��������Ʈ �̹����� ��ü�ϸ鼭 �߶�)
		{
			newObject = MakeObject<BrokenHeart>(objectPos);

			dynamic_cast<BrokenHeart*>(newObject)->mChangeSprite = new Sprite(20116, Vector2D(0.5f, 0.5f), { 160.f, 77.f }, 0.f, Vector2D(800.f, 81.f));

			newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

			newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

			AddGameObject(newObject);
		}
		else if (iter->ID == 211003)		// EXIT : �̵���Ż
		{
			newObject = MakeObject<ExitPortal>(objectPos);

			newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

			newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

			AddGameObject(newObject);
		}
		else if (iter->ID == 211004)		//  �Ͼ� ���
		{
			newObject = MakeObject<BreakableBlock>(objectPos);

			newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

			newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

			AddGameObject(newObject);
		}
		else if (iter->ID == 211005)		// ȸ�� ���
		{
			newObject = MakeObject<BreakableBlock>(objectPos);

			newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

			newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

			AddGameObject(newObject);
		}
		else if (iter->ID == 211006)		// 3ĭ¥�� / ���� 5ĭ
		{
			Vector2D startPos = { TILE_WIDTH / 2 + (_xOriTile) * (TILE_WIDTH), TILE_HEIGHT / 2 + (_yOriTile) * (TILE_HEIGHT) };

			newObject = MakeObject<LRMovingPlatform>(objectPos, startPos, 5, 3, 0.f);

			newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

			AddGameObject(newObject);
		}
		else if (iter->ID == 211007)		// 3ĭ¥�� / ���� 7ĭ
		{
			Vector2D startPos = { TILE_WIDTH / 2 + (_xOriTile) * (TILE_WIDTH), TILE_HEIGHT / 2 + (_yOriTile) * (TILE_HEIGHT) };

			newObject = MakeObject<UDMovingPlatform>(objectPos, startPos, 7, 3, 0.f, "Platform");

			newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

			AddGameObject(newObject);
		}
		else if (iter->ID == 211008)		// 3ĭ¥�� �������
		{
			Vector2D startPos = { TILE_WIDTH / 2 + (_xOriTile) * (TILE_WIDTH), TILE_HEIGHT / 2 + (_yOriTile) * (TILE_HEIGHT) };

			newObject = MakeObject<FallingBlock>(objectPos, startPos, 3);

			newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

			newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

			AddGameObject(newObject);
		}
		else if (iter->ID == 211009)		// 1ĭ¥�� �������
		{
			Vector2D startPos = { TILE_WIDTH / 2 + (_xOriTile) * (TILE_WIDTH), TILE_HEIGHT / 2 + (_yOriTile) * (TILE_HEIGHT) };

			newObject = MakeObject<FallingBlock>(objectPos, startPos, 1);

			newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

			newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

			AddGameObject(newObject);
		}
		else if (iter->ID == 211010)		// �̹����� ������ �ݶ��̴��� ����.
		{
			newObject = MakeObject<BlockBase>(objectPos);

			AddGameObject(newObject);
		}
		else if (iter->ID == 211011)		// �̹����� ����, �ݶ��̴��� ����.
		{
			newObject = MakeObject<DecoObject>(objectPos);

			newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

			newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

			AddGameObject(newObject);
		}
		else if (iter->ID == 211012)		// �̹����� ����, �ݶ��̴��� ����
		{
			newObject = MakeObject<DecoObject>(objectPos);

			newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

			newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

			AddGameObject(newObject);
		}
		else if (iter->ID == 211013)		// �÷��� ����
		{
			Vector2D startPos = { TILE_WIDTH / 2 + (_xOriTile) * (TILE_WIDTH), TILE_HEIGHT / 2 + (_yOriTile) * (TILE_HEIGHT) };

			newObject = MakeObject<FallingBlock>(objectPos, startPos, 3);

			newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

			newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

			AddGameObject(newObject);
		}
		else if (iter->ID == 211014)		// ����¡ ���� Ÿ�̴�
		{
			newObject = MakeObject<RedUp>(objectPos, Vector2D::Up, 1300.f, TILE_HEIGHT * 7);

			newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

			newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

			AddGameObject(newObject);
		}
		else if (iter->ID == 210115)		// ���̽� (�¸� ����)
		{
			newObject = MakeObject<FreeEnemy>(objectPos + 200.f * Vector2D::Up, "Face");

			AddGameObject(newObject);
		}
		else if (iter->ID == 210116)		// �� (�¸� ����)
		{
			newObject = MakeObject<FreeEnemy>(objectPos + 200.f * Vector2D::Up, "Fur");

			AddGameObject(newObject);
		}
		else if (iter->ID == 210201)		// �κ����� �ڽ� 4
		{
			newObject = MakeObject<TransBox>(objectPos);

			newObject->GetSpriteRenderer()->SetSprite(new Sprite(20104, Vector2D(0.5f, 0.5f), { 80.f, 77.f }, 0.f, Vector2D(560.f, 80.f)));

			newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

			newObject->GetSpriteRenderer()->SetOpacity(0.f);

			AddGameObject(newObject);
		}
		else if (iter->ID == 210202)		// Red Right
		{
			newObject = MakeObject<RedRight>(objectPos, Vector2D::Right, 1300.f);

			newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

			newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

			AddGameObject(newObject);
		}
		else if (iter->ID == 210203)		// �ϵ� ��
		{
			newObject = MakeObject<Niddle>(objectPos + Vector2D(0.f, TILE_HEIGHT), CollisionDir::UP);

			newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

			newObject->GetSpriteRenderer()->SetSortOrder(4);

			AddGameObject(newObject);
		}
		else if (iter->ID == 210204)		// �ϵ� ����Ʈ
		{
			newObject = MakeObject<Niddle>(objectPos + Vector2D(TILE_WIDTH, 0.f), CollisionDir::LEFT);

			newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

			newObject->GetSpriteRenderer()->SetSortOrder(4);

			AddGameObject(newObject);
		}
		else if (iter->ID == 210205)		// �ϵ� ����Ʈ
		{
			newObject = MakeObject<Niddle>(objectPos - Vector2D(TILE_WIDTH, 0.f), CollisionDir::RIGHT);

			newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

			newObject->GetSpriteRenderer()->SetSortOrder(4);

			AddGameObject(newObject);
		}
		else if (iter->ID == 210207)		// ���� ��
		{
			newObject = MakeObject<RedUp>(objectPos, Vector2D::Up, 1300.f);

			newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

			newObject->GetSpriteRenderer()->SetSortOrder(6);

			AddGameObject(newObject);
		}
		else if (iter->ID == 210209)		// ���ο� �ϵ� �ٿ�
		{
			newObject = MakeObject<SlowNiddle>(objectPos);

			newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

			newObject->GetAABBCollision()->SetSize({ TILE_WIDTH, TILE_HEIGHT });

			newObject->GetSpriteRenderer()->SetSortOrder(6);

			AddGameObject(newObject);
		}
		else if (iter->ID == 210210)		// slow niddle2x Up
		{
			newObject = MakeObject<SlowNiddle>(objectPos);

			newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

			newObject->GetAABBCollision()->SetSize({ TILE_WIDTH, 2 * TILE_HEIGHT });

			newObject->GetSpriteRenderer()->SetSortOrder(6);

			AddGameObject(newObject);
		}
		else if (iter->ID == 210212)		// ȭ��Ʈ �ٿ�
		{
			newObject = MakeObject<RedUp>(objectPos, Vector2D::Down, 1300.f);

			newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

			newObject->GetSpriteRenderer()->SetSortOrder(6);

			AddGameObject(newObject);
		}
		else if (iter->ID == 210214)
		{
			newObject = MakeObject<DecoObject>(objectPos);

			newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

			newObject->GetSpriteRenderer()->SetSortOrder(3);

			AddGameObject(newObject);
		}
		else if (iter->ID == 210215)
		{
			newObject = MakeObject<SlowNiddle>(objectPos);

			newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

			newObject->GetAABBCollision()->SetSize({ TILE_WIDTH, TILE_HEIGHT });

			newObject->GetSpriteRenderer()->SetSortOrder(6);

			AddGameObject(newObject);
		}
		else if (iter->ID == 210220)	// ����'��' ����
		{
			newObject = MakeObject<RidingNacho>(objectPos);

			newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

			newObject->GetSpriteRenderer()->SetSortOrder(6);

			AddGameObject(newObject);
		}
		else if (iter->ID == 210223)	// ����
		{
		newObject = MakeObject<TriggerDoor>(objectPos);

		newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

		newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

		AddGameObject(newObject);
		}

		// Five Falling Object
		else if ((210301 <= iter->ID) && (iter->ID <= 210320))
		{
			float dropTime = iter->ID - 210301;

			D2D1_SIZE_F size = { iter->imageRect.right - iter->imageRect.left, iter->imageRect.bottom - iter->imageRect.top };

			newObject = MakeObject<FiveFalling>(objectPos, size, 4.f + dropTime * 2.f);

			newObject->GetSpriteRenderer()->SetSprite(MakeSprite(iter));

			newObject->GetSpriteRenderer()->SetSortOrder(iter->layer);

			AddGameObject(newObject);
		}



		// newObject->SetID(iter->ID);
	}

}

void Scene::AddGameObject(GameObject* object)
{
	Collision* collision = object->GetCollision();

	Renderer* renderer = object->GetRenderer();

	mGameObjects.push_back(object);

	if (collision != nullptr)
		mCollisions.push_back(collision);

	if (renderer != nullptr)
		mRenderers.insert(renderer);
}

void Scene::AddGlobalGameObject(GameObject* object)
{
	Collision* collision = object->GetCollision();

	Renderer* renderer = object->GetRenderer();

	mGlobalGameObjects.push_back(object);

	if (collision != nullptr)
		mCollisions.push_back(collision);

	if (renderer != nullptr)
		mRenderers.insert(renderer);
}

void Scene::SubGameObject(GameObject* object)
{
	Renderer* renderer = object->GetRenderer();

	Collision* collision = object->GetCollision();

	if (renderer != nullptr)
		for (auto iter = mRenderers.begin(); iter != mRenderers.end(); iter++)
		{
			if (*iter == renderer)
			{
				mRenderers.erase(iter);

				break;
			}
		}

	if (collision != nullptr)
		for (auto iter = mCollisions.begin(); iter != mCollisions.end(); iter++)
		{
			if (*iter == collision)
			{
				mCollisions.erase(iter);

				break;
			}
		}

	for (auto iter = mGameObjects.begin(); iter != mGameObjects.end(); iter++)
	{
		if (*iter == object)
		{
			object->Release();

			delete object;

			mGameObjects.erase(iter);

			break;
		}
	}
}

/// <summary>
/// �������� ����� ������Ʈ�� ����Ͽ� �����ϱ�
/// </summary>
void Scene::SubGlobalGameObject(GameObject* object)
{
	Renderer* renderer = object->GetRenderer();

	Collision* collision = object->GetCollision();

	if (renderer != nullptr)
		for (auto iter = mRenderers.begin(); iter != mRenderers.end(); iter++)
		{
			if (*iter == renderer)
			{
				mRenderers.erase(iter);

				break;
			}
		}

	if (collision != nullptr)
		for (auto iter = mCollisions.begin(); iter != mCollisions.end(); iter++)
		{
			if (*iter == collision)
			{
				mCollisions.erase(iter);

				break;
			}
		}

	for (auto iter = mGlobalGameObjects.begin(); iter != mGlobalGameObjects.end(); iter++)
	{
		if (*iter == object)
		{
			mGlobalGameObjects.erase(iter);

			break;
		}
	}
}

std::string Scene::GetSceneName()
{
	return mSceneName;
}

void Scene::ReserveCreate(GameObject* object, float time)
{
	mCreateReserveList.push_back({ object, time });
}

void Scene::ReserveDestroy(GameObject* object, float time)
{
	mDestroyReserveList.push_back({ object, time });
}

void Scene::BirthAndDeath()
{
	float delta = Timer::GetInstance().GetDeltaTime() / 1000.f;

	// Birth
	for (auto iter = mCreateReserveList.begin(); iter != mCreateReserveList.end(); )
	{
		(*iter).second -= delta;

		if ((*iter).second <= 0.f)
		{
			AddGameObject((*iter).first);

			iter = mCreateReserveList.erase(iter);
		}
		else
			++iter;
	}

	// Death
	for (auto iter = mDestroyReserveList.begin(); iter != mDestroyReserveList.end(); )
	{
		(*iter).second -= delta;

		// ������, �ݸ���, ������Ʈ ����Ʈ���� �� �����ؾ���
		if ((*iter).second <= 0.f)
		{
			SubGameObject((*iter).first);

			iter = mDestroyReserveList.erase(iter);
		}
		else
			++iter;
	}
}