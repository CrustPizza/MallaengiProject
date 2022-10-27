#pragma once

/// <summary>
/// 2022. 02. 07. MG
/// 
/// ��������Ʈ ����
/// </summary>
class Sprite
{
public:
	Sprite(int spriteSheetIndex, Vector2D pivot, D2D1_SIZE_F size, float delayTime, Vector2D posInSheet, Vector2D spritePivot = {});
	~Sprite();

	int mSpriteSheetIndex;		// �ش� ��������Ʈ�� �ִ� ��������Ʈ ��Ʈ�� �ε���

	Vector2D mPivot;			// ���������� ��� �߽���

	D2D1_SIZE_F mSize;			// ��������Ʈ ������

	float mDelayTime;			// �ִϸ��̼� ����� �� �ð� ����

	Vector2D mPosInSheet;		// ��������Ʈ ��Ʈ ���� ��ġ

	Vector2D mSpritePivot;		// ��������Ʈ�� �Ǻ�
};