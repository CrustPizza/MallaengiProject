#pragma once
#include "Vector2D.h"

class GameObject;
//class Vector2D;

#define PI 3.141592f
#define DEGTORAD(degree) (degree / 180) * PI 

/// <summary>
/// 2022.02.03. MG.
/// 
/// ���� ��ǥ�� ������ Ʈ������
/// </summary>
class Transform
{
public:
	Transform(GameObject* object);
	~Transform();

private:
	GameObject* mGameObject;

	Vector2D mPosition;		// ��ġ
	Vector2D mScale;		// ������
	Vector2D mRotation;		// ȸ�� (���⺤��)

	D2D1::Matrix3x2F mWorldTransformMatrix;		// ���� ��ȯ ��Ʈ����

	D2D1::Matrix3x2F mWorldTranslateMatrix;		// ���� T ��Ʈ����		// �� 3���� ���� ��Ʈ���� �������� �ۿ��� �� �� ����?
	D2D1::Matrix3x2F mWorldRotateMatrix;		// ���� R ��Ʈ����
	D2D1::Matrix3x2F mWorldScaleMatrix;			// ���� S ��Ʈ����

public:
	GameObject* GetGameObject() { return mGameObject; }

	Vector2D GetPosition() { return mPosition; }
	void SetPosition(Vector2D position);

	Vector2D GetScale() { return mScale; }
	void SetScale(Vector2D scale);

	Vector2D GetRotation() { return mRotation; }
	void SetRotation(float angle);			// �������� ������ ��������ŭ ���ư����� (�ݽð����)
	void SetRotation(Vector2D dirVec);

	void UpdateWorldTranslateMatrix();
	void UpdateWorldRotationMatrix();
	void UpdateWorldScaleMatrix();

	void UpdateWorldTransformMatrix();

	void Update();

	D2D1::Matrix3x2F GetWorldTransformMatrix() const { return mWorldTransformMatrix; }
};