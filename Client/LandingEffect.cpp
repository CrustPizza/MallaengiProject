#include "PreCompiledHeader.h"
#include "LandingEffect.h"
#include "Player.h"

LandingEffect::LandingEffect(GameObject* target)
	: mCharacter(target)
	, mAnimClip(nullptr)
{
	// Json���� �о����
	SpriteJson spriteJson;
	spriteJson.DeserializeFromCArchive(_T("Resources\\Effects\\LandingEffect.json"));

	// Json�� �ִ� ��������Ʈ ���� ���� ����
	for (int i = 0; i < spriteJson.mSpriteList.size(); i++)
	{
		SpriteInfo temp = spriteJson.mSpriteList[i];
		mSprites.push_back(new Sprite(
			temp.imageID,
			Vector2D::Zero,
			{ temp.imageRect.right - temp.imageRect.left, temp.imageRect.bottom - temp.imageRect.top },
			temp.duration,
			Vector2D(temp.imageRect.left, temp.imageRect.top),
			{ temp.pivot.mX, temp.pivot.mY }));
	}

	// �ִϸ��̼� Ŭ���� �߰��ϰ�
	mAnimClip = new AnimationClip(mSprites);

	// �ִϸ����͸� �����Ͽ� �����Ѵ�.
	AddAnimator()->SetAnimationClip(mAnimClip);

	// �ִϸ����Ϳ��� �ִϸ��̼� ����� ������ ȣ���� �̺�Ʈ �Լ�
	GetAnimator()->SetNotLoop([](GameObject* object)
		{
			Game::GetInstance().GetPlayer()->mLandingEffect = nullptr;
			Game::GetInstance().GetCurrentScene()->ReserveDestroy(object);
		});

	// ����� SpriteRenderer ��ü ����
	AddSpriteRenderer()->SetSortOrder(1);

	//Landing sound
	SoundManager::GetInstance()->Play(500015);
}

LandingEffect::~LandingEffect()
{
	Game::GetInstance().GetPlayer()->mLandingEffect = nullptr;
}

void LandingEffect::Update()
{
	GameObject::Update();

	// �������� �׻� �÷��̾� ������ ���ش�.
	GetTransform()->SetPosition(mCharacter->GetTransform()->GetPosition() - Vector2D(mDirection.mX * 37, -75));
}

void LandingEffect::Release()
{
	GetAnimator()->Release();
}