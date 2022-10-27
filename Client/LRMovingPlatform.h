#pragma once

class BlockBase;
class Platform;

// �����̴� �÷����� ��ӿ���� �������ϴ�.
#define LR_MOVING_SPEED 70.f
#define LR_COLUMN_COUNT 4
#define LR_MOVING_TIME 3.f

/// <summary>
/// ��, ��� �����̴� Platform ..
/// 
/// 2022. 02. 10. Comet Lee
/// </summary>
class LRMovingPlatform : public GameObject
{
public:
	// ù ���� �����ϴ� ��ġ�� �޾Ƽ� ���η� ������ ������ �÷����� ��ġ�մϴ�.
	LRMovingPlatform(Vector2D startPos, int moveTile, int column, float idleTime);
	virtual ~LRMovingPlatform();

private:
	std::vector<BlockBase*> mPlatforms;	// ��� �ؾ��ϴϱ� �÷������ üũ�Ѵ�. �� �༮���� ���� �߰��Ǿ�� ��.

	// ������ �ð����� ��� ����, ������ �ð����� �·� ���� �ݺ�
	float mMoveTime;
	// ������ Control �մϴ� ..
	int mDirControl;

	int mMoveTile;

	int mColumn;

	float mIdleTime;

	// �÷��̾ ���� �� �÷����� �� �ϳ��� �浹 ���̶��, ��ǥ�� �Ű��ش�.

public:
	std::vector<BlockBase*>& GetPlatforms() { return mPlatforms; }

	virtual void FixedUpdate() override;

	virtual void Update() override;
};