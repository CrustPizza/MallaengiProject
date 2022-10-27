#include "pch.h"
#include "Timer.h"
#include "Animator.h"

Animator::Animator(GameObject* object)
	: mGameObject(object),
	mTransform(object->GetTransform()),
	mPlayingTime(0),
	mTotalFrameOfClip(0),
	mCurrentIndex(0),
	mIsLoop(true),
	mAnimationClip(nullptr),
	mFunc(nullptr)
{
	
}

Animator::~Animator()
{

}

void Animator::SetNotLoop(void(*func)(GameObject* object))
{
	mFunc = func;
	mIsLoop = false;
}

void Animator::Play()
{
	// ���� �帣�� ��Ÿ Ÿ�� �޾ƿ� -> ���� ��� ���� ��������Ʈ�� ����� �����ϰ� ���� ������� ���� ��� �ð�
	mPlayingTime += Timer::GetInstance().GetDeltaTime() / 1000;

	// ���� ��� ���� ��������Ʈ�� ������Ÿ�� (�󸶳� ����� ������ ������) <= ���� ������� ���� ��������Ʈ�� �ð�
	if (mAnimationClip->GetSpriteOfIndex(mCurrentIndex)->mDelayTime <= mPlayingTime)
	{
		// ���� �ε����� 1 �������Ѽ� �� �ε��� ���� ������������ �Ѵ�. -> ���� ��������Ʈ �ε��� ����.
		mCurrentIndex++;

		if (mIsLoop == true)
		{
			mCurrentIndex %= mTotalFrameOfClip;
		}
		else
		{
			if (mCurrentIndex >= mTotalFrameOfClip)
			{
				mFunc(mGameObject);
			}
		}

		// ���� ��½ð� 0���� �����
		mPlayingTime = 0.f;
	}

	// ������ ���� ����
	Renderer* renderer = mGameObject->GetRenderer();

	// setSprite�ϱ� ���� ����
	SpriteRenderer* spriteRenderer = dynamic_cast<SpriteRenderer*>(renderer);

	if (spriteRenderer != nullptr)
	{
		// ���� ��������Ʈ �ε����� ����ϵ��� CurrentIndex �� ���� ���ش�.
		spriteRenderer->SetSprite(mAnimationClip->GetSpriteOfIndex(mCurrentIndex));
	}
}

void Animator::Release()
{
	if (mAnimationClip != nullptr)
	{
		Renderer* renderer = mGameObject->GetRenderer();

		SpriteRenderer* spriteRenderer = dynamic_cast<SpriteRenderer*>(renderer);

		if (spriteRenderer != nullptr)
			spriteRenderer->SetSprite(nullptr);

		delete mAnimationClip;
		mAnimationClip = nullptr;
	}
}