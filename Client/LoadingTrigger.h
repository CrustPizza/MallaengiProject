#pragma once

/// <summary>
/// Ʈ���Ÿ� ���� �� ��ȯ�� �߻���Ű�� ���� Ŭ����
/// 
/// 2022. 02. 22 JK
/// </summary>
class LoadingTrigger : public GameObject
{
	const std::string mTriggerSceneName;

public:
	LoadingTrigger(const string& triggerSceneName);
	~LoadingTrigger();

	virtual void Update() override;
};