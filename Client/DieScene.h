#pragma once


/**
 * �÷��̾ ���� �÷��� �� �׾��� �� ������ ��
 *
 * 2022. 02. 17. Comet
 */
class DieScene : public Scene
{
public:
	DieScene(std::string sceneName);
	virtual ~DieScene();

	// ���� ���������� �̸�
	std::string mStageName;

public:
	virtual void BuildScene() override;
};