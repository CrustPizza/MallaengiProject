#pragma once
#include "Player.h"

#define DELETE_TIME 1.5f

/// <summary>
/// �ֺ��� �÷��̾ ���� (1.5 tile) ���, ������ ����
/// 
/// 1.5�ʵ� ����. 2022. 02. 24. Comet
/// </summary>
class SlowNiddle : public GameObject
{
public:
	SlowNiddle();

	virtual ~SlowNiddle();

private:
	bool mTrigger;

	Player* mGamePlayer;

	float mTimer;

public:
	virtual void Update() override;
};