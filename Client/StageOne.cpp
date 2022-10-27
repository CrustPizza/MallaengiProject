#include "PreCompiledHeader.h"
#include "StageOne.h"

#include "Player.h"

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
#include "FallingPlatform.h"
#include "FreeEnemy.h"
#include "FallingTriggerTrap.h"
#include "Watch.h"

StageOne::StageOne() :
	Scene("StageOne")
{

}

StageOne::~StageOne()
{

}

void StageOne::BuildScene()
{
	Scene::BuildScene();

	// �� �Լ��� ������ ȣ���ϸ�, Build�� �� Fade-In�� �ǽ��Ѵ�.
	GameObject* fadeIn = new FadeInObject();

	AddGameObject(fadeIn);

	// UI ��ġ
	GameObject* bodyScale = new BodyScale();

	// ü�߰�
	AddGameObject(bodyScale);

	// �ð�
	AddGameObject(new Watch());

	// �÷��̾� ��ġ
	AddGameObject(mGamePlayer);

	// ���� ī�޶�
	MainCamera* mainCamera = new MainCamera(TILE_WIDTH * 100 * 3, TILE_HEIGHT * 30);

	AddGameObject(mainCamera);

	// ��� ȭ��
	GameObject* background = new StageOneBackGround();

	SpriteRenderer* sprite = dynamic_cast<SpriteRenderer*>(background->GetRenderer());

	sprite->SetSprite(Game::GetInstance().GetResource()->GetSprite("STAGE_1_BACKGROUND"));

	sprite->SetSortOrder(10);

	AddGameObject(background);

	LoadSceneData(_T("Resources\\Json\\Stage 1-%d.json"), 3);

	SoundManager::GetInstance()->Play(500002);

	// �÷��� ������������ ù ��°�� �ε�� ���� ����Ʈ ������ �����Ѵ�.
	if (mIsFirstLoad)
	{
		// �������� ���� ��ŸƮ�ϴ� ��ġ (�� �������� ���ӿ� �����ؼ� Stage Restart ��� ���� ?)
		mGamePlayer->GetTransform()->SetPosition(Vector2D(100.f, 1760.f));

		//mGamePlayer->GetTransform()->SetPosition(Vector2D(23000.f, 160.f));

		Game::GetInstance().SetSaveBodyWeight(mGamePlayer->GetBodyWeight());

		Game::GetInstance().SetSavePosition(mGamePlayer->GetTransform()->GetPosition());

		mIsFirstLoad = false;
	}
	else
		// ������ ���� �� �Ǵ� Restart�� ������ j..�ٽ� �¾�ٴ� �Ҹ��� ?
	{
		// IDLE + STEP + �ִϸ��̼� ������ �ٲ���� ���� �Ǳ� �� ..
		float saveBodyWeight = Game::GetInstance().GetSaveBodyWeight();

		if (saveBodyWeight < 100.f)
			mGamePlayer->SetPlayerStep(PlayerStep::One);
		else if ((saveBodyWeight >= 100.f) && (saveBodyWeight < 120.f))
			mGamePlayer->SetPlayerStep(PlayerStep::Two);
		else if ((saveBodyWeight >= 120) && (saveBodyWeight < 140.f))
			mGamePlayer->SetPlayerStep(PlayerStep::Three);

		mGamePlayer->SetPlayerState(PlayerState::Idle);
		mGamePlayer->mRunEffect = nullptr;
		mGamePlayer->mLandingEffect = nullptr;

		std::vector<std::vector<AnimationClip*>> clips = mGamePlayer->GetAnimationClipsOfPlayer();

		mGamePlayer->GetAnimator()->SetAnimationClip(clips[static_cast<int>(mGamePlayer->GetCurrentStep())][static_cast<int>(mGamePlayer->GetCurrentState())]);

		// ù ��° �ε尡 �ƴ϶�� ����� ������ �÷��̾ �����Ѵ�. (Save Point ����)
		mGamePlayer->GetTransform()->SetPosition(Game::GetInstance().GetSavePosition());
		mGamePlayer->SetBodyWeight(saveBodyWeight);
		mGamePlayer->SetHp(1);
	}

	// ĳ���� ��
	GameObject* faceUI = new DecoObject();

	faceUI->GetTransform()->SetPosition(Vector2D(50, 50));

	faceUI->GetSpriteRenderer()->SetSprite(Game::GetInstance().GetResource()->GetSprite("LIFE_FACE"));

	faceUI->GetSpriteRenderer()->SetSortOrder(1);		// �� �ڷ� ���� ..

	faceUI->SetTag("UI");

	AddGameObject(faceUI);

	if (mGamePlayer->GetPlayerLife() < 0)
	{
		mGamePlayer->SetHp(100);
	}

	// �������� �޾ƿ´�. �� ���ڿ� �´� ������Ʈ���� ��ġ�ؼ� ��������� ..!
	std::string life = std::to_string(mGamePlayer->GetPlayerLife());

	// ���
	if (life[0] != '-')
	{
		for (int i = 0; i < life.length(); i++)
		{
			GameObject* number = new DecoObject();

			if (life[i] == '0')
				number->GetSpriteRenderer()->SetSprite(Game::GetInstance().GetResource()->GetSprite("WATCH_TIME_ZERO"));
			else if (life[i] == '1')
				number->GetSpriteRenderer()->SetSprite(Game::GetInstance().GetResource()->GetSprite("WATCH_TIME_ONE"));
			else if (life[i] == '2')
				number->GetSpriteRenderer()->SetSprite(Game::GetInstance().GetResource()->GetSprite("WATCH_TIME_TWO"));
			else if (life[i] == '3')
				number->GetSpriteRenderer()->SetSprite(Game::GetInstance().GetResource()->GetSprite("WATCH_TIME_THREE"));
			else if (life[i] == '4')
				number->GetSpriteRenderer()->SetSprite(Game::GetInstance().GetResource()->GetSprite("WATCH_TIME_FOUR"));
			else if (life[i] == '5')
				number->GetSpriteRenderer()->SetSprite(Game::GetInstance().GetResource()->GetSprite("WATCH_TIME_FIVE"));
			else if (life[i] == '6')
				number->GetSpriteRenderer()->SetSprite(Game::GetInstance().GetResource()->GetSprite("WATCH_TIME_SIX"));
			else if (life[i] == '7')
				number->GetSpriteRenderer()->SetSprite(Game::GetInstance().GetResource()->GetSprite("WATCH_TIME_SEVEN"));
			else if (life[i] == '8')
				number->GetSpriteRenderer()->SetSprite(Game::GetInstance().GetResource()->GetSprite("WATCH_TIME_EIGHT"));
			else if (life[i] == '9')
				number->GetSpriteRenderer()->SetSprite(Game::GetInstance().GetResource()->GetSprite("WATCH_TIME_NINE"));

			number->GetTransform()->SetPosition(Vector2D(i * 20.f + 115.0f, 55.f));

			number->GetSpriteRenderer()->SetSortOrder(1);

			number->SetTag("UI");

			AddGameObject(number);
		}
	}
}