#include "PreCompiledHeader.h"
#include "Stage3OpeningScene.h"

#include "SubCamera.h"
#include "NarrativeSprite.h"

#include "JsonManager.h"

Stage3OpeningScene::Stage3OpeningScene()
	: Scene("StageThreeOneOpening")
{

}

Stage3OpeningScene::~Stage3OpeningScene()
{

}

void Stage3OpeningScene::BuildScene()
{
	Scene::BuildScene();

	AddGameObject(new SubCamera());

	// Opening Narrative ������ �о����
	NarrativeJson narrativeJson;
	narrativeJson.DeserializeFromFile("Resources\\Json\\Stage3OpeningNarrativeTable.json");

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
	
	// Story_3
	SoundManager::GetInstance()->Play(500007);
}