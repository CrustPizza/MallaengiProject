#pragma once

class Player;

/**
 * \brief �������� �÷��� �� �ֻ�ܿ��� �÷��̾��� �����Ը� ǥ���ϴ� UI
 *
 * 2022. 02. 19. Comet
 */
class BodyScale : public GameObject
{
public:
	BodyScale();
	virtual ~BodyScale();

private:
	Player* mGamePlayer;

	std::vector<GameObject*> mNumbers;

public:
	virtual void LateUpdate() override;
};