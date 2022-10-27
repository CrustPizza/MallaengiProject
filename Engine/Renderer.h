#pragma once

/// <summary>
/// 2022. 02. 08. MG
/// 
/// ��� �׸��� ���� ����ϴ� ������ Ŭ����
/// </summary>
class Renderer
{
public:
	Renderer(GameObject* object);
	virtual ~Renderer();

protected:
	GameObject* mGameObject;

	// ȭ�鿡 �� ��° ������ ����� �� ���Դϱ� ..?
	int mSortOrder;

public:
	virtual void Render();
	virtual void Release() abstract;

	// SortOrder Getter / Setter
	int GetSortOrder() const { return mSortOrder; }
	void SetSortOrder(int value) { mSortOrder = value; }

	GameObject* GetGameObject() { return mGameObject; }
};