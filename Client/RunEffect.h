#pragma once

#include "JsonManager.h"

/// <summary>
/// ĳ���� �̵��� ����� ���� ���� ����Ʈ
/// 
/// 2022. 02. 24 JK
/// </summary>
class RunEffect : public GameObject
{
	GameObject* mCharacter;
	Vector2D	mDirection;
	AnimationClip* mAnimClip;

	std::vector<Sprite*> mSprites;

public:
	RunEffect(GameObject* target, Vector2D direction);
	virtual ~RunEffect();

	virtual void Update() override;
	virtual void Release() override;

	void SetDirection(Vector2D direction);
};