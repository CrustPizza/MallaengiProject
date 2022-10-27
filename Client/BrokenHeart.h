#pragma once

#define SPEED 1500.f

/// <summary>
/// ���ū ��Ʈ. �Ʒ��� ĳ���Ͱ� �νĵǸ� ��������Ʈ ���� �� �ϰ�. ������ ���
/// 
/// 2022. 02. 24. Comet
/// </summary>
class BrokenHeart : public GameObject
{
public:
	BrokenHeart();
	virtual ~BrokenHeart();

	Player* mGamePlayer;

	bool mTrigger;

	Sprite* mChangeSprite;

public:
	virtual void Update() override;
	virtual void Release() override;
};