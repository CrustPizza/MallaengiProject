#pragma once

class FallingDum;
class EnemyBase;
class ItemBox;
// Blocks
class BlockBase;
class BreakableBlock;

// Platforms
class Platform;
class BlockPlatform;

//Box
class BoxBase;
class TransBox;

class RunEffect;
class LandingEffect;

// Simple Physics
#define MAX_FALLING_SPEED 700.f
// #define MOVE_SPEED 150.f ����
#define MOVE_SPEED 350.f
// #define JUMP_POWER 500.f ����
#define JUMP_POWER 1000.f
#define GRAVITY 2400.f

// ���� �÷����� �����ϱ� ���� ������ (�̰� ��Ʈ���� �� ������ �÷��� �ڵ� ������ �ʿ䰡 �ֽ��ϴ� ..)
#define ADJUST_X 15.f
// #define ADJUST_Y 15.f	// ��, �� �浹�� ���ο��� ���� �� �����Ƿ� Y�� ���� �� ���ش�.
#define ADJUST_Y 30.f	// ��, �� �浹�� ���ο��� ���� �� �����Ƿ� Y�� ���� �� ���ش�.
#define PLAYER_WIDTH 45.f
#define PLAYER_HEIGHT 125.f

// �ڽ� �Ʒ����� �浹 �� �з��� �� �ݹ߷� (�־���ұ� ..?
#define BLOCKING_POWER 100.f

// �÷��̾� �ܰ� �� ������ �� ����
#define PLAYER_TOTAL_STEP 3
#define PLAYER_TOTAL_STATE 4

// Character Scale Control
#define PLAYER_SCALE 1.2f

// Trans�ϴµ� �ɸ��� �� �ð�
#define TRANS_TIME 2.3f

// Fade In-Out Time
#define FADE_TIME 1.7f

// ������ ������Ʈ�� ���ΰ��� 3������ �ܰ踦 ������. 
enum class PlayerStep
{
	Three = 0,		// ������
	Two = 1,		// ������
	One = 2		// ������
};

// ������ ������Ʈ�� ���ΰ��� 5������ ���¸� ������.
enum class PlayerState
{
	Idle = 0,
	Run = 1,
	Jump = 2,
	Die = 3,
	Trans = 4		// ��ȭ ..!
};

// �÷��̾� �ܰ躰 �ӵ�, ���ӵ��� ������ ��ĥ ����
#define RESISTANCE 0.15f

/**
 * ������ ������Ʈ�� ���ΰ� Ŭ����
 *
 * 2022. 02. 05. Comet
 */
class Player : public GameObject
{
public:
	Player();
	virtual ~Player();

	RunEffect* mRunEffect;
	LandingEffect* mLandingEffect;

private:
	// ���� ��� !!
	bool mInvincible;

	// �÷��̾��� �̵� ���� ���� (�̵��� ���ٸ� Zero / �̵��� �ִٸ� Normal Vector)
	Vector2D mDirVector;

	// Update �� ��ġ
	Vector2D mBeforePosition;

	// ������ �� �� �ִ� �����ΰ� ..?
	bool mIsJumpable;

private:
	// ---------------------------- Player FSM ----------------------------
	// ���� Player State
	PlayerState mCurrentState;

	// �� �����ӿ����� Player State
	PlayerState mPrevState;

	// ���� Player Step
	PlayerStep mCurrentStep;

	// �� �����ӿ����� Player Step
	PlayerStep mPrevStep;

	// �÷��̾��� ���. ������ ������ �ϴ�.
	int mLife;

public:
	int GetPlayerLife() { return mLife; }

	// State Ʈ�������� �Ͼ �� �ִϸ��̼��� �˸°� �������ְ� �ٲ�� ..!
	void Idle();
	void Run();
	void Jump();
	void Die();
	void Trans();

	// �׾�����.
	bool mIsFirstDieLoop;

	// Player �ܰ� -> ���º� Animation
	std::vector<std::vector<AnimationClip*>> mAnimationClips;

public:
	std::vector <std::vector<AnimationClip*>> GetAnimationClipsOfPlayer() { return mAnimationClips; }
private:

	// Player�� ������ �� Animation Load�� ����ϴ� Func
	void LoadAnimationProcess();

	// ���� ���¸� üũ�Ͽ� ���������� ���� ��� �̿� ���õ� �̺�Ʈ�� �����մϴ�.
	void StateChangeProcess();
	// �����Ը� üũ�Ͽ� Step�� ������ ���� ��� �̿� ���õ� �̺�Ʈ�� �����մϴ�.
	void StepChangeProcess();

	// Hp
	int mHp;

	// ������
	float mBodyWeight;

	float mPrevWeight;

public:
	PlayerStep GetPrevStep() { return mPrevStep; }
	PlayerStep GetCurrentStep() { return mCurrentStep; }

	PlayerState GetCurrentState() { return mCurrentState; }

	// �ε��� �� step�� �����ش�. �ֳ��ϸ� ���̺�� ���� �ε�� �� �ܰ谡 �ٸ� �� ���� ..!
	void SetPlayerStep(PlayerStep step)
	{
		mCurrentStep = step;
		mPrevStep = step;
	}
	// ---------------------------- Player FSM ----------------------------
public:
	int GetHp() { return mHp; }
	void SetHp(int value) { if (mInvincible) return; mHp = value; }

	float GetBodyWeight() { return mBodyWeight; }

	void SetBodyWeight(float value) { mBodyWeight = value; if (mBodyWeight <= 60.f) mBodyWeight = 60.f; }
	void PlusBodyWeight(float delta) { mBodyWeight += delta; if (mBodyWeight <= 60.f) mBodyWeight = 60.f; }

private:
	// ---------------------------- Simple Physics ----------------------------
	// �ʴ� ��� ������ �Ÿ��� �̵��ϴ°�
	Vector2D mVelocity;
	
	// �߷�
	float mGravity;

	// ������
	float mJumpPower;

	// ������ư�� ������ �־��� �ð�
	float mJumpPressedTime;

	// ����Ű�� ���ȳ��� ..?
	bool mIsJumpKeyPressed;
	
	// Walk or Dash�� �Ѵٴ� ���� �ӵ��� x-value�� ����ϸ� �Ǵ� �� ..!
	float mMaxWalkSpeed;
	// ---------------------------- Simple Physics ----------------------------

public:
	void InitPlayer();
	// ���� ���� ..
	virtual void FixedUpdate() override;

	virtual void Update() override;

	// �÷��̾��� �̵� ���� ���͸� ��ȯ�Ѵ�.
	Vector2D GetDirVector() { return mDirVector; }

	void SetJumpable() { mIsJumpable = true; }
	bool GetIsJumpable() { return mIsJumpable; }

	// Velocity Getter, Setter
	Vector2D GetVelocity() { return mVelocity; }
	void SetVelocity(Vector2D value) { mVelocity = value; }

	PlayerState GetPlayerState() { return mCurrentState; }
	void SetPlayerState(PlayerState state) { mCurrentState = state; }

	// Block Process
	void BlockCollisionProcess(BlockBase* block);
	void BreakableBlockCollisionProcess(BreakableBlock* breakableBlock);

	// Platform Process (��, ��, �Ϸδ� ���� ���� / �������� ����)
	void PlatformCollisionProcess(Platform* platform);
	void BlockPlatformCollisionProcess(BlockPlatform* blockplatform);

	// Box Process
	void BoxCollisionProcess(BoxBase* box);
	void TransBoxCollisionProcess(TransBox* transBox);
	void ItemBoxCollisionProcess(ItemBox* itemBox);

	// Enemy Process
	void EnemyCollsionProcess(EnemyBase* enemy);
	
private:
	// IntroScene���� ���������ϴ� Ư���� ���ǵ��� ��´�.
	void IntroSceneProcess();

	void StageProcess();

private:
	// ���� �ش�Ǵ� ���������� �󸶳� �÷����߳���
	float mStagePlayTime;

	// �������� ������ ���� ��
	float mAcculmulTime;

	void TimeProcess();

public:
	float GetStagePlayTime() { return mStagePlayTime; }	// UI : In-Game �ð迡�� ȣ��

	void SetStagePlayTime(float value) { mStagePlayTime = value; }	// �������� �� ����� �� ȣ�� (���� 0���� ����� ����)
};

// 1�ʿ� ��� ������ �����Ը� �� ..?
#define GAIN_PER_SEC 1.0f		// 10�� �� 1kg