#include "PreCompiledHeader.h"
#include "CometScene.h"

#include "Player.h"
#include "StageOneBackGround.h"
#include "BreakableBlock.h"
#include "CheckPoint.h"
#include "MainCamera.h"

#include "Platform.h"
#include "DecoObject.h"

#include "JsonManager.h"
#include "TriggerTrap.h"

// CometScene�� Size
#define COMET_SCENE_WIDTH 3840
#define COMET_SCENE_HEIGHT 2160

CometScene::CometScene() :
	Scene("CometScene")
{

}

CometScene::~CometScene()
{

}

void CometScene::BuildScene()
{
	Scene::BuildScene();

	// �� ���� �÷��̾ �ʿ��մϴ�.
	AddGameObject(mGamePlayer);

	// �� ���� Play�� �̷������ ���̹Ƿ� ���� ī�޶� ����մϴ�.
	AddGameObject(new MainCamera());

	GameObject* background = new StageOneBackGround();

	SpriteRenderer* sprite = dynamic_cast<SpriteRenderer*>(background->GetRenderer());

	sprite->SetSprite(Game::GetInstance().GetResource()->GetSprite("STAGE_1_BACKGROUND"));

	sprite->SetSortOrder(10);

	AddGameObject(background);

	// ���� ������ �о���� ��ġ�ϱ�.
	MapJson cometSceneJson;

	cometSceneJson.DeserializeFromFile("Json\\CometScene.json");

	for (auto iter = cometSceneJson.mTileList.begin(); iter != cometSceneJson.mTileList.end(); iter++)
	{
		// 1. �� ��° Ÿ�Ͽ� �̰��� ���Դ��� üũ�մϴ�.
		int _yTile = iter->index >> 16;
		int _xTile = iter->index & 0xffff;

		// 2. ID ���� �з��Ͽ� ���ǿ� �´� ������Ʈ�� ��Ȯ�� ��ġ�� ��ġ�մϴ�.
		if (iter->ID == 102004) // Block
		{
			BreakableBlock* block = new BreakableBlock();

			block->GetTransform()->SetPosition(Vector2D(TILE_WIDTH / 2 + _xTile * TILE_WIDTH, TILE_HEIGHT / 2 + _yTile * TILE_HEIGHT));

			D2D1_RECT_F rect = iter->imageRect;

			Sprite* sprite = new Sprite(iter->imageID, Vector2D(0.5f, 0.5f),
				{ rect.right - rect.left, rect.bottom - rect.top }, 0.f, Vector2D(rect.left, rect.top));

			block->GetSpriteRenderer()->SetSprite(sprite);

			AddGameObject(block);
		}
		// Ÿ�� (�ܵ� �ִ�, �ö� �� �ִ� ���̴�)
		else if ((iter->ID == 10101001) || (iter->ID == 10101002) || (iter->ID == 10101003))
		{
			Platform* platform = new Platform();

			platform->GetTransform()->SetPosition(Vector2D(TILE_WIDTH / 2 + _xTile * TILE_WIDTH, TILE_HEIGHT / 2 + _yTile * TILE_HEIGHT));

			D2D1_RECT_F rect = iter->imageRect;

			Sprite* sprite = new Sprite(iter->imageID, Vector2D(0.5f, 0.5f),
				{ rect.right - rect.left, rect.bottom - rect.top }, 0.f, Vector2D(rect.left, rect.top));

			platform->GetSpriteRenderer()->SetSprite(sprite);

			AddGameObject(platform);
		}
		// Ÿ�� (�ܵ� ����, �ö� �� ���� ���̴�)
		else if ((iter->ID == 10101004) || (iter->ID == 10101005) || (iter->ID == 10101006)
			|| (iter->ID == 10101007) || (iter->ID == 10101008) || (iter->ID == 10101009))
		{
			DecoObject* deco = new DecoObject();

			deco->GetTransform()->SetPosition(Vector2D(TILE_WIDTH / 2 + _xTile * TILE_WIDTH, TILE_HEIGHT / 2 + _yTile * TILE_HEIGHT));

			D2D1_RECT_F rect = iter->imageRect;

			Sprite* sprite = new Sprite(iter->imageID, Vector2D(0.5f, 0.5f),
				{ rect.right - rect.left, rect.bottom - rect.top }, 0.f, Vector2D(rect.left, rect.top));

			deco->GetSpriteRenderer()->SetSprite(sprite);

			AddGameObject(deco);
		}
		// üũ ����Ʈ (�ϴ� ���ڷ� �صӴϴ�)
		else if (iter->ID == 103001)
		{
			CheckPoint* deco = new CheckPoint();

			deco->GetTransform()->SetPosition(Vector2D(TILE_WIDTH / 2 + _xTile * TILE_WIDTH, TILE_HEIGHT / 2 + _yTile * TILE_HEIGHT));

			D2D1_RECT_F rect = iter->imageRect;

			Sprite* sprite = new Sprite(iter->imageID, Vector2D(0.5f, 0.5f),
				{ rect.right - rect.left, rect.bottom - rect.top }, 0.f, Vector2D(rect.left, rect.top));

			deco->GetSpriteRenderer()->SetSprite(sprite);

			AddGameObject(deco);
		}
		// ������� (�ϴ� ���ڷ� �صӴϴ�)
		else if (iter->ID == 107002)
		{
			// ��� ������ ���η� 3ĭ, ������ 5ĭ
			DecoObject* deco = new DecoObject();

			// ��� ���� �Ʒ��� �÷����� �ʾƼ� 10 ���� �� ������
			deco->GetTransform()->SetPosition(Vector2D(TILE_WIDTH / 2 + (_xTile + 1) * TILE_WIDTH, TILE_HEIGHT / 2 + (_yTile + 2) * TILE_HEIGHT + 10.f));

			D2D1_RECT_F rect = iter->imageRect;

			Sprite* sprite = new Sprite(iter->imageID, Vector2D(0.5f, 0.5f),
				{ rect.right - rect.left, rect.bottom - rect.top }, 0.f, Vector2D(rect.left, rect.top));

			deco->GetSpriteRenderer()->SetSprite(sprite);

			AddGameObject(deco);
		}
		// ���Ǵ�
		else if (iter->ID == 105001)
		{
			DecoObject* deco = new DecoObject();

			deco->GetTransform()->SetPosition(Vector2D(TILE_WIDTH / 2 + (_xTile + 1) * TILE_WIDTH, TILE_HEIGHT + (_yTile + 1) * TILE_HEIGHT));

			D2D1_RECT_F rect = iter->imageRect;

			Sprite* sprite = new Sprite(iter->imageID, Vector2D(0.5f, 0.5f),
				{ rect.right - rect.left, rect.bottom - rect.top }, 0.f, Vector2D(rect.left, rect.top));

			deco->GetSpriteRenderer()->SetSprite(sprite);

			AddGameObject(deco);
		}
		// Item-Box (�ϴ� �극��Ŀ�� ������� .. Ŭ������ ������Ʈ���� �� �ľ߰ڴ� ..)
		else if (iter->ID == 102001)
		{
			BreakableBlock* block = new BreakableBlock();

			block->GetTransform()->SetPosition(Vector2D(TILE_WIDTH / 2 + _xTile * TILE_WIDTH, TILE_HEIGHT / 2 + _yTile * TILE_HEIGHT));

			D2D1_RECT_F rect = iter->imageRect;

			Sprite* sprite = new Sprite(iter->imageID, Vector2D(0.5f, 0.5f),
				{ rect.right - rect.left, rect.bottom - rect.top }, 0.f, Vector2D(rect.left, rect.top));

			block->GetSpriteRenderer()->SetSprite(sprite);

			AddGameObject(block);
		}
		// Bush
		else if (iter->ID == 107001)
		{
			DecoObject* deco = new DecoObject();

			deco->GetTransform()->SetPosition(Vector2D(TILE_WIDTH + (_xTile) * TILE_WIDTH, TILE_HEIGHT + (_yTile) * TILE_HEIGHT));

			D2D1_RECT_F rect = iter->imageRect;

			Sprite* sprite = new Sprite(iter->imageID, Vector2D(0.5f, 0.5f),
				{ rect.right - rect.left, rect.bottom - rect.top }, 0.f, Vector2D(rect.left, rect.top));

			deco->GetSpriteRenderer()->SetSprite(sprite);

			AddGameObject(deco);
		}
		// Donut
		else if (iter->ID == 104001)
		{
			DecoObject* deco = new DecoObject();

			deco->GetTransform()->SetPosition(Vector2D(TILE_WIDTH / 2 + (_xTile)*TILE_WIDTH, TILE_HEIGHT / 2 + (_yTile)*TILE_HEIGHT));

			D2D1_RECT_F rect = iter->imageRect;

			Sprite* sprite = new Sprite(iter->imageID, Vector2D(0.5f, 0.5f),
				{ rect.right - rect.left, rect.bottom - rect.top }, 0.f, Vector2D(rect.left, rect.top));

			deco->GetSpriteRenderer()->SetSprite(sprite);

			AddGameObject(deco);
		}
	}

	// �������� ���� ��ġ�� �÷��̾� ����
	mGamePlayer->GetTransform()->SetPosition(Vector2D(0.f, 1400.f));

	TriggerTrap* trigger = new TriggerTrap();

	trigger->GetTransform()->SetPosition(Vector2D(200.f, 1700.f));

	AddGameObject(trigger);
}