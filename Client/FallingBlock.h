#pragma once
#include "BoxBase.h"

class BlockBase;
class Platform;

/// <summary>
/// ������ �÷��̾�� �Բ� ������ ���� ����
/// 
/// ������ ������ �÷��̾�� �浹�ߴٸ� Ʈ���Ÿ� Ű�� y��ǥ�� �����ϸ鼭 �߶��Ѵ�.
/// 
/// �� ��, �÷��̾ ���� ���� ä�� �̵��ؾ��ϴ� �÷��̾��� y��ǥ�� �Բ� ���ҽ����ش�.
/// 
/// 2022. 02. 09. Comet
/// </summary>
class FallingBlock : public GameObject
{
public:
	FallingBlock(Vector2D startPos, int columnCount);
	virtual ~FallingBlock();

private:
	bool mIsTriggered;					// Ʈ���� �Ǿ����� ����

	Player* mGamePlayer;				// Ʈ���ŵǸ� ���⿡ �÷��̾ �����մϴ�.

	std::vector<BlockBase*> mBlocks;	// ��� �ؾ��ϴϱ� �÷������ üũ�Ѵ�. �� �༮���� ���� �߰��Ǿ�� ��.

	float mFallingSpeed;				// �������� �ӵ��� ��Ʈ���Ѵ�. �߷��� �޴� ��ó�� �����մϴ�.

	int mColumnCount;

	float mTimer;						// ���� �ð� ������ ���־���

public:

	// �߶��� ������ ���� ������Ʈ������ �� �ȿ� �ִ� ���� ������Ʈ���� �����Դϴ�.
	std::vector<BlockBase*>& GetPlatforms() { return mBlocks; }

	virtual void FixedUpdate() override;
	virtual void Update() override;

	virtual void LateUpdate() override;
};