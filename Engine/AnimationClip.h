#pragma once

/// <summary>
/// 2022.02.07. MG.
/// 
/// �ִϸ��̼� Ŭ��
/// </summary>
class AnimationClip
{
public:
	AnimationClip(std::vector<Sprite*>& animation);
	~AnimationClip();

private:
	std::vector<Sprite*> mAnimation;		// ��������Ʈ���� ������ ��� ����

public:
	Sprite* GetSpriteOfIndex(int index) 
	{
		if (mAnimation.size() <= index)
			return nullptr;

		return mAnimation[index]; 
	}

	int GetTotalFrameOfAnimation()
	{
		return mAnimation.size();
	}
};