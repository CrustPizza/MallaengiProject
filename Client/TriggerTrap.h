#pragma once

/// <summary>
/// 2. ĳ���� �浹 Ʈ������ ����
/// 
/// ĳ���Ϳ� �浹�ϸ� ĳ���͸� ����Ų��. ��, hp = 0�� ����ϴ�.
/// 
/// 2022. 02. 11. Comet Lee
/// </summary>
class TriggerTrap : public GameObject
{
public:
	TriggerTrap();
	TriggerTrap(Sprite* nextSprite);
	TriggerTrap(bool IsUp);
	virtual ~TriggerTrap();

	// Ʈ���Ű� �Ǹ� ������ ��������Ʈ
	Sprite* mNextSprite;

	// Ƣ����� Ʈ�����ΰ�
	bool mIsUp;

public:
	void FixedUpdate() override;

	void Update() override;

	virtual void Release() override;
};