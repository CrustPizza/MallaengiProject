#pragma once
#include "TriggerTrap.h"
#include "Player.h"


/**
 * \brief : �ִϸ��̼��� ��� �ִ� ���� �������̴�.
 *
 * 2022. 02. 22. Comet
 */
class NachoShuriken : public GameObject
{
public:
	NachoShuriken(Vector2D dir);
	virtual ~NachoShuriken();

	// �������� ���ư� ����
	Vector2D mDirrection;



	AnimationClip* mAnimation;

public:
	virtual void Update() override;
};