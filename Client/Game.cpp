#include "PreCompiledHeader.h"
#include "Game.h"

// Scenes
//#include "CometScene.h"
#include "DieScene.h"
#include "Intro.h"
#include "StageOne.h"
#include "LoadingScene.h"

// ������
#include "Stage1OpeningScene.h"
#include "Stage2OpeningScene.h"
#include "Stage3OpeningScene.h"

// ����
#include "EndingSceneOne.h"
#include "EndingSceneTwo.h"

#include "Player.h"
#include "StageTwo.h"
#include "StageThree1.h"

#include "ProgressBar.h"
#include "Exit.h"
#include "Title.h"
#include "Back.h"
#include "Restart.h"
#include "StageThree2.h"
#include "StageThree3.h"
#include "StageThree4.h"
#include "StageThree5.h"
#include "UIDecoObject.h"

#include "FadeOutObject.h"

Game::Game() :
	mScenes(std::map<std::string, Scene*>()),
	mSaveBodyWeight(0.f),
	mSavePosition(Vector2D(0.f, 0.f)),
	mCurrentScene(nullptr),
	mIsReservedLoadScene(false),
	mIsDebugMode(false),
	mIsEndGame(false),
	mResource(nullptr),
	mLastBodyWeight(0.f)
{

}

Game::~Game()
{
	mCurrentScene->ClearScene();

	for (auto iter : mScenes)
	{
		if (iter.second != nullptr)
			delete iter.second;
	}

	for (auto iter : mMenuList)
	{
		if (iter != nullptr)
			delete iter;
	}

	if (mGamePlayer != nullptr)
		delete mGamePlayer;

	if (mResource != nullptr)
		delete mResource;
}

void Game::Initialize(HWND hWnd)
{
	// Engine Initialize
	Engine::GetInstance().Initialize(hWnd);

	// Resource Initialize
	mResource = new Resource();
	mResource->Initialize();

	// �÷��̾ �������ݴϴ�. �̰��� ���� ������ ���������� ����� ��ȹ�Դϴ�.
	mGamePlayer = new Player();

	// �غ��� Scene Class���� �����ϰ� �������ش�.

	// Intro
	RegisterScene(new Intro());

	// Opening
	RegisterScene(new Stage1OpeningScene());
	RegisterScene(new Stage2OpeningScene());
	RegisterScene(new Stage3OpeningScene());

	// Stages
	RegisterScene(new StageOne());
	RegisterScene(new StageTwo());
	RegisterScene(new StageThree1());
	RegisterScene(new StageThree2());
	RegisterScene(new StageThree3());
	RegisterScene(new StageThree4());
	RegisterScene(new StageThree5());

	// DieScene // �׾��� �� �ҷ��;��ϴ� ���� �����ִ� ��
	RegisterScene(new DieScene("StageOne"));
	RegisterScene(new DieScene("StageTwo"));
	RegisterScene(new DieScene("StageThreeOne"));
	RegisterScene(new DieScene("StageThreeTwo"));
	RegisterScene(new DieScene("StageThreeThree"));
	RegisterScene(new DieScene("StageThreeFour"));
	RegisterScene(new DieScene("StageThreeFive"));

	// Loading Scene (Narative ..?)
	RegisterScene(new LoadingScene("Intro"));
	RegisterScene(new LoadingScene("StageOneOpening"));
	RegisterScene(new LoadingScene("StageTwoOpening"));
	RegisterScene(new LoadingScene("StageThreeOneOpening"));
	RegisterScene(new LoadingScene("OneEnding"));
	RegisterScene(new LoadingScene("TwoEnding"));
	RegisterScene(new LoadingScene("StageOne"));	// Name = IntroLoadingScene
	RegisterScene(new LoadingScene("StageTwo"));	// Name = IntroLoadingScene
	RegisterScene(new LoadingScene("StageThreeOne"));	// Name = IntroLoadingScene

	// Ending Scene
	RegisterScene(new EndingSceneOne());
	RegisterScene(new EndingSceneTwo());

	// �� ���Ĵ� �ε����� ����.

	//// soundManager Initialize
	SoundManager::GetInstance()->Initialize();
	SoundManager::GetInstance()->LoadJsonAddSound();

	ReserveLoadScene("Intro");

	// Intro Scene �ε� �� �غ�
	LoadScene();

	// �ɼ� �ʱ�ȭ
	InitOption();
}

bool Game::GameLoop()
{
	SoundManager::GetInstance()->Update();

	

	std::string name = mCurrentScene->GetSceneName();

	// �������� �÷��� �� ESC ��ư�� ������ Optionâ On / Off
	if ((Input::GetInstance().GetKeyState(VK_ESCAPE) == KEYSTATE::DOWN))
	{
		// ���ڿ��� Ư�� �κ��� �߶󳻿� ���� ������ �����Ѵ�.
		// ���� Ÿ�� ������ �־����� �� �������� ������ ���߿�..
		string str = name;

		if (str.length() >= 7)
			str = str.substr(str.length() - 7);

		if (str == "Opening")
		{
			str = name;

			str = str.substr(0, str.length() - 7) + "LoadingScene";

			FadeOutObject* fadeOut = new FadeOutObject(str);

			Game::GetInstance().GetCurrentScene()->AddGameObject(fadeOut);
		}
		else
		{
			str = name;

			if (str.length() >= 6)
				str = str.substr(str.length() - 6);

			if (str == "Ending")
			{
				FadeOutObject* fadeOut = new FadeOutObject("IntroLoadingScene");

				Game::GetInstance().GetCurrentScene()->AddGameObject(fadeOut);
			}
			else
			{
				str = name;

				str = str.substr(0, 5);

				if ((str == "Intro") || (str == "Stage"))
					OptionToggle();
			}
		}
	}

	// DieScene�� �� Ű �Է��� ���� �� �ְ���
	if (Input::GetInstance().GetKeyState(VK_RETURN) == KEYSTATE::DOWN)
	{
		string str = name;

		if (str.length() >= 8)
			str = str.substr(str.length() - 8);

		if (str == "DieScene")
		{
			str = name;

			str = str.substr(0, str.length() - 8);

			FadeOutObject* fadeOut = new FadeOutObject(str);

			Game::GetInstance().GetCurrentScene()->AddGameObject(fadeOut);
		}
	}

	if (mIsOptionClicked)
		// �ɼ��� Ŭ���Ǿ����� OffOption�ؾ���.
	{
		OffOption();

		//sound 
		/*SoundManager::GetInstance()->Resume();*/

		mIsOptionOn = false;

		mIsOptionClicked = false;
	}

#ifdef _DEBUG || DEBUG
	if (Input::GetInstance().GetKeyState(VK_F5) == KEYSTATE::DOWN)
	{
		mIsDebugMode = !mIsDebugMode;
	}
	else if (Input::GetInstance().GetKeyState(VK_F6) == KEYSTATE::DOWN)
	{
		ReserveLoadScene("StageOne");

		int bgmNumber = SoundManager::GetInstance()->GetCurrentPlayBGM();

		if (bgmNumber != -1)
			SoundManager::GetInstance()->Stop(bgmNumber);
	}
	else if (Input::GetInstance().GetKeyState(VK_F7) == KEYSTATE::DOWN)
	{
		ReserveLoadScene("StageTwo");

		int bgmNumber = SoundManager::GetInstance()->GetCurrentPlayBGM();

		if (bgmNumber != -1)
			SoundManager::GetInstance()->Stop(bgmNumber);
	}
	else if (Input::GetInstance().GetKeyState(VK_F8) == KEYSTATE::DOWN)
	{
		std::string scenename = Game::GetInstance().GetCurrentScene()->GetSceneName();

		if (scenename == "StageThreeOne")
			ReserveLoadScene("StageThreeTwo");
		else if (scenename == "StageThreeTwo")
			ReserveLoadScene("StageThreeThree");
		else if (scenename == "StageThreeThree")
			ReserveLoadScene("StageThreeFour");
		else if (scenename == "StageThreeFour")
			ReserveLoadScene("StageThreeFive");
		else
			ReserveLoadScene("StageThreeOne");

		int bgmNumber = SoundManager::GetInstance()->GetCurrentPlayBGM();

		if (bgmNumber != -1)
			SoundManager::GetInstance()->Stop(bgmNumber);
	}
	else if (Input::GetInstance().GetKeyState(VK_F9) == KEYSTATE::DOWN)
	{
		std::string scenename = Game::GetInstance().GetCurrentScene()->GetSceneName();

		FadeOutObject* fadeOut;

		if (scenename == "StageOne")
		{
			fadeOut = new FadeOutObject("StageTwoOpeningLoadingScene");

			Game::GetInstance().GetCurrentScene()->AddGameObject(fadeOut);
		}
		else if (scenename == "StageTwo")
		{
			fadeOut = new FadeOutObject("StageThreeOneOpeningLoadingScene");

			Game::GetInstance().GetCurrentScene()->AddGameObject(fadeOut);
		}
		else if (scenename == "StageThreeOne")
		{
			if (mGamePlayer->GetBodyWeight() >= 100.0f)
			{
				fadeOut = new FadeOutObject("OneEndingLoadingScene");
			}
			else
			{
				fadeOut = new FadeOutObject("TwoEndingLoadingScene");
			}

			Game::GetInstance().GetCurrentScene()->AddGameObject(fadeOut);
		}
		else
		{
			fadeOut = new FadeOutObject("StageOneOpeningLoadingScene");

			Game::GetInstance().GetCurrentScene()->AddGameObject(fadeOut);
		}

		int bgmNumber = SoundManager::GetInstance()->GetCurrentPlayBGM();

		if (bgmNumber != -1)
			SoundManager::GetInstance()->Stop(bgmNumber);
	}
#endif

	mCurrentScene->SceneLoop();

	// �� ������ ����Ǿ� �ִٸ� ��� SceneLoop�� ������ �� ���� ������ ��ȯ�մϴ�.
	if (mIsReservedLoadScene)
		LoadScene();

	// ���� ���� �÷��̾��� ����� �� �����ų�,
	// �������� �� ���� ��ŷ ����� �Ϸ�Ǿ��ų�,
	// ���� ���Ḧ ������ �� ���� �ǰڽ��ϴ�.

	return mIsEndGame;
}

void Game::EndGame()
{
	mIsEndGame = true;
	/*SoundManager::DestroyInstance();*/
}

// ������ �츮 ������ ���� Initialize���� ��� ���� �غ��ϱ� ������ �ʿ䰡 ���� ����̴�.
void Game::RegisterScene(Scene* scene)
{
	// ���ӿ��� �����ϴ� Scene Collection�� ����մϴ�.
	mScenes.insert({ scene->GetSceneName(), scene });
}

// �� �̸��� �޾Ƽ� �ε带 �����մϴ�.
void Game::ReserveLoadScene(std::string sceneName)
{
	// ������ �߿� ���� �޼����� ������ �� �ֵ��� assert �ɾ���� ..
	assert(mScenes.find(sceneName) != mScenes.end());

	mIsReservedLoadScene = true;

	mNextScene = mScenes[sceneName];
}

void Game::LoadScene()
{
	if (mCurrentScene != nullptr)
		mCurrentScene->ClearScene();

	// ����, �ε� ���ε� �ɼ��� �� �����ִٸ� ���� ..!
	if (mIsOptionOn)
		mIsOptionOn = !mIsOptionOn;

	mCurrentScene = mNextScene;

	// �÷��̾� �������ݴϴ�.
	mCurrentScene->SetPlayer(mGamePlayer);

	mGamePlayer->GetSpriteRenderer()->SetOpacity(1.f);

	mCurrentScene->BuildScene();

	mIsReservedLoadScene = false;
}

void Game::OptionToggle()
{
	mIsOptionOn = !mIsOptionOn;
	// True�̸�, �����ؼ� �־�����.

	if (mIsOptionOn)
		OnOption();
	else
		OffOption();
}

void Game::InitPlayer()
{
	// �� ���� �ִ� �÷��̾� ������ ����������ϴµ� .. ���� �̰� �����ϴ�.
	// mCurrentScene->ReserveDestroy(mGamePlayer);

	if (mGamePlayer != nullptr)
	{
		mGamePlayer->InitPlayer();
	}
}

void Game::InitScenes()
{
	for (auto scene : mScenes)
		scene.second->mIsFirstLoad = true;
}

void Game::InitOption()
{
	D2D1_SIZE_F size = Graphics::GetInstance().GetScreenSize();

	// �޴��� ����
	UIDecoObject* menu = new UIDecoObject();

	menu->GetSpriteRenderer()->SetSprite(mResource->GetSprite("OPTION_MENU"));
	menu->GetTransform()->SetPosition(Vector2D(size.width / 2, size.height / 2));
	menu->GetSpriteRenderer()->SetSortOrder(2);

	// BGM Progress Bar
	//ProgressBar* bgmBack = new ProgressBar(mResource->GetSprite("OPTION_BACK_PROGRESS_BAR"), false);
	//bgmBack->GetTransform()->SetPosition(bgmBack->GetTransform()->GetPosition() + Vector2D(size.width / 2, size.height / 2 - 130));
	//bgmBack->GetSpriteRenderer()->SetSortOrder(3);

	// BGM ���� �����ϴ� �Լ��� �������ش�.
	ProgressBar* bgmFront = new ProgressBar(mResource->GetSprite("OPTION_FRONT_PROGRESS_BAR"), true, [](float percent)
		{
			SoundManager::GetInstance()->SetBGMVolume(percent);
		},
		SoundManager::GetInstance()->GetBGMVolume());
	bgmFront->SetPercent(SoundManager::GetInstance()->GetBGMVolume());

	bgmFront->GetTransform()->SetPosition(bgmFront->GetTransform()->GetPosition() + Vector2D(size.width / 2, size.height / 2 - 130));
	bgmFront->GetSpriteRenderer()->SetSortOrder(2);

	ProgressBar* bgmBar = new ProgressBar(mResource->GetSprite("OPTION_PROGRESS_BAR"), false);
	bgmBar->GetTransform()->SetPosition(bgmBar->GetTransform()->GetPosition() + Vector2D(size.width / 2, size.height / 2 - 130));
	bgmBar->GetSpriteRenderer()->SetSortOrder(1);

	UIDecoObject* bgmText = new UIDecoObject();

	bgmText->GetSpriteRenderer()->SetSprite(mResource->GetSprite("OPTION_BGM_TEXT"));
	bgmText->GetTransform()->SetPosition(bgmBar->GetTransform()->GetPosition() - Vector2D(60.0f, -30.0f));
	bgmText->GetSpriteRenderer()->SetSortOrder(1);

	// SFX Progress Bar
	//ProgressBar* sfxBack = new ProgressBar(mResource->GetSprite("OPTION_BACK_PROGRESS_BAR"), false);
	//sfxBack->GetTransform()->SetPosition(sfxBack->GetTransform()->GetPosition() + Vector2D(size.width / 2, size.height / 2 - 65));
	//sfxBack->GetSpriteRenderer()->SetSortOrder(3);

	// SFX ���� �����ϴ� �Լ��� �������ش�.
	ProgressBar* sfxFront = new ProgressBar(mResource->GetSprite("OPTION_FRONT_PROGRESS_BAR"), true, [](float percent)
		{
			SoundManager::GetInstance()->SetSFXVolume(percent);
		},
		SoundManager::GetInstance()->GetSFXVolume());
	sfxFront->SetPercent(SoundManager::GetInstance()->GetSFXVolume());

	sfxFront->GetTransform()->SetPosition(sfxFront->GetTransform()->GetPosition() + Vector2D(size.width / 2, size.height / 2 - 65));
	sfxFront->GetSpriteRenderer()->SetSortOrder(2);

	ProgressBar* sfxBar = new ProgressBar(mResource->GetSprite("OPTION_PROGRESS_BAR"), false);
	sfxBar->GetTransform()->SetPosition(sfxBar->GetTransform()->GetPosition() + Vector2D(size.width / 2, size.height / 2 - 65));
	sfxBar->GetSpriteRenderer()->SetSortOrder(1);

	UIDecoObject* sfxText = new UIDecoObject();

	sfxText->GetSpriteRenderer()->SetSprite(mResource->GetSprite("OPTION_SFX_TEXT"));
	sfxText->GetTransform()->SetPosition(sfxBar->GetTransform()->GetPosition() - Vector2D(60.0f, -30.0f));
	sfxText->GetSpriteRenderer()->SetSortOrder(1);

	// �����
	Restart* res = new Restart();
	res->GetTransform()->SetPosition(Vector2D(size.width / 2, size.height / 2 + 50.f));
	res->GetSpriteRenderer()->SetSortOrder(1);

	// �ڷΰ���
	Back* back = new Back();
	back->GetTransform()->SetPosition(Vector2D(size.width / 2, size.height / 2 + 125.0f));
	back->GetSpriteRenderer()->SetSortOrder(1);

	// Ÿ��Ʋ��
	Title* title = new Title();
	title->GetTransform()->SetPosition(Vector2D(size.width / 2, size.height / 2 + 200.f));
	title->GetSpriteRenderer()->SetSortOrder(1);

	// ���� ����
	Exit* exit = new Exit();
	exit->GetTransform()->SetPosition(Vector2D(size.width / 2, size.height / 2 + 275.f));
	exit->GetSpriteRenderer()->SetSortOrder(1);

	// �޴� ��ü ����Ʈ ����
	mMenuList.push_back(menu);
	mMenuList.push_back(res);
	mMenuList.push_back(back);
	mMenuList.push_back(title);
	//mMenuList.push_back(bgmBack);
	mMenuList.push_back(bgmBar);
	mMenuList.push_back(bgmFront);
	mMenuList.push_back(bgmText);
	//mMenuList.push_back(sfxBack);
	mMenuList.push_back(sfxBar);
	mMenuList.push_back(sfxFront);
	mMenuList.push_back(sfxText);
	mMenuList.push_back(exit);
}

void Game::OnOption()
{
	for (int i = 0; i < mMenuList.size(); i++)
	{
		mCurrentScene->AddGlobalGameObject(mMenuList[i]);
	}

	// �Ϻ� �޴��� Ư�� �������� Ȱ�� / ��Ȱ��ȭ �Ѵ�.
	if (mCurrentScene->GetSceneName() != "Intro" && mCurrentScene->GetSceneName() != "StageOneOpening" &&
		mCurrentScene->GetSceneName() != "StageTwoOpening" && mCurrentScene->GetSceneName() != "StageThreeOneOpening")
	{
		dynamic_cast<ButtonBase*>(mMenuList[1])->SetActivate(true);
		dynamic_cast<ButtonBase*>(mMenuList[2])->SetActivate(true);
		dynamic_cast<ButtonBase*>(mMenuList[3])->SetActivate(true);
	}
	else
	{
		dynamic_cast<ButtonBase*>(mMenuList[1])->SetActivate(false);
		dynamic_cast<ButtonBase*>(mMenuList[2])->SetActivate(false);
		dynamic_cast<ButtonBase*>(mMenuList[3])->SetActivate(false);
	}

	int bgmNumber = SoundManager::GetInstance()->GetCurrentPlayBGM();

	if (bgmNumber != -1)
		SoundManager::GetInstance()->Pause(bgmNumber);
}

void Game::OffOption()
{
	for (int i = 0; i < mMenuList.size(); i++)
	{
		mCurrentScene->SubGlobalGameObject(mMenuList[i]);
	}

	int bgmNumber = SoundManager::GetInstance()->GetCurrentPlayBGM();

	if (bgmNumber != -1)
		SoundManager::GetInstance()->Resume(bgmNumber);
}