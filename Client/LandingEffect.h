#pragma once

#include "JsonManager.h"

/// <summary>
/// ĳ���� ������ �� ����� ����Ʈ
/// 
/// 2022. 02. 24 JK
/// </summary>
class LandingEffect : public GameObject
{
	GameObject* mCharacter;
	Vector2D	mDirection;
	AnimationClip* mAnimClip;

	std::vector<Sprite*> mSprites;

public:
	LandingEffect(GameObject* target);
	virtual ~LandingEffect();

	virtual void Update() override;
	virtual void Release() override;
};