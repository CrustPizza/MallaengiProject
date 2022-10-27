#pragma once

/**
 * \brief : Ʈ���ŵǸ� ���ư��� ������ ~ 2022. 02. 21. Comet
 */
class TriggerPigeon : public GameObject
{
public:
	TriggerPigeon(std::string color);
	virtual ~TriggerPigeon();

private:
	std::string mColor;

	bool mTrigger;

	float mTimer;

	// �÷��̾ �����Ѵ�.
	Player* mGamePlayer;

	std::vector<AnimationClip*> mAnimationClips;

	void LoadAnimation();
	
public:
	virtual void FixedUpdate() override;
	virtual void Update() override;
};