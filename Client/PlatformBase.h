#pragma once

// �浹�� ��� ���⿡�� �Ͼ�°�.
enum class CollisionDir
{
	LEFT,
	UP,
	RIGHT,
	DOWN,
	NONE
};

/// <summary>
/// Platform One-Way Collision�� �޴� ������Ʈ
/// 
/// 2022. 02. 09. Comet
/// </summary>
class PlatformBase : public GameObject
{
public:
	PlatformBase();
	virtual ~PlatformBase();

protected:
	D2D1_SIZE_F mSize;

	AABBCollision* mAABBCollision;

	CollisionDir mCollisionDir;

public:
	virtual void FixedUpdate() override;

	virtual void Update() override;

	D2D1_SIZE_F GetSize() { return mSize; }

	CollisionDir GetCollisionDir() { return mCollisionDir; }

	void SetCollisionDir(CollisionDir collisionDir) { mCollisionDir = collisionDir; }

	// Platform �̺�Ʈ ��¿� �Լ�
	// virtual void OnCollisionEvent();
};