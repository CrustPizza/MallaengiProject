#include "pch.h"
#include "Transform.h"

Transform::Transform(GameObject* object)
	: mGameObject(object),
	mPosition(Vector2D(0.f, 0.f)),
	mRotation(Vector2D(1.f, 0.f)),
	mScale(Vector2D(1.f, 1.f))
{
	UpdateWorldTranslateMatrix();
	UpdateWorldRotationMatrix();
	UpdateWorldScaleMatrix();
}

Transform::~Transform()
{

}

void Transform::SetPosition(Vector2D position)
{
	// ���� �־ ����
	mPosition = position;

	UpdateWorldTranslateMatrix();
}

void Transform::SetScale(Vector2D scale)
{
	mScale = scale;

	UpdateWorldScaleMatrix();
}

void Transform::SetRotation(float angle)
{
	// ȣ���� ��� (�ޱ� ��ȯ)
	float rad = DEGTORAD(angle);

	// ��ǥ ����
	float newX = mRotation.GetX() * cos(rad) + mRotation.GetY() * sin(rad);
	float newY = -mRotation.GetX() * sin(rad) + mRotation.GetY() * cos(rad);

	// ���� ��ǥ��(����) ũ�� -> 1�� �ٲ� �� ���ֱ�
	mRotation = Vector2D(newX, newY).Normalize();

	// �ٲ� ���⺤�ͷ� �ʱ�ȭ
	UpdateWorldRotationMatrix();
}

void Transform::SetRotation(Vector2D dirVec)
{
	mRotation = dirVec;

	UpdateWorldRotationMatrix();
}

void Transform::UpdateWorldTranslateMatrix()
{
	// ������� �����
	mWorldTranslateMatrix = D2D1::Matrix3x2F::Identity();

	// x, y ��ǥ��ŭ �̵�
	mWorldTranslateMatrix.dx = mPosition.GetX();
	mWorldTranslateMatrix.dy = mPosition.GetY();

	// ������ǥ�� �ٲ���İ� ����
	UpdateWorldTransformMatrix();
}

void Transform::UpdateWorldRotationMatrix()
{
	// ������� �����
	mWorldRotateMatrix = D2D1::Matrix3x2F::Identity();

	mWorldRotateMatrix._11 = mRotation.GetX();
	mWorldRotateMatrix._12 = mRotation.GetY();
	mWorldRotateMatrix._21 = -mRotation.GetY();
	mWorldRotateMatrix._22 = mRotation.GetX();

	// ������ǥ�� �ٲ���İ� ����
	UpdateWorldTransformMatrix();
}

void Transform::UpdateWorldScaleMatrix()
{
	// ������� �����
	mWorldScaleMatrix = D2D1::Matrix3x2F::Identity();

	mWorldScaleMatrix._11 = mScale.GetX();
	mWorldScaleMatrix._22 = mScale.GetY();

	// ������ǥ�� �ٲ� ��İ� ����
	UpdateWorldTransformMatrix();
}

void Transform::UpdateWorldTransformMatrix()
{
	mWorldTransformMatrix = mWorldScaleMatrix * mWorldRotateMatrix * mWorldTranslateMatrix;
}