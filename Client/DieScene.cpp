#include "PreCompiledHeader.h"
#include "DieScene.h"

#include "DecoObject.h"
#include "Player.h"
#include "RestartButton.h"
#include "SubCamera.h"

DieScene::DieScene(std::string sceneName):
Scene(sceneName + "DieScene"),
mStageName(sceneName)
{

}

DieScene::~DieScene()
{
}

void DieScene::BuildScene()
{
	Scene::BuildScene();

	// ���� ���� ��� ī�޶�� ..!
	AddGameObject(new SubCamera());

	D2D1_SIZE_F size = Graphics::GetInstance().GetScreenSize();

	// ĳ���� ��
	GameObject* faceUI = new DecoObject();

	faceUI->GetTransform()->SetPosition(Vector2D(size.width / 2 / CAMERA_ZOOM, size.height / 2 / CAMERA_ZOOM));

	faceUI->GetSpriteRenderer()->SetSprite(Game::GetInstance().GetResource()->GetSprite("DIE_FACE"));

	faceUI->GetSpriteRenderer()->SetSortOrder(10);		// �� �ڷ� ���� ..

	AddGameObject(faceUI);

	GameObject* restart = new RestartButton(mStageName);

	restart->GetTransform()->SetPosition(Vector2D(size.width / 2, size.height / 2 + 100));

	AddGameObject(restart);

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
				number->GetSpriteRenderer()->SetSprite(Game::GetInstance().GetResource()->GetSprite("DIE_NUMBERS_ZERO"));
			else if (life[i] == '1')
				number->GetSpriteRenderer()->SetSprite(Game::GetInstance().GetResource()->GetSprite("DIE_NUMBERS_ONE"));
			else if (life[i] == '2')
				number->GetSpriteRenderer()->SetSprite(Game::GetInstance().GetResource()->GetSprite("DIE_NUMBERS_TWO"));
			else if (life[i] == '3')
				number->GetSpriteRenderer()->SetSprite(Game::GetInstance().GetResource()->GetSprite("DIE_NUMBERS_THREE"));
			else if (life[i] == '4')
				number->GetSpriteRenderer()->SetSprite(Game::GetInstance().GetResource()->GetSprite("DIE_NUMBERS_FOUR"));
			else if (life[i] == '5')
				number->GetSpriteRenderer()->SetSprite(Game::GetInstance().GetResource()->GetSprite("DIE_NUMBERS_FIVE"));
			else if (life[i] == '6')
				number->GetSpriteRenderer()->SetSprite(Game::GetInstance().GetResource()->GetSprite("DIE_NUMBERS_SIX"));
			else if (life[i] == '7')
				number->GetSpriteRenderer()->SetSprite(Game::GetInstance().GetResource()->GetSprite("DIE_NUMBERS_SEVEN"));
			else if (life[i] == '8')
				number->GetSpriteRenderer()->SetSprite(Game::GetInstance().GetResource()->GetSprite("DIE_NUMBERS_EIGHT"));
			else if (life[i] == '9')
				number->GetSpriteRenderer()->SetSprite(Game::GetInstance().GetResource()->GetSprite("DIE_NUMBERS_NINE"));

			number->GetTransform()->SetPosition(Vector2D(size.width / 2 / CAMERA_ZOOM + 55.f + i * 30.f, size.height / 2 / CAMERA_ZOOM - 100.f));

			AddGameObject(number);
		}
	}
}