#pragma once

/**
 * \brief : �������� �÷��� �� �ð踦 ��Ÿ���� �༮
 */
class Watch : public GameObject
{
public:
	Watch();
	virtual ~Watch();

private:
	Player* mGamePlayer;

	std::vector<GameObject*> mNumbers;

public:
	virtual void LateUpdate() override;
};