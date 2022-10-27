#pragma once

class Platform;
/**
 * \brief �÷��̾ ������ �߶��ϴ� �÷���
 *
 * 2022. 02. 21. Comet
 */
class FallingPlatform : public GameObject
{
public:
	FallingPlatform(Vector2D startPos);
	virtual ~FallingPlatform();

	bool mIsTriggered;					// Ʈ���� �Ǿ����� ����

	Player* mGamePlayer;				// Ʈ���ŵǸ� ���⿡ �÷��̾ �����մϴ�.

	Platform* mUpper;

	float mFallingSpeed;				// �������� �ӵ��� ��Ʈ���Ѵ�. �߷��� �޴� ��ó�� �����մϴ�.

	float mTimer;						// ���� �ð� ������ ���־���

public:
	virtual void Update() override;
};