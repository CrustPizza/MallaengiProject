#pragma once

#include "Scene.h"

/// <summary>
/// �������� 3 ������ �� ������ ����Ƽ�� ��
/// 
/// 2022. 02. 24 JK
/// </summary>
class Stage3OpeningScene : public Scene
{
public:
	Stage3OpeningScene();
	virtual ~Stage3OpeningScene();

	virtual void BuildScene() override;
};