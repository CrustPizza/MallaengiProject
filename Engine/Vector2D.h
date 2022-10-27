#pragma once

/// <summary>
/// 2022. 02. 08. MG
/// Vector2D�� ������ ������ �� �� ������ �չ� �������� �ȴ�!
/// </summary>
class Vector2D
{
public:
	Vector2D() { this->mX = 0.f; this->mY = 0.f; }
	Vector2D(const Vector2D& other) { this->mX = other.mX; this->mY = other.mY; }
	Vector2D(float x, float y) { this->mX = x; this->mY = y; }
	~Vector2D() { };

public:
	float mX;		// x
	float mY;		// y

	static float mVectorEpsilon;	// ������ �� ���꿡 �־� ���� ��� ����

public:
	inline float GetX() const { return this->mX; }
	inline float GetY() const { return this->mY; }

	float GetMagnitude() const;			// ������ ũ�⸦ ���ϴ� �Լ�
	Vector2D& Normalize();				// ������ ũ�⸦ 1�� ����� �Լ�

	Vector2D operator + (const Vector2D& other);
	Vector2D operator - (const Vector2D& other);
	Vector2D operator / (float scalar);
	bool operator == (Vector2D& other) const;
	bool operator != (Vector2D& other) const;
	Vector2D& operator+=(const Vector2D& other);
	Vector2D& operator-=(const Vector2D& other);
	Vector2D operator = (Vector2D other);
	Vector2D operator * (float scalar);

	float DistanceTo(Vector2D& to) const;			// ���Ͱ��� �Ÿ� ���ϴ� �Լ�

	float DotProduct(Vector2D other);				// ���Ͱ��� ����

	friend Vector2D operator * (float scalar, Vector2D vector);

	// �⺻ �����ϴ� ����
	static Vector2D Zero;
	static Vector2D Up;
	static Vector2D Left;
	static Vector2D Down;
	static Vector2D Right;
};