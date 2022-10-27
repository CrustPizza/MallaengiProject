#pragma once

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
class FallingTrap : public GameObject
{
public:
	FallingTrap(Vector2D startPos);
	virtual ~FallingTrap();

private:
	bool mIsTriggered;					// Ʈ���� �Ǿ����� ����
	GameObject* mPlayer;				// Ʈ���ŵǸ� ���⿡ �÷��̾ �����մϴ�.

	std::vector<Platform*> mPlatforms;	// ��� �ؾ��ϴϱ� �÷������ üũ�Ѵ�. �� �༮���� ���� �߰��Ǿ�� ��.

	float mFallingSpeed;				// �������� �ӵ��� ��Ʈ���Ѵ�. �߷��� �޴� ��ó�� �����մϴ�.

public:
	
	// �߶��� ������ ���� ������Ʈ������ �� �ȿ� �ִ� ���� ������Ʈ���� �����Դϴ�.
	std::vector<Platform*>& GetPlatforms() { return mPlatforms; }

	virtual void FixedUpdate() override;
	virtual void Update() override;
}; 