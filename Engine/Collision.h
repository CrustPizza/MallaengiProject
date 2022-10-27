#pragma once
// Collision�� �浹 üũ ��ɸ� �ִ´� ..!

class GameObject;

/// <summary>
/// 2022. 02. 08. MG
/// 
/// �浹 üũ�� ���� �浹 ���̽� Ŭ����
/// </summary>
class Collision
{
public:
	Collision(GameObject* object);
	~Collision();

private:
	GameObject* mGameObject;

	bool mIsCollision;																// �浹 ��?����?

	std::vector<GameObject*> mCollisionInfo;										// �浹 ������ ���� ����

public:
	/// ����� �� ���� ���۷��� �ȵȴ�. ���������� ���� �ʴ´�!
	virtual bool CheckCollision(Collision* collision) = 0;							// �浹 �ߴ��� ���ߴ��� üũ

	GameObject* GetGameObject() { return mGameObject; }

	bool GetIsCollision() { return mIsCollision; }
	void SetIsCollision(bool value) { mIsCollision = value; }						// �浹 �� ����

	std::vector<GameObject*>& GetCollisionInfo() { return mCollisionInfo; }			// �� ����ϰ� Get -> &(����) �� ����ϸ� ���ϰ� ���� ����ϰ� ����� ����
	void AddCollisionInfo(GameObject* object) { mCollisionInfo.push_back(object); } // �浹���� �ޱ�
};