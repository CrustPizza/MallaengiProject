#pragma once

/**
 * Json File ��, ����� �����ϱ� ���� Scene
 *
 * 2022. 02. 15. Comet
 */
class CometScene : public Scene
{
public:
	CometScene();
	virtual ~CometScene();

public:
	virtual void BuildScene() override;
};