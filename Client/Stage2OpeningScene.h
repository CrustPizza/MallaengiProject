#pragma once

#include "Scene.h"

/// <summary>
/// �������� 2 ������ �� ������ ����Ƽ�� ��
/// 
/// 2022. 02. 24 JK
/// </summary>
class Stage2OpeningScene : public Scene
{
public:
	Stage2OpeningScene();
	virtual ~Stage2OpeningScene();

	virtual void BuildScene() override;
};