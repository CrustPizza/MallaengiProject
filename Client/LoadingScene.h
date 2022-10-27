#pragma once
#include "Scene.h"

/// <summary>
/// ���⼭ ��¥ ���� ���������� �ִ� �͵��� �̸� �ε��ϴ°� ���� �� ���⵵ .. (Scene�� New�ϴ� ��)
/// 
/// 2022. 02. 10.
/// </summary>
class LoadingScene : public Scene
{
public:
	LoadingScene(std::string nextSceneName);
	virtual ~LoadingScene();

	std::string mLoadSceneName;

public:
	void BuildScene() override;
};