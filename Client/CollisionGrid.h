#pragma once

#define GRID_HALF_WIDTH 1000.f
#define GRID_HALF_HEIGHT 500.f

/**
 * \brief ���带 �ɰ��� �浹�˻縦 �ǽ��� ���̴� ..
 */
class CollisionGrid : public GameObject
{
public:
	CollisionGrid(Vector2D centerPos);
	virtual ~CollisionGrid();

	// �� �׸���� �浹�� Collision���� ����
	std::vector<Collision*> mCollisions;

	AABBCollision* mCollisionOfGrid;
};