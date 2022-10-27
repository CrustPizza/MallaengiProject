#pragma once

#define WALKPAD_SPEED_PER_SEC 250.f
#include "BlockBase.h"

/**
 * \brief : ��� ������ ������ �������� ���� ..!
 *
 * 2022. 02. 23. Comet
 */
class WalkPad : public GameObject
{
public:
	WalkPad(Vector2D dir);
	virtual ~WalkPad();

private:
	Vector2D mDirrection;

	Player* mGamePlayer;

	static bool mPlayerChecked;

	static bool mMonsterChecked;

	static bool mIsDoen;

public:

	virtual void FixedUpdate() override;
	virtual void Update() override;
};