#include "PreCompiledHeader.h"
#include "Intro.h"

#include "StageOneBackGround.h"
#include "CheckPoint.h"
#include "DecoObject.h"
#include "InfoButton.h"
#include "JsonManager.h"
#include "StartButton.h"
#include "SubCamera.h"
#include "BreakableBlock.h"
#include "FreeEnemy.h"
#include "IntroBackGround.h"
#include "Platform.h"

#include "Player.h"
#include "UIDecoObject.h"

// �Ľ� ������ 0,0 ���� �����ϹǷ� .. => �������� �� �� �����ִ� �� !
#define LEFT_ADJUST 500.f
#define DOWN_ADJUST 350.f

Intro::Intro() :
Scene("Intro")
{

}

Intro::~Intro()
{

}

void Intro::BuildScene()
{
	Graphics::GetInstance().Resize();
	Scene::BuildScene();

	// �� ���� �÷��̾ �ʿ��մϴ�.
	AddGameObject(mGamePlayer);

	D2D1_SIZE_F size = Graphics::GetInstance().GetScreenSize();

	// ����� �÷��̾ ��ġ�����ش�.
	mGamePlayer->GetTransform()->SetPosition(Vector2D((size.width / 2) / CAMERA_ZOOM, (size.height / 2) / CAMERA_ZOOM));

	// �� ���� ���� ī�޶� �ʿ��մϴ�. (��ġ ����)
	AddGameObject(new SubCamera());

	GameObject* background = new IntroBackGround();
	
	SpriteRenderer* sprite = dynamic_cast<SpriteRenderer*>(background->GetRenderer());

	sprite->SetSprite(Game::GetInstance().GetResource()->GetSprite("STAGE_1_BACKGROUND"));

	sprite->SetSortOrder(10);

	AddGameObject(background);

	// ��ŸƮ ��ư�� �ʿ��ϴ�.
	GameObject* startButton = new StartButton();

	startButton->SetTag("UI");

	startButton->GetSpriteRenderer()->SetSprite(Game::GetInstance().GetResource()->GetSprite("INTRO_START"));

	startButton->GetTransform()->SetPosition(Vector2D(size.width / 2, 400.f));

	AddGameObject(startButton);

	// LOGO
	GameObject* logo = new DecoObject();

	logo->SetTag("UI");

	logo->GetTransform()->SetPosition(Vector2D(size.width / 2 - 20.f, 200.f));

	logo->GetSpriteRenderer()->SetSprite(Game::GetInstance().GetResource()->GetSprite("INTRO_LOGO"));

	AddGameObject(logo);

	// TUTO
	GameObject* tuto = new UIDecoObject();

	tuto->SetTag("UI");

	tuto->GetTransform()->SetPosition(Vector2D(size.width / 2, size.height - 60.f));

	tuto->GetSpriteRenderer()->SetSprite(Game::GetInstance().GetResource()->GetSprite("INTRO_TUTO"));

	AddGameObject(tuto);

	// INFO
	GameObject* info = new InfoButton();

	info->SetTag("UI");

	info->GetTransform()->SetPosition(Vector2D(size.width - 45.f, 60.f));

	info->GetSpriteRenderer()->SetSprite(Game::GetInstance().GetResource()->GetSprite("INTRO_INFO"));

	AddGameObject(info);

	// COPYRIGHT
	GameObject* copy = new UIDecoObject();

	copy->SetTag("UI");

	copy->GetTransform()->SetPosition(Vector2D(200.f, size.height - 15.f));

	copy->GetSpriteRenderer()->SetSprite(Game::GetInstance().GetResource()->GetSprite("INTRO_COPY"));

	AddGameObject(copy);

	LoadSceneData(_T("Resources\\Json\\IntroScene.json"), 1);
	
	//Title Sound
	SoundManager::GetInstance()->Play(500001);
}