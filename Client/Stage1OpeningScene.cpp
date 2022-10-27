#include "PreCompiledHeader.h"
#include "Stage1OpeningScene.h"

#include "SubCamera.h"
#include "NarrativeSprite.h"

#include "JsonManager.h"

/// <summary>
/// ���� ȿ���� �ִ� �׽�Ʈ Ŭ����
/// �Ⱦ��ٸ� ����ų� ���ÿ� �����Ͽ� ���
/// 
/// 2022. 02. 24 JK
/// </summary>
class testobject : public GameObject
{
	float volume;	// ȿ���� ����
	float delay;	// ��±��� ���� �ð�

public:
	testobject(float _delay) : volume(1.0f), delay(_delay) {};

	virtual void Update() override
	{
		// ���� �ð����� ���
		if (delay <= 0.0f)
		{
			// ��� �� ����
			SoundManager::GetInstance()->Play(500032);
			Game::GetInstance().GetCurrentScene()->ReserveDestroy(this);
		}
		else
		{
			delay -= Timer::GetInstance().GetDeltaTime() / 1000;
		}
	}
};

Stage1OpeningScene::Stage1OpeningScene()
	: Scene("StageOneOpening")
{

}

Stage1OpeningScene::~Stage1OpeningScene()
{

}

void Stage1OpeningScene::BuildScene()
{
	Scene::BuildScene();

	AddGameObject(new SubCamera());

	// Opening Narrative ������ �о����
	NarrativeJson narrativeJson;
	narrativeJson.DeserializeFromFile("Resources\\Json\\Stage1OpeningNarrativeTable.json");

	for (int i = 0; i < narrativeJson.mNarrativeList.size(); i++)
	{
		const NarrativeData& data = narrativeJson.mNarrativeList[i];

		// ��������Ʈ ������ ����
		Sprite* sprite = new Sprite(
			data.imageID,	// Image ID
			Vector2D::Zero,	// Pivot
			{ data.imageSize.width / CAMERA_ZOOM, data.imageSize.height / CAMERA_ZOOM }, // Size
			0.0f,			// Delay
			Vector2D::Zero);// Sheet Position

		// ��������Ʈ�� ��� ����, ����, �ӵ� �� ���ڷ� �־��ְ� ��ǥ ����
		NarrativeSprite* narrativeSprite = new NarrativeSprite(sprite, data.delay, data.duration, data.alphaSpeed);
		narrativeSprite->GetTransform()->SetPosition({ data.imagePos.mX / CAMERA_ZOOM, data.imagePos.mY / CAMERA_ZOOM });

		// ���� ������Ʈ �߰�
		AddGameObject(narrativeSprite);
	}

	// ���� ȿ������ �ִ´ٸ� �̷�������..
	AddGameObject(new testobject(34));
	
	// Story_1
	SoundManager::GetInstance()->Play(500005);
}