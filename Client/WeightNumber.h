#pragma once

/// <summary>
/// ü�� ���� / ���� ����Ʈ ��¿� ��ü
/// +1, +3, +5, +10 / -1, -3, -5, -10
/// 8������ ��� ����
/// 
/// 2022. 02. 25 JK
/// </summary>
class WeightNumber : public GameObject
{
	AnimationClip* mAnimClip;

	std::vector<Sprite*> mSprites;
public:
	WeightNumber(Vector2D pos, int number);
	virtual ~WeightNumber();

	virtual void Release() override;
};