#include "PreCompiledHeader.h"
#include "StageThree2.h"

#include "FadeInObject.h"
#include "IntroBackGround.h"
#include "Player.h"
#include "SubCamera.h"
#include "DecoObject.h"

StageThree2::StageThree2():
Scene("StageThreeTwo")
{
}

StageThree2::~StageThree2()
{
}

void StageThree2::BuildScene()
{
	Scene::BuildScene();

	// ����
	while (SoundManager::GetInstance()->Playing(500004) != true)
		SoundManager::GetInstance()->Play(500004);

	// Fade-In
	AddGameObject(new FadeInObject());

	AddGameObject(mGamePlayer);

	// �� ���� ���� ī�޶� �ʿ��մϴ�. (��ġ ����, ��ũ�� X)
	AddGameObject(new SubCamera());

	// ��� ȭ��
	GameObject* background = new IntroBackGround();

	background->GetSpriteRenderer()->SetSprite(Game::GetInstance().GetResource()->GetSprite("STAGE_3_BACKGROUND"));

	background->GetSpriteRenderer()->SetSortOrder(10);

	background->GetTransform()->SetPosition(Vector2D(1400.f, 500.f));

	AddGameObject(background);

	LoadSceneDataStageThree(_T("Resources\\Json\\Stage 3-2.json"));

	// �÷��� ������������ ù ��°�� �ε�� ���� ����Ʈ ������ �����Ѵ�.
	if (mIsFirstLoad)
	{
		mGamePlayer->GetTransform()->SetPosition(Vector2D(700.f, 100.f));				// �������� 3�� ��ȹ�ںе��� ���ϴ� ��ġ�� ����.

		Game::GetInstance().SetSavePosition(mGamePlayer->GetTransform()->GetPosition());

		// �������� 3������ ������ ���� / 2�ܰ� fix
		float fixedBodyWeight = 105.f;

		if (fixedBodyWeight < 100.f)
			mGamePlayer->SetPlayerStep(PlayerStep::One);
		else if ((fixedBodyWeight >= 100.f) && (fixedBodyWeight < 120.f))
			mGamePlayer->SetPlayerStep(PlayerStep::Two);
		else if ((fixedBodyWeight >= 120) && (fixedBodyWeight < 140.f))
			mGamePlayer->SetPlayerStep(PlayerStep::Three);

		mGamePlayer->SetPlayerState(PlayerState::Idle);

		std::vector<std::vector<AnimationClip*>> clips = mGamePlayer->GetAnimationClipsOfPlayer();

		mGamePlayer->GetAnimator()->SetAnimationClip(clips[static_cast<int>(mGamePlayer->GetCurrentStep())][static_cast<int>(mGamePlayer->GetCurrentState())]);

		// ù ��° �ε尡 �ƴ϶�� ����� ������ �÷��̾ �����Ѵ�. (Save Point ����)
		mGamePlayer->GetTransform()->SetPosition(Game::GetInstance().GetSavePosition());
		mGamePlayer->SetBodyWeight(fixedBodyWeight);
		mGamePlayer->SetHp(1);

		mIsFirstLoad = false;
	}
	else
	{
		// �������� 3������ ������ ���� / 2�ܰ� fix
		float fixedBodyWeight = 105.f;

		if (fixedBodyWeight < 100.f)
			mGamePlayer->SetPlayerStep(PlayerStep::One);
		else if ((fixedBodyWeight >= 100.f) && (fixedBodyWeight < 120.f))
			mGamePlayer->SetPlayerStep(PlayerStep::Two);
		else if ((fixedBodyWeight >= 120) && (fixedBodyWeight < 140.f))
			mGamePlayer->SetPlayerStep(PlayerStep::Three);

		mGamePlayer->SetPlayerState(PlayerState::Idle);

		std::vector<std::vector<AnimationClip*>> clips = mGamePlayer->GetAnimationClipsOfPlayer();

		mGamePlayer->GetAnimator()->SetAnimationClip(clips[static_cast<int>(mGamePlayer->GetCurrentStep())][static_cast<int>(mGamePlayer->GetCurrentState())]);

		// ù ��° �ε尡 �ƴ϶�� ����� ������ �÷��̾ �����Ѵ�. (Save Point ����)
		mGamePlayer->GetTransform()->SetPosition(Game::GetInstance().GetSavePosition());
		mGamePlayer->SetBodyWeight(fixedBodyWeight);
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
