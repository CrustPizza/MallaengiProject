#pragma once
#include "Collision.h"

class Transform;
class GameObject;

/// <summary>
/// 2022. 02. 08. MG
/// 
/// ��Ŭ �ݸ���
/// </summary>
class CircleCollision : public Collision
{
public:
	CircleCollision(GameObject* object);
	~CircleCollision();

private:
	float mRadius;

public:
	float GetRadius() { return mRadius; }
	void SetRadius(float radius) { mRadius = radius; }

	virtual bool CheckCollision(Collision* collision) override;
};