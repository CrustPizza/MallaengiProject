#include "PreCompiledHeader.h"
#include "RunEffect.h"
#include "Player.h"

RunEffect::RunEffect(GameObject* target, Vector2D direction)
	: mCharacter(target)
	, mDirection(direction)
	, mAnimClip(nullptr)
{
	// ���⿡ ���缭 ����
	GetTransform()->SetScale(Vector2D(direction.mX, 1));

	// Json���� �о����
	SpriteJson spriteJson;
	spriteJson.DeserializeFromCArchive(_T("Resources\\Effects\\RunEffect.json"));

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
			Game::GetInstance().GetPlayer()->mRunEffect = nullptr;
			Game::GetInstance().GetCurrentScene()->ReserveDestroy(object);
		});

	// ����� SpriteRenderer ��ü ����
	AddSpriteRenderer();
}

RunEffect::~RunEffect()
{
	Game::GetInstance().GetPlayer()->mRunEffect = nullptr;
}

void RunEffect::Update()
{
	GameObject::Update();

	// �������� �׻� �÷��̾� ������ ���ش�.
	GetTransform()->SetPosition(mCharacter->GetTransform()->GetPosition() - Vector2D(mDirection.mX * 37, -75));
}

void RunEffect::Release()
{
	GetAnimator()->Release();
}

void RunEffect::SetDirection(Vector2D direction)
{
	// ������ �ٲ���� �� ���
	mDirection = direction;
	GetTransform()->SetScale(Vector2D(direction.mX, 1));
}