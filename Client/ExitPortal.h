#pragma once

/// <summary>
/// �������� 3�� ���� ��Ż (3-1 => ...)
/// 
/// 2022. 02. 24. Comet
/// </summary>
class ExitPortal : public GameObject
{
public:
	ExitPortal();
	virtual ~ExitPortal();
	
	bool mTrigger;

	float mTimer;

	bool mIsDone;

	Player* mGamePlayer;

public:
	virtual void Update() override;
};