#pragma once


/// <summary>
/// ���� ���ᰡ �� �� Fade-Out�� �ǽ��ϴ� ������Ʈ
///
///	2022. 02. 16. Comet
/// </summary>
class FadeOutObject : public GameObject
{
public:
	FadeOutObject();
	FadeOutObject(std::string sceneName);
	virtual ~FadeOutObject();

private:
	float mFadeTime;

	// ���̵� �ƿ��� ������ �ε��� �� �̸�
	std::string mSceneName;

	bool mIsWantLoadScene;

	// Ÿ��Ʋ ȭ������ ..?
	bool mIsTitle;

	float mVolume;

public:
	virtual void Update() override;
};