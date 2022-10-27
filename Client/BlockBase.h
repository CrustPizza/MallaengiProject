#pragma once

enum class CollisionDir;

/// <summary>
/// ��� ��ϵ��� �⺻�� �Ǵ� Ŭ����
/// 
/// 2022. 02. 07. Comet
/// </summary>
class BlockBase : public GameObject
{
public:
	BlockBase();
	virtual ~BlockBase();

protected:
	// ����, ���� (Ÿ�� �� ũ�� �޾Ƽ� �ϸ� �ɵ� ..?)
	// ������, �ݸ��� ��� ������ ���� �־�� �Ѵ�.
	D2D1_SIZE_F mSize;

	CollisionDir mCollisionDir;

	// �̰� ���� �ֵ��� ����. ������ ��� �̰� ���Ŵϱ�.
	AABBCollision* mAABBCollision;

public:
	void SetCollisionDir(CollisionDir col) { mCollisionDir = col; }
	
	CollisionDir GetCollisionDir() { return mCollisionDir; }

	virtual void FixedUpdate() override;

	virtual void Update() override;

	D2D1_SIZE_F GetSize() { return mSize; }

	// �� ģ���� �������̵��ؼ�, �ʿ��� ��Ȳ�� �θ���.
	//virtual void OnCollisionEvent();
};