#pragma once

// �ε� ���� ������ �ð��� ��Ʈ���մϴ�.
#define GARA_LOADING_TIME 2.f

/// <summary>
/// �ε� ���� �����ϴ� Object. �ð��� üũ�ؼ� ���� ������ �Ѱ��ش�.
/// 
/// ��, �̹��� ����� ���� ��� �� ģ���� ���ؼ� ����Ѵ�.
/// 
/// 2022. 02. 10. Comet
/// </summary>
class LoadingSceneObject : public GameObject
{
public:
	LoadingSceneObject(std::string nextSceneName);
	virtual ~LoadingSceneObject();

	std::string mNextSceneName;

	float mLoadTime;
	float mVolume;

public:
	virtual void FixedUpdate() override;

	virtual	void Update() override;
};