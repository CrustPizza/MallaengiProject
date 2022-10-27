#pragma once
// #include "Vector2D.h"

/// <summary>
/// 2022. 02. 08. MG
/// 
/// ī�޶� ������
/// </summary>
class Camera
{
public:
	Camera(GameObject* gameObject);
	~Camera();
	
private:
	GameObject* mGameObject;			// ī�޶� ������ ������Ʈ(pos �ʿ�)

	// ī�޶� ���� ��ġ -> �»������ ���
	Vector2D mLeftTop;

	// ī�޶��� ũ��
	D2D1_SIZE_F mSize;
	//Vector2D mSize;

public:
	D2D1_SIZE_F GetSize() { return mSize; }			// ī�޶��� ũ�� �ޱ�

	GameObject* GetGameObject() { return mGameObject; }

	void UpdateCamera();					// ī�޶� ������Ʈ ��� ���ֱ�
};