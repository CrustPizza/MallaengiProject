#pragma once
#include "Collision.h"

class GameObject;

/// <summary>
/// 2022. 02. 08. MG
/// 
/// ����� �������� �ߴµ� ����� �����ϰ� �������ڰ� ���ٰ� ����� �ȵ�. ã�ƺ���
/// 
/// => �������� �� : Override�� �� ��� ���� �����Լ��� �������̵� �ؾ��ϸ�, �θ� �߻� Ŭ���� ������ �ִ�
/// 
/// ��� ������ Include ���־�� ���� ������ ������ �ʽ��ϴ�. �̰� Ű���� ���� ..
/// </summary>
class AABBCollision : public Collision
{
public:
	AABBCollision(GameObject* object);
	~AABBCollision();

	bool CheckCollision(Collision* collision);

private:
	D2D1_SIZE_F mSize;

public:
	// Getter�� Setter�� �߰��մϴ�.
	D2D1_SIZE_F GetSize() { return mSize; }
	void SetSize(D2D1_SIZE_F value) { mSize = value; }
};