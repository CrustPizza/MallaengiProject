#pragma once

/// <summary>
/// ������ �ڽ� ���ο� reserve �Ǿ� �ִٰ�, �̺�Ʈ ������ ���ͼ�
///
///	�ִϸ��̼� Ʋ�� �÷��̾� ���� 2022. 02. 23. Comet
/// </summary>
class CatBoxTrap : public GameObject
{
public:
	CatBoxTrap();
	virtual ~CatBoxTrap();

private:
	void LoadAnimation();

	float mTimer;

public:
	virtual void Update() override;
};