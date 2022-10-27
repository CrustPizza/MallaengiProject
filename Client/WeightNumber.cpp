#include "PreCompiledHeader.h"
#include "WeightNumber.h"
#include "JsonManager.h"

WeightNumber::WeightNumber(Vector2D pos, int number)
	: mAnimClip(nullptr)
{
	if (number >= 0)
		pos.mX += 100.0f;
	else
		pos.mX -= 50.0f;

	GetTransform()->SetPosition(pos);
	GetTransform()->SetScale(Vector2D(2.5f, 2.5f));

	// �Է¹��� �Ű������� ���� ���� �ּҸ� ����� �о�´�.
	SpriteJson spriteJson;
	char buf[16];

	_itoa_s(number, buf, 10);

	if (number == 1 || number == 3 || number == 5 || number == 10
		|| number == -1 || number == -3 || number == -5 || number == -10)
	{
		string str = "Resources\\Effects\\Weight";
		str += buf;
		str += ".json";
		spriteJson.DeserializeFromCArchive(str);

		// Json�� �ִ� ��������Ʈ ���� ���� ����
		for (int i = 0; i < spriteJson.mSpriteList.size(); i++)
		{
			SpriteInfo temp = spriteJson.mSpriteList[i];
			mSprites.push_back(new Sprite(
				temp.imageID,
				Vector2D(0.5f, 0.5f),
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
				Game::GetInstance().GetCurrentScene()->ReserveDestroy(object);
			});

		// ����� SpriteRenderer ��ü ����
		AddSpriteRenderer();
	}
	else
	{
		Game::GetInstance().GetCurrentScene()->ReserveDestroy(this);
	}
}

WeightNumber::~WeightNumber()
{

}

void WeightNumber::Release()
{
	GetAnimator()->Release();
}