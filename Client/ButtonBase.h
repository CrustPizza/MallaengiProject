#pragma once

/// <summary>
/// ��� ��ư���� Base Class.
/// 
/// ������ ���� �̺�Ʈ �������̵� �Լ��� �ֽ��ϴ�.
/// 
/// 2022. 02. 11. Comet
/// </summary>
class ButtonBase : public GameObject
{
public:
	ButtonBase();
	virtual ~ButtonBase();

protected:
	// Button Size (�� ģ���� Collider�� ������ �ʴ´�)
	D2D1_SIZE_F mSize;

	bool mIsClicked;

	// Ȱ��ȭ�� ������
	bool mIsActivated;

	// ������ ��ư�̳� �ƴϸ� ���� ��ư�̳�
	bool mIsSquare;

	bool IsCursorOnButton();

	// �Ǻ�
	Vector2D mPivot;

public:

	D2D1_SIZE_F GetSize() { return mSize; }

	void SetSize(D2D1_SIZE_F size) { mSize = size; }

	virtual void FixedUpdate() override;

	virtual void Update() override;

	virtual void OnButtonClicked();

	void SetActivate(bool active);
};