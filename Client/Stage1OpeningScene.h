#pragma once

#include "Scene.h"

/// <summary>
/// �������� 1 ������ �� ������ ����Ƽ�� ��
/// 
/// 2022. 02. 24 JK
/// </summary>
class Stage1OpeningScene : public Scene
{
public:
	Stage1OpeningScene();
	virtual ~Stage1OpeningScene();

	virtual void BuildScene() override;
};