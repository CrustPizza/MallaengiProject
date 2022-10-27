#pragma once

enum class CollisionDir;
/**
 * �÷��̾ �Ʒ��鿡�� ġ�� �������� ���� �� ���� �̺�Ʈ�� ��ȸ������ �ǽ��Ѵ�.
 *
 * 2022. 02. 17. Comet
 */
class BoxBase : public GameObject
{
public:
	BoxBase();
	virtual ~BoxBase();

private:
	D2D1_SIZE_F mSize;

protected:
	// Ʈ���Ű� ������. (== Player�� �Ʒ����⿡�� ġ�� ����)
	bool mTrigger;
	// Ʈ���Ű� ������ ù ������ ���� �̺�Ʈ�� ������ ����.
	bool mIsFirstLoop;

	CollisionDir mCollisionDir;

public:
	void SetCollisionDir(CollisionDir col) { mCollisionDir = col; }

	CollisionDir GetCollisionDir() { return mCollisionDir; }
	
	virtual void Update() override;

	D2D1_SIZE_F GetSize() { return mSize; }

	// Ʈ���Ÿ� ���ش�.
	void TriggerOn() { mTrigger = true; }

	// Ʈ���Ű� �Ǿ��� �� (== Player�� �Ʒ����⿡�� ���� ��)
	virtual void TriggerEvent();
	virtual void Release() {};
};