#pragma once


#define FALLING_SPEED 6 * TILE_HEIGHT

// �������� ���� �� 4�� �ں��� 1.5�� �������� ��������.

/// <summary>
/// 3-5 ����� Falling 
/// 
/// 2022. 02. 24. Comet
/// </summary>
class FiveFalling : public GameObject
{
public:
	FiveFalling(D2D1_SIZE_F collisionSize, float dropTime);

	virtual ~FiveFalling();

	float mDropTime;

public:
	virtual void Update() override;
};