#pragma once

/**
 * ������ �������� ��ȭ Food. ������ �ڽ��� ������ �����ȴ�.
 *
 * 2022. 02. 17. Comet
 */
class Food : public GameObject
{
public:
	Food(float weight, int ID, Vector2D pos, bool isInBox);
	virtual ~Food();

	// �̰� ���� �� �þ�ų�, ���ҵǴ� ������
	float mWeight;

	// �� ģ���� ����ϴ� �̹��� ���̵�
	int mID;

	// �� ģ���� ����ϴ� �̹��� ��Ʈ������ �»��
	Vector2D mPos;

	// Ÿ�� ũ���� ���ݸ�ŭ �̵��ϸ� ��
	float mMoveDist;

	// �ڽ����� ���� �� �༮����, �ƴ���.
	bool mIsInBox;

	// ��, �Ʒ��� ������ �� ����ϴ� ����
	int mDirrection;

	// ��, �Ʒ��� ������ �� �󸶳� �������°�.
	float mTotalMoveDist;

	// �����°�.
	bool mIsEaten;

	static float mMallaengBar;

public:
	virtual void Update() override;
};