#include "PreCompiledHeader.h"
#include "EndingSceneOne.h"

#include "SubCamera.h"
#include "NarrativeSprite.h"

#include "JsonManager.h"

EndingSceneOne::EndingSceneOne()
	: Scene("OneEnding")
{
}

EndingSceneOne::~EndingSceneOne()
{
}

void EndingSceneOne::BuildScene()
{
	Scene::BuildScene();

	AddGameObject(new SubCamera());

	// Opening Narrative ������ �о����
	NarrativeJson narrativeJson;
	narrativeJson.DeserializeFromFile("Resources\\Json\\Ending1NarrativeTable.json");

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

	SoundManager::GetInstance()->Play(500008);
}