#pragma once

class GameObject;
class Transform;
/// <summary>
/// 2022.02.07. MG
/// </summary>
class Animator
{
public:
	Animator(GameObject* object);
	~Animator();

private:
	GameObject* mGameObject;

	Transform* mTransform;

	AnimationClip* mAnimationClip;

	int mCurrentIndex;				// ����� ��������Ʈ�� �ε���
	float mPlayingTime;				// ���� ��� ���� ��������Ʈ�� ��� ������ �ð����� ��µǾ��°�.
	int mTotalFrameOfClip;			// ���� ��� ���� �ִϸ��̼� Ŭ���� Sprite �� ��

	bool mIsLoop;
	void (*mFunc)(GameObject* object);

public:
	GameObject* GetGameObject() { return mGameObject; }

	Transform* GetTransform() { return mTransform; }

	void SetNotLoop(void (*func)(GameObject* object));

	// �ִϸ��̼��� ���� ��������Ʈ ���� ������ ����. �� ��������Ʈ �� ����
	void SetAnimationClip(AnimationClip* animationClip) 
	{
		mAnimationClip = animationClip;

		mTotalFrameOfClip = mAnimationClip->GetTotalFrameOfAnimation();

		// CurrentIndex �ʱ�ȭ�� �ʼ� ..!
		mCurrentIndex = 0;
	}

	void Play();
	void Release();
};