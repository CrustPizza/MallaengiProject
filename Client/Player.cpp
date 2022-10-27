#include "PreCompiledHeader.h"

// Blocks
#include "BlockBase.h"
#include "BreakableBlock.h"

// Platforms
#include "Platform.h"
#include "BlockPlatform.h"

// Box
#include "TransBox.h"
#include "ItemBox.h"

// Player
#include "Player.h"

// Enemy
#include "EnemyBase.h"

// Effect
#include "FadeOutObject.h"
#include "FreeEnemy.h"
#include "RunEffect.h"
#include "LandingEffect.h"
#include "WeightNumber.h"

Player::Player() :
	mDirVector(Vector2D::Zero),
	mBeforePosition(Vector2D::Zero),
	mIsJumpable(false),
	mInvincible(false),
	mCurrentState(PlayerState::Idle),
	mPrevState(PlayerState::Idle),
	mCurrentStep(PlayerStep::Two),
	mPrevStep(PlayerStep::Two),
	mLife(100),							// ���� ��� ��
	mIsFirstDieLoop(false),
	mHp(1),
	mBodyWeight(105.f),				// ���� ������
	mPrevWeight(105.f),
	mVelocity(Vector2D::Zero),
	mGravity(GRAVITY),
	// MaxWalkSpeed�� JumpPower�� ���� �����ο� �´� �������� ����� �� �ʿ䰡 �ֽ��ϴ�.
	// �ƴϸ� �ܰ躰�� �α�
	mJumpPower(JUMP_POWER* (static_cast<int>(mCurrentStep) + 1)),
	mMaxWalkSpeed(MOVE_SPEED* (static_cast<int>(mCurrentStep) + 1)),
	mJumpPressedTime(0.1f),
	mIsJumpKeyPressed(false),
	mAnimationClips(std::vector<std::vector<AnimationClip*>>(PLAYER_TOTAL_STEP + 1, std::vector<AnimationClip*>(PLAYER_TOTAL_STATE))),
	mRunEffect(nullptr),
	mLandingEffect(nullptr),
	mAcculmulTime(0.f)
{
	// ������ �� �� ������Ʈ�� �ʿ��� ������Ʈ�� �غ� ��� ���־�� �մϴ�.
	AddSpriteRenderer()->SetSortOrder(4);

	// �̰Ŵ� ������ Sprite Tool�� �ǰ� ������ ���� �����Դϴ�.
	AddAABBCollision()->SetSize({ PLAYER_WIDTH, PLAYER_HEIGHT });

	Animator* animator = AddAnimator();

	LoadAnimationProcess();

	animator->SetAnimationClip(Game::GetInstance().GetResource()->GetAnimationClip("PLAYER_TWO_IDLE"));

	GetTransform()->SetPosition(GetTransform()->GetPosition() + Vector2D(100.f, 100.f));

	GetTransform()->SetScale(Vector2D(PLAYER_SCALE, PLAYER_SCALE));

	// ���� �߿��� �±� ����
	SetTag("Player");
}

Player::~Player()
{
}

void Player::Idle()
{
	if (Input::GetInstance().GetKeyState(VK_LEFT) == KEYSTATE::HOLD)
	{
		mVelocity.mX = -mMaxWalkSpeed;

		mTransform->SetScale(Vector2D(-PLAYER_SCALE, PLAYER_SCALE));

		mCurrentState = PlayerState::Run;
	}
	else if (Input::GetInstance().GetKeyState(VK_RIGHT) == KEYSTATE::HOLD)
	{
		mVelocity.mX = mMaxWalkSpeed;

		mTransform->SetScale(Vector2D(PLAYER_SCALE, PLAYER_SCALE));

		mCurrentState = PlayerState::Run;
	}
	else if ((mIsJumpable) && (Input::GetInstance().GetKeyState(VK_SPACE) == KEYSTATE::DOWN))
	{
		mIsJumpKeyPressed = true;

		mIsJumpKeyPressed = true;

		mVelocity.mY = -mJumpPower * (1 + mJumpPressedTime * 2);

		mJumpPressedTime = 0.1f;

		mCurrentState = PlayerState::Jump;

		mIsJumpKeyPressed = false;

		SoundManager::GetInstance()->Play(500012);

		mIsJumpable = false;
	}

	return;
}

void Player::Run()
{
	if ((Input::GetInstance().GetKeyState(VK_LEFT) == KEYSTATE::UP) ||
		(Input::GetInstance().GetKeyState(VK_LEFT) == KEYSTATE::NONE) ||
		(Input::GetInstance().GetKeyState(VK_RIGHT) == KEYSTATE::UP) ||
		(Input::GetInstance().GetKeyState(VK_RIGHT) == KEYSTATE::NONE))
	{
		mCurrentState = PlayerState::Idle;
	}

	// �Է¿� ���� �ӵ� ���� (Walk State / FSM���� �� �����Դϴ�.)
	if (Input::GetInstance().GetKeyState(VK_LEFT) == KEYSTATE::HOLD)
	{
		mVelocity.mX = -mMaxWalkSpeed;

		mTransform->SetScale(Vector2D(-PLAYER_SCALE, PLAYER_SCALE));

		mCurrentState = PlayerState::Run;
		if (SoundManager::GetInstance()->Playing(500011) != true)
		{
			SoundManager::GetInstance()->Play(500011);
		}

		// �÷��̾� �̵��ÿ� ��µǴ� ����Ʈ
		if (mRunEffect == nullptr)
		{
			mRunEffect = new RunEffect(this, Vector2D::Left);
			Game::GetInstance().GetCurrentScene()->AddGameObject(mRunEffect);
		}
		else
		{
			mRunEffect->SetDirection(Vector2D::Left);
		}
	}
	else if (Input::GetInstance().GetKeyState(VK_RIGHT) == KEYSTATE::HOLD)
	{
		mVelocity.mX = mMaxWalkSpeed;

		mTransform->SetScale(Vector2D(PLAYER_SCALE, PLAYER_SCALE));

		mCurrentState = PlayerState::Run;
		if (SoundManager::GetInstance()->Playing(500011) != true)
		{
			SoundManager::GetInstance()->Play(500011);
		}

		// �÷��̾� �̵��ÿ� ��µǴ� ����Ʈ
		if (mRunEffect == nullptr)
		{
			mRunEffect = new RunEffect(this, Vector2D::Right);
			Game::GetInstance().GetCurrentScene()->AddGameObject(mRunEffect);
		}
		else
		{
			mRunEffect->SetDirection(Vector2D::Right);
		}
	}

	if ((mIsJumpable) && (Input::GetInstance().GetKeyState(VK_SPACE) == KEYSTATE::DOWN))
	{
		mVelocity.mY = -mJumpPower * (1 + mJumpPressedTime * 2);

		mJumpPressedTime = 0.1f;

		SoundManager::GetInstance()->Play(500012);

		mCurrentState = PlayerState::Jump;

		mIsJumpable = false;
	}

	return;
}

void Player::Jump()
{
	// �Է¿� ���� �ӵ� ����, ���� ���¿����� �������� �����ϴ� ..?
	if (Input::GetInstance().GetKeyState(VK_LEFT) == KEYSTATE::HOLD)
	{
		mVelocity.mX = -mMaxWalkSpeed;

		mTransform->SetScale(Vector2D(-PLAYER_SCALE, PLAYER_SCALE));

	}
	else if (Input::GetInstance().GetKeyState(VK_RIGHT) == KEYSTATE::HOLD)
	{

		mVelocity.mX = mMaxWalkSpeed;

		mTransform->SetScale(Vector2D(PLAYER_SCALE, PLAYER_SCALE));
	}

	// �ٽ� ������ ���������ٴ� ���� �ϴ��� �÷����� ��Ҵٴ� ��, ���� Idle�� Transition�մϴ�.
	if (mIsJumpable)
	{
		if (mLandingEffect == nullptr)
		{
			mLandingEffect = new LandingEffect(this);
			Game::GetInstance().GetCurrentScene()->AddGameObject(mLandingEffect);
		}

		mCurrentState = PlayerState::Idle;
	}

	return;
}

void Player::Die()
{
	//Die sound

	// �÷��̾ �װ� �Ǿ��� ���� Process�� ���⼭ �����ϰ� �˴ϴ�.

	// ����� �ϳ� ���̰�, �ٽ� �����ϴ��� ..? �׷� �������� �����ϰ� �� �� �����ϴ�.

	// ������ ���̵� �ƿ� ����

	// 1. ������ �������� �ʱ�
	mVelocity = Vector2D(0.f, 0.f);

	if (!mIsFirstDieLoop)
	{
		mIsFirstDieLoop = true;
		SoundManager::GetInstance()->Play(500017);

		mLife -= 1;

		// TODO : ���� �÷��� ���� ���������� ����ؾ��Ѵ�. �׸��� �ε��� �� ���̺�� ��ġ, �����Է� �ٽ� ������ �� �ֵ��� �ۼ� �ʿ�
		std::string name = Game::GetInstance().GetCurrentScene()->GetSceneName();

		FadeOutObject* fadeOut = new FadeOutObject(name + "DieScene");

		Game::GetInstance().GetCurrentScene()->AddGameObject(fadeOut);
	}

	static float timer = 0;

	timer += Timer::GetInstance().GetDeltaTime() / 1000.f;

	if (timer >= FADE_TIME)
	{
		timer = 0;

		mIsFirstDieLoop = false;

		mCurrentState = PlayerState::Idle;
	}

	return;
}

void Player::Trans()
{
	static float timer = 0;

	timer += Timer::GetInstance().GetDeltaTime() / 1000.f;

	if (timer >= TRANS_TIME)
	{
		mCurrentState = PlayerState::Idle;

		timer = 0;

		// Ʈ������ ������ !
		GetAnimator()->SetAnimationClip(mAnimationClips[static_cast<int>(mCurrentStep)][static_cast<int>(mCurrentState)]);

		//������ ����,���� ��ȭ �� �� ���� ȿ����


		/*mMaxWalkSpeed = MOVE_SPEED * (static_cast<int>(mCurrentStep) + 1);
		mJumpPower = JUMP_POWER * (static_cast<int>(mCurrentStep) + 1);*/
	}
}

void Player::LoadAnimationProcess()
{
	// �÷��̾� �ܰ� ���濡 ���� ���� ������ ����ϰ� ���� ���ؼ� �÷��̾ �ڱ� �ڽ��� �ൿ �ִϸ��̼���
	// ���� �ֵ��� �ϰڽ��ϴ�.
	mAnimationClips[0][0] = Game::GetInstance().GetResource()->GetAnimationClip("PLAYER_THREE_IDLE");
	mAnimationClips[0][1] = Game::GetInstance().GetResource()->GetAnimationClip("PLAYER_THREE_RUN");
	mAnimationClips[0][2] = Game::GetInstance().GetResource()->GetAnimationClip("PLAYER_THREE_JUMP");
	mAnimationClips[0][3] = Game::GetInstance().GetResource()->GetAnimationClip("PLAYER_THREE_DIE");

	mAnimationClips[1][0] = Game::GetInstance().GetResource()->GetAnimationClip("PLAYER_TWO_IDLE");
	mAnimationClips[1][1] = Game::GetInstance().GetResource()->GetAnimationClip("PLAYER_TWO_RUN");
	mAnimationClips[1][2] = Game::GetInstance().GetResource()->GetAnimationClip("PLAYER_TWO_JUMP");
	mAnimationClips[1][3] = Game::GetInstance().GetResource()->GetAnimationClip("PLAYER_TWO_DIE");

	mAnimationClips[2][0] = Game::GetInstance().GetResource()->GetAnimationClip("PLAYER_ONE_IDLE");
	mAnimationClips[2][1] = Game::GetInstance().GetResource()->GetAnimationClip("PLAYER_ONE_RUN");
	mAnimationClips[2][2] = Game::GetInstance().GetResource()->GetAnimationClip("PLAYER_ONE_JUMP");
	mAnimationClips[2][3] = Game::GetInstance().GetResource()->GetAnimationClip("PLAYER_ONE_DIE");

	// Trans !!
	mAnimationClips[3][0] = Game::GetInstance().GetResource()->GetAnimationClip("THREE_TO_TWO");
	mAnimationClips[3][1] = Game::GetInstance().GetResource()->GetAnimationClip("TWO_TO_THREE");
	mAnimationClips[3][2] = Game::GetInstance().GetResource()->GetAnimationClip("TWO_TO_ONE");
	mAnimationClips[3][3] = Game::GetInstance().GetResource()->GetAnimationClip("ONE_TO_TWO");
}

void Player::StateChangeProcess()
{
	// ����, ���°� ����Ǿ��ٸ� PlayerStep, CurrentState�� �ش��ϴ� Animation���� ��ü�մϴ�.

	if (mCurrentState == PlayerState::Trans)
		return;

	if (mPrevState != mCurrentState)
		GetAnimator()->SetAnimationClip(mAnimationClips[static_cast<int>(mCurrentStep)][static_cast<int>(mCurrentState)]);
}

void Player::StepChangeProcess()
{
	if ((mCurrentState == PlayerState::Die) || (mCurrentState == PlayerState::Jump) || (!mIsJumpable))
		return;

	if (mBodyWeight < 100.f)
		mCurrentStep = PlayerStep::One;
	else if ((100.f <= mBodyWeight) && (mBodyWeight < 120.f))
		mCurrentStep = PlayerStep::Two;
	else if ((120.f <= mBodyWeight) && (mBodyWeight < 140.f))
		mCurrentStep = PlayerStep::Three;
	else if ((140.f <= mBodyWeight))			// 140kg �Ѿ�� ��ȹ �ǵ��� �°� ���� �̺�Ʈ�� �ǽ��ϸ� �ȴ�.
	{
		// Die �ϴ� ���� ���ڽ��ϴ�.
		mCurrentState = PlayerState::Die;

		mHp = 0;
	}

	if (mPrevStep != mCurrentStep)
	{
		mCurrentState = PlayerState::Trans;

		// ������ �� �� �ִ�.
		mTransform->SetScale(Vector2D(PLAYER_SCALE, PLAYER_SCALE));

		if ((mPrevStep == PlayerStep::Three) && (mCurrentStep == PlayerStep::Two))
		{
			SoundManager::GetInstance()->Play(500020);
			GetAnimator()->SetAnimationClip(mAnimationClips[3][0]);
		}
		else if ((mPrevStep == PlayerStep::Two) && (mCurrentStep == PlayerStep::Three))
		{
			SoundManager::GetInstance()->Play(500019);
			GetAnimator()->SetAnimationClip(mAnimationClips[3][1]);
		}
		else if ((mPrevStep == PlayerStep::Two) && (mCurrentStep == PlayerStep::One))
		{
			SoundManager::GetInstance()->Play(500020);
			GetAnimator()->SetAnimationClip(mAnimationClips[3][2]);
		}
		else
		{
			SoundManager::GetInstance()->Play(500019);
			GetAnimator()->SetAnimationClip(mAnimationClips[3][3]);
		}
	}
}

void Player::InitPlayer()
{
	mDirVector = Vector2D::Zero;
	mBeforePosition = Vector2D::Zero;
	mIsJumpable = false;
	mInvincible = false;
	mCurrentState = PlayerState::Idle;
	mPrevState = PlayerState::Idle;
	mCurrentStep = PlayerStep::Two;
	mPrevStep = PlayerStep::Two;
	mLife = 100;							// ���� ��� ��
	mIsFirstDieLoop = false;
	mHp = 1;
	mBodyWeight = 105.f;				// ���� ������
	mPrevWeight = mBodyWeight;
	mVelocity = Vector2D::Zero;

	// MaxWalkSpeed�� JumpPower�� ���� �����ο� �´� �������� ����� �� �ʿ䰡 �ֽ��ϴ�.
	// �ƴϸ� �ܰ躰�� �α�
	mJumpPower = JUMP_POWER * (static_cast<int>(mCurrentStep) + 1);
	mMaxWalkSpeed = MOVE_SPEED * (static_cast<int>(mCurrentStep) + 1);
	mJumpPressedTime = 0.1f;
	mIsJumpKeyPressed = false;

	mRunEffect = nullptr;
	mLandingEffect = nullptr;

	// �̰Ŵ� ������ Sprite Tool�� �ǰ� ������ ���� �����Դϴ�.
	AddAABBCollision()->SetSize({ PLAYER_WIDTH, PLAYER_HEIGHT });

	Animator* animator = GetAnimator();

	animator->SetAnimationClip(Game::GetInstance().GetResource()->GetAnimationClip("PLAYER_TWO_IDLE"));

	GetTransform()->SetPosition(GetTransform()->GetPosition() + Vector2D(100.f, 100.f));

	GetTransform()->SetScale(Vector2D(PLAYER_SCALE, PLAYER_SCALE));
}

void Player::FixedUpdate()
{
	GameObject::FixedUpdate();

	//�ܰ躰 �̵��ӵ� ������ ��Ʈ��
	if (mCurrentStep == PlayerStep::One)
	{
		mMaxWalkSpeed = MOVE_SPEED + 150.f;
		mJumpPower = JUMP_POWER + 150.f;
	}
	else if (mCurrentStep == PlayerStep::Two)
	{
		mMaxWalkSpeed = MOVE_SPEED + 100.f;
		mJumpPower = JUMP_POWER + 100.f;
	}
	else if (mCurrentStep == PlayerStep::Three)
	{
		mMaxWalkSpeed = MOVE_SPEED;
		mJumpPower = JUMP_POWER;
	}

	//// �ӵ� ��Ʈ�ѷ�
	//mMaxWalkSpeed = MOVE_SPEED * (static_cast<int>(mCurrentStep) + 1);
	//mJumpPower = JUMP_POWER * (static_cast<int>(mCurrentStep) + 1);

	// 0. ������Ʈ �� ���� ��ġ�� ����صӴϴ�.
	mBeforePosition = mTransform->GetPosition();

	// 1-1. �߷� ó�� (�ִ� �������� �ӵ��� ��Ʈ���մϴ�.)
	if (mVelocity.mY <= MAX_FALLING_SPEED)
	{
		mVelocity.mY = mVelocity.mY + mGravity * Timer::GetInstance().GetDeltaTime() / 1000.f;
	}

	// ---------------------------- Player FSM ----------------------------
	// 1. Update ��, PrevState�� PrevStep�� ��ü�� �ǽ��մϴ�.
	mPrevState = mCurrentState;
	mPrevStep = mCurrentStep;

	// Player State�� ���� �з�
	switch (mCurrentState)
	{
	case PlayerState::Idle:
	{
		Idle();
		break;
	}

	case PlayerState::Run:
	{
		Run();
		break;
	}

	case PlayerState::Jump:
	{
		Jump();
		break;
	}

	case PlayerState::Die:
	{
		Die();
		break;
	}

	case PlayerState::Trans:
	{
		Trans();
		break;
	}

	default:
	{
		break;
	}
	}

	// �� �����Ӹ��� ���ش�. Update���� �ٴ��� �ִٰ� �򰡰� �Ǿ��� ���� ������ �� �ֱ⸦ �ٶ�� �����̴�.
	mIsJumpable = false;

	// 1. ���� �ӵ��� ���� �̵� ó��
	mTransform->SetPosition(mTransform->GetPosition() + (Timer::GetInstance().GetDeltaTime() / 1000.f) * mVelocity);

	mVelocity.mX = 0.f;
}

void Player::Update()
{
	

	GameObject::Update();

	// �ð� �ý��� ������Ʈ
	TimeProcess();

	// �׽�Ʈ �ڵ�
#ifdef _DEBUG || DEBUG
	if (Input::GetInstance().GetKeyState(VK_F1) == KEYSTATE::DOWN)
		mBodyWeight += 1.0f;
	else if (Input::GetInstance().GetKeyState(VK_F2) == KEYSTATE::DOWN)
		mBodyWeight -= 1.0f;
	else if (Input::GetInstance().GetKeyState(VK_F3) == KEYSTATE::DOWN)
		mInvincible = !mInvincible;
#endif

	// Hp ������ 0 �����̸�, Die Process�� �����Ѵ�.
	if (mHp <= 0)
		mCurrentState = PlayerState::Die;

	if (abs(mPrevWeight - mBodyWeight) >= 1.0f)
	{
		int sign = 1;

		if (mPrevWeight <= mBodyWeight)
			sign = 1;
		else
			sign = -1;

		while (abs(mPrevWeight - mBodyWeight) >= 1.0f)
		{
			if (abs(mPrevWeight - mBodyWeight) >= 10.0f)
			{
				Game::GetInstance().GetCurrentScene()->AddGameObject(new WeightNumber(GetTransform()->GetPosition(), sign * 10));

				mPrevWeight += sign * 10.0f;
			}
			else if (abs(mPrevWeight - mBodyWeight) >= 5.0f)
			{
				Game::GetInstance().GetCurrentScene()->AddGameObject(new WeightNumber(GetTransform()->GetPosition(), sign * 5));

				mPrevWeight += sign * 5.0f;
			}
			else if (abs(mPrevWeight - mBodyWeight) >= 3.0f)
			{
				Game::GetInstance().GetCurrentScene()->AddGameObject(new WeightNumber(GetTransform()->GetPosition(), sign * 3));

				mPrevWeight += sign * 3.0f;
			}
			else
			{
				Game::GetInstance().GetCurrentScene()->AddGameObject(new WeightNumber(GetTransform()->GetPosition(), sign * 1));

				mPrevWeight += sign * 1.0f;
			}
		}
	}

	// �� �� ���� Ư���� ���μ������� �����մϴ�.
	IntroSceneProcess();

	StageProcess();

	// �̵� ���� ���͸� �ֽ�ȭ�մϴ�. �� ���͸� ���ؼ� ĳ���Ͱ� � �����ӿ��� �浹�ϴ��� �ľ��մϴ�.
	// �̵��� ������ mDirVector == Vector2D::Zero / �����̶�� ������ ������ �����ϴ�.
	GetTransform()->GetPosition() == mBeforePosition ? mDirVector = Vector2D::Zero
		: mDirVector = (GetTransform()->GetPosition() - mBeforePosition).Normalize();

	// 1. �̹� �����ӿ����� �浹�� Ȯ����
	// �÷����� ���� ������ �����Ѵ�. �� ������Ʈ ���� ������ �ٸ� ..!
	std::vector<GameObject*> collisionInfo = mCollision->GetCollisionInfo();

	for (GameObject* object : collisionInfo)
	{
		// Block
		if (object->GetTag() == "BlockBase")
		{
			BlockBase* block = dynamic_cast<BlockBase*>(object);

			BlockCollisionProcess(block);
		}
		// �Ӱ����� ������ ������ ���
		else if (object->GetTag() == "BreakableBlock")
		{
			BreakableBlock* breakableBlock = dynamic_cast<BreakableBlock*>(object);

			BreakableBlockCollisionProcess(breakableBlock);
		}
		// Platform
		else if (object->GetTag() == "Platform")
		{
			Platform* platform = dynamic_cast<Platform*>(object);

			PlatformCollisionProcess(platform);
		}
		// BlockPlatform
		else if (object->GetTag() == "BlockPlatform")
		{
			BlockPlatform* platform = dynamic_cast<BlockPlatform*>(object);

			BlockPlatformCollisionProcess(platform);
		}

		// Box
		else if (object->GetTag() == "BoxBase")
		{
			BoxBase* box = dynamic_cast<BoxBase*>(object);

			BoxCollisionProcess(box);
		}
		// TransBox�� �浹�� ���
		else if (object->GetTag() == "TransBox")
		{
			TransBox* transBox = dynamic_cast<TransBox*>(object);

			TransBoxCollisionProcess(transBox);
		}
		// ItemBox
		else if (object->GetTag() == "ItemBox")
		{
			ItemBox* itemBox = dynamic_cast<ItemBox*>(object);

			ItemBoxCollisionProcess(itemBox);
		}

		// Enemy
		else if (object->GetTag() == "Enemy")
		{
			EnemyBase* enemy = dynamic_cast<EnemyBase*>(object);

			EnemyCollsionProcess(enemy);
		}
	}

	// ������ üũ ��, ��� ����� ���� �� �����Ƿ� ���� üũ�մϴ�.
	StepChangeProcess();

	StateChangeProcess();
}

void Player::BlockCollisionProcess(BlockBase* block)
{
	if (block != nullptr)
	{
		Transform* blockBaseTransform = block->GetTransform();
		// ���� ��ġ
		Vector2D blockBasePos = blockBaseTransform->GetPosition();
		// ���� ������
		D2D1_SIZE_F blockBaseSize = block->GetSize();
		// Player�� ������
		D2D1_SIZE_F playerSize = dynamic_cast<AABBCollision*>(this->mCollision)->GetSize();

		// -- ��� ���⿡�� �÷��̾�� �浹�ߴ°� + ��� ������ �浹���� �˻��Ѵ�.
		float vertTovertTangent = ((blockBaseSize.height / 2) + (playerSize.height * PLAYER_SCALE / 2)) / ((blockBaseSize.width / 2) + (playerSize.width * PLAYER_SCALE / 2));

		Vector2D centerTocenter = this->mTransform->GetPosition() - blockBasePos;

		float curTangent = centerTocenter.mY / centerTocenter.mX;

		Vector2D velo = this->GetVelocity();

		// �÷��̾�� ���� ���鿡 �ִ�.
		// ���⼭ �� ����ؾ��� ���� �ִٸ�, �÷��̾ ��� �߿��� ��ġ ���� X ..!
		// ����, ���� �鿡�� ���� �� .. ���߰��ϰ� ���� �� ������ �� �� �ִ�.
		if ((centerTocenter.mY < -(TILE_HEIGHT / 2) - (PLAYER_HEIGHT * PLAYER_SCALE / 2) + ADJUST_Y) && ((curTangent >= vertTovertTangent) || (curTangent <= -vertTovertTangent)))
		{
			// �߶� �� �Ǵ� �÷��� ���� �ִ�.
			if (mDirVector.mY >= 0.f)
			{
				// �÷��̾ ��� �ִ� �ظ��� �ϳ��� �ִٴ� ���̹Ƿ� ���� �����ϰ� �ٲ��ݴϴ�.
				mIsJumpable = true;

				velo.mY = 0.f;

				// �÷��̾� ĳ������ ��ġ�� ���� ������� �������ش�.
				mTransform->SetPosition(Vector2D(mTransform->GetPosition().mX,
					blockBasePos.mY + 0.005 - (blockBaseSize.height / 2) - (playerSize.height * PLAYER_SCALE / 2)));

				block->SetCollisionDir(CollisionDir::UP);
			}
		}
		// �÷��̾�� ���� �ظ鿡 �ִ�.
		else if ((centerTocenter.mY >= 0) && ((curTangent >= vertTovertTangent) || (curTangent <= -vertTovertTangent)))
		{
			// ��� �� �÷����� �ظ鿡�� �浹�Ͽ��ٸ�.
			if (mDirVector.mY < 0.f)
			{
				velo.mY = 0.f;

				// �÷��̾� ĳ������ ��ġ�� ���� �ϴ����� �������ش�.
				mTransform->SetPosition(Vector2D(mTransform->GetPosition().mX,
					blockBasePos.mY + (blockBaseSize.height / 2) + (playerSize.height * PLAYER_SCALE / 2)));

				block->SetCollisionDir(CollisionDir::DOWN);
			}
		}
		// �÷��̾�� ���� ������ �鿡 �ִ�.
		else if ((centerTocenter.mX >= 0) && ((curTangent < vertTovertTangent) || (curTangent > -vertTovertTangent)))
		{
			// velo.mX = 0.f;

			// �÷��̾� ĳ������ ��ġ�� ���� �������� �������ش�.
			mTransform->SetPosition(Vector2D(blockBasePos.mX + (blockBaseSize.width / 2) + (playerSize.width * PLAYER_SCALE / 2),
				mTransform->GetPosition().mY));

			block->SetCollisionDir(CollisionDir::RIGHT);
		}
		// �÷��̾�� ���� ���� �鿡 �ִ�.
		else if ((centerTocenter.mX < 0) && ((curTangent < vertTovertTangent) || (curTangent > -vertTovertTangent)))
		{
			// velo.mX = 0.f;

			// �÷��̾� ĳ������ ��ġ�� ���� �������� �������ش�.
			mTransform->SetPosition(Vector2D(blockBasePos.mX - (blockBaseSize.width / 2) - (playerSize.width * PLAYER_SCALE / 2),
				mTransform->GetPosition().mY));

			block->SetCollisionDir(CollisionDir::LEFT);
		}

		// ��ȯ�� �ӵ��� �������ش�.
		this->SetVelocity(velo);
	}
}

void Player::BreakableBlockCollisionProcess(BreakableBlock* breakableBlock)
{
	if (breakableBlock != nullptr)
	{
		Transform* blockBaseTransform = breakableBlock->GetTransform();
		// ���� ��ġ
		Vector2D blockBasePos = blockBaseTransform->GetPosition();
		// ���� ������
		D2D1_SIZE_F blockBaseSize = breakableBlock->GetSize();
		// Player�� ������
		D2D1_SIZE_F playerSize = dynamic_cast<AABBCollision*>(this->mCollision)->GetSize();

		// -- ��� ���⿡�� �÷��̾�� �浹�ߴ°� + ��� ������ �浹���� �˻��Ѵ�.
		float vertTovertTangent = ((blockBaseSize.height / 2) + (playerSize.height * PLAYER_SCALE / 2)) / ((blockBaseSize.width / 2) + (playerSize.width * PLAYER_SCALE / 2));

		Vector2D centerTocenter = this->mTransform->GetPosition() - blockBasePos;

		float curTangent = centerTocenter.mY / centerTocenter.mX;

		Vector2D velo = this->GetVelocity();

		// �÷��̾�� ���� ���鿡 �ִ�.
		// ���⼭ �� ����ؾ��� ���� �ִٸ�, �÷��̾ ��� �߿��� ��ġ ���� X ..!
		// ����, ���� �鿡�� ���� �� .. ���߰��ϰ� ���� �� ������ �� �� �ִ�.
		if ((centerTocenter.mY < -(TILE_HEIGHT / 2) - (PLAYER_HEIGHT * PLAYER_SCALE / 2) + ADJUST_Y) && ((curTangent >= vertTovertTangent) || (curTangent <= -vertTovertTangent)))
		{
			// �߶� �� �Ǵ� �÷��� ���� �ִ�.
			if (mDirVector.mY >= 0.f)
			{
				// �÷��̾ ��� �ִ� �ظ��� �ϳ��� �ִٴ� ���̹Ƿ� ���� �����ϰ� �ٲ��ݴϴ�.
				mIsJumpable = true;

				velo.mY = 0.f;

				// �÷��̾� ĳ������ ��ġ�� ���� ������� �������ش�.
				mTransform->SetPosition(Vector2D(mTransform->GetPosition().mX,
					blockBasePos.mY + 0.005 - (blockBaseSize.height / 2) - (playerSize.height * PLAYER_SCALE / 2)));

				breakableBlock->SetCollisionDir(CollisionDir::UP);
			}
		}
		// �÷��̾�� ���� �ظ鿡 �ִ�.
		else if ((centerTocenter.mY >= 0) && ((curTangent >= vertTovertTangent) || (curTangent <= -vertTovertTangent)))
		{
			// ��� �� �÷����� �ظ鿡�� �浹�Ͽ��ٸ�.
			if (mDirVector.mY < 0.f)
			{
				velo.mY = 0.f;

				// �÷��̾� ĳ������ ��ġ�� ���� �ϴ����� �������ش�.
				mTransform->SetPosition(Vector2D(mTransform->GetPosition().mX,
					blockBasePos.mY + (blockBaseSize.height / 2) + (playerSize.height * PLAYER_SCALE / 2)));

				breakableBlock->BreakBlock();

				breakableBlock->SetCollisionDir(CollisionDir::DOWN);
			}
		}
		// �÷��̾�� ���� ������ �鿡 �ִ�.
		else if ((centerTocenter.mX >= 0) && ((curTangent < vertTovertTangent) || (curTangent > -vertTovertTangent)))
		{
			// velo.mX = 0.f;

			// �÷��̾� ĳ������ ��ġ�� ���� �������� �������ش�.
			mTransform->SetPosition(Vector2D(blockBasePos.mX + (blockBaseSize.width / 2) + (playerSize.width * PLAYER_SCALE / 2),
				mTransform->GetPosition().mY));

			breakableBlock->SetCollisionDir(CollisionDir::RIGHT);
		}
		// �÷��̾�� ���� ���� �鿡 �ִ�.
		else if ((centerTocenter.mX < 0) && ((curTangent < vertTovertTangent) || (curTangent > -vertTovertTangent)))
		{
			// velo.mX = 0.f;

			// �÷��̾� ĳ������ ��ġ�� ���� �������� �������ش�.
			mTransform->SetPosition(Vector2D(blockBasePos.mX - (blockBaseSize.width / 2) - (playerSize.width * PLAYER_SCALE / 2),
				mTransform->GetPosition().mY));

			breakableBlock->SetCollisionDir(CollisionDir::LEFT);
		}

		// ��ȯ�� �ӵ��� �������ش�.
		this->SetVelocity(velo);
	}
}

void Player::PlatformCollisionProcess(Platform* platform)
{
	if (platform != nullptr)
	{
		Transform* blockBaseTransform = platform->GetTransform();
		// ���� ��ġ
		Vector2D blockBasePos = blockBaseTransform->GetPosition();
		// ���� ������
		D2D1_SIZE_F blockBaseSize = platform->GetSize();
		// Player�� ������
		D2D1_SIZE_F playerSize = dynamic_cast<AABBCollision*>(this->mCollision)->GetSize();

		// -- ��� ���⿡�� �÷��̾�� �浹�ߴ°� + ��� ������ �浹���� �˻��Ѵ�.
		float vertTovertTangent = ((blockBaseSize.height / 2) + (playerSize.height * PLAYER_SCALE / 2)) / ((blockBaseSize.width / 2) + (playerSize.width * PLAYER_SCALE / 2));

		Vector2D centerTocenter = this->mTransform->GetPosition() - blockBasePos;

		float curTangent = centerTocenter.mY / centerTocenter.mX;

		Vector2D velo = this->GetVelocity();

		// �÷��̾�� �÷����� ���鿡 �ִ�.
		// ���⼭ �� ����ؾ��� ���� �ִٸ�, �÷��̾ ��� �߿��� ��ġ ���� X ..!
		// ���⼭ �� ó�� ������ �ǹ��ϴ� �ٴ� �÷����� ĳ���Ͱ� �ǹ��ִ� �Ÿ� �̻� ������ �־�߸� �÷��� ������ �ǽ��ϵ��� ������ ���Դϴ�.
		if ((centerTocenter.mY < -(TILE_HEIGHT / 2) - (PLAYER_HEIGHT * PLAYER_SCALE / 2) + ADJUST_Y) && ((curTangent >= vertTovertTangent) || (curTangent <= -vertTovertTangent)))
		{
			// �ش� �������� Jump�� ����� �������̸� �Ѿ��.
			if (((mPrevState == PlayerState::Run) && (mCurrentState == PlayerState::Jump))
				|| ((mPrevState == PlayerState::Idle) && (mCurrentState == PlayerState::Jump)))
				return;

			if ((mDirVector.mY >= 0.f))
			{
				// �÷��̾ ��� �ִ� �ظ��� �ϳ��� �ִٴ� ���̹Ƿ� ���� �����ϰ� �ٲ��ݴϴ�.
				mIsJumpable = true;

				velo.mY = 0.f;

				// �÷��̾� ĳ������ ��ġ�� �÷����� ������� �������ش�. ������ ����� �񱳰����� �� �۾����Ƿ� ������ �����˴ϴ�.
				mTransform->SetPosition(Vector2D(mTransform->GetPosition().mX,
					blockBasePos.mY + 0.005f - (blockBaseSize.height / 2) - (playerSize.height * PLAYER_SCALE / 2)));

				platform->SetCollisionDir(CollisionDir::UP);
			}
		}
		// �÷��̾�� ���� �ظ鿡 �ִ�.
		else if ((centerTocenter.mY > 0) && ((curTangent >= vertTovertTangent) || (curTangent <= -vertTovertTangent)))
		{
			platform->SetCollisionDir(CollisionDir::DOWN);
		}
		// �÷��̾�� �÷����� ������ �鿡 �ִ�.
		else if ((centerTocenter.mX > 0) && ((curTangent < vertTovertTangent) || (curTangent > -vertTovertTangent)))
		{
			platform->SetCollisionDir(CollisionDir::RIGHT);
		}
		// �÷��̾�� �÷����� ���� �鿡 �ִ�.
		else if ((centerTocenter.mX < 0) && ((curTangent < vertTovertTangent) || (curTangent > -vertTovertTangent)))
		{
			platform->SetCollisionDir(CollisionDir::LEFT);
		}

		// ��ȯ�� �ӵ��� �������ش�.
		this->SetVelocity(velo);
	}
}
void Player::BlockPlatformCollisionProcess(BlockPlatform* blockplatform)
{
	if (blockplatform != nullptr)
	{
		Transform* blockBaseTransform = blockplatform->GetTransform();
		// ���� ��ġ
		Vector2D blockBasePos = blockBaseTransform->GetPosition();
		// ���� ������
		D2D1_SIZE_F blockBaseSize = blockplatform->GetSize();
		// Player�� ������
		D2D1_SIZE_F playerSize = dynamic_cast<AABBCollision*>(this->mCollision)->GetSize();

		// -- ��� ���⿡�� �÷��̾�� �浹�ߴ°� + ��� ������ �浹���� �˻��Ѵ�.
		float vertTovertTangent = ((blockBaseSize.height / 2) + (playerSize.height * PLAYER_SCALE / 2)) / ((blockBaseSize.width / 2) + (playerSize.width * PLAYER_SCALE / 2));

		Vector2D centerTocenter = this->mTransform->GetPosition() - blockBasePos;

		float curTangent = centerTocenter.mY / centerTocenter.mX;

		Vector2D velo = this->GetVelocity();

		// �÷��̾�� ���� ���鿡 �ִ�.
		// ���⼭ �� ����ؾ��� ���� �ִٸ�, �÷��̾ ��� �߿��� ��ġ ���� X ..!
		// ����, ���� �鿡�� ���� �� .. ���߰��ϰ� ���� �� ������ �� �� �ִ�.
		if ((centerTocenter.mY < -(TILE_HEIGHT / 2) - (PLAYER_HEIGHT * PLAYER_SCALE / 2) + ADJUST_Y) && ((curTangent >= vertTovertTangent) || (curTangent <= -vertTovertTangent)))
		{
			// �߶� �� �Ǵ� �÷��� ���� �ִ�.
			if (mDirVector.mY >= 0.f)
			{
				// �÷��̾ ��� �ִ� �ظ��� �ϳ��� �ִٴ� ���̹Ƿ� ���� �����ϰ� �ٲ��ݴϴ�.
				mIsJumpable = true;

				velo.mY = 0.f;

				// �÷��̾� ĳ������ ��ġ�� ���� ������� �������ش�.
				mTransform->SetPosition(Vector2D(mTransform->GetPosition().mX,
					blockBasePos.mY + 0.005 - (blockBaseSize.height / 2) - (playerSize.height * PLAYER_SCALE / 2)));

				blockplatform->SetCollisionDir(CollisionDir::UP);
			}
		}
		// �÷��̾�� ���� �ظ鿡 �ִ�.
		else if ((centerTocenter.mY >= 0) && ((curTangent >= vertTovertTangent) || (curTangent <= -vertTovertTangent)))
		{
			//// ��� �� �÷����� �ظ鿡�� �浹�Ͽ��ٸ�.
			//if (mDirVector.mY < 0.f)
			//{
			//	velo.mY = 0.f;

			//	// �÷��̾� ĳ������ ��ġ�� ���� �ϴ����� �������ش�.
			//	mTransform->SetPosition(Vector2D(mTransform->GetPosition().mX,
			//		blockBasePos.mY + (blockBaseSize.height / 2) + (playerSize.height * PLAYER_SCALE / 2)));
			//}
			blockplatform->SetCollisionDir(CollisionDir::DOWN);

		}
		// �÷��̾�� ���� ������ �鿡 �ִ�.
		else if ((centerTocenter.mX >= 0) && ((curTangent < vertTovertTangent) || (curTangent > -vertTovertTangent)))
		{
			// velo.mX = 0.f;

			// �÷��̾� ĳ������ ��ġ�� ���� �������� �������ش�.
			mTransform->SetPosition(Vector2D(blockBasePos.mX + (blockBaseSize.width / 2) + (playerSize.width * PLAYER_SCALE / 2),
				mTransform->GetPosition().mY));

			blockplatform->SetCollisionDir(CollisionDir::RIGHT);
		}
		// �÷��̾�� ���� ���� �鿡 �ִ�.
		else if ((centerTocenter.mX < 0) && ((curTangent < vertTovertTangent) || (curTangent > -vertTovertTangent)))
		{
			// velo.mX = 0.f;

			// �÷��̾� ĳ������ ��ġ�� ���� �������� �������ش�.
			mTransform->SetPosition(Vector2D(blockBasePos.mX - (blockBaseSize.width / 2) - (playerSize.width * PLAYER_SCALE / 2),
				mTransform->GetPosition().mY));

			blockplatform->SetCollisionDir(CollisionDir::LEFT);
		}

		// ��ȯ�� �ӵ��� �������ش�.
		this->SetVelocity(velo);
	}
}

void Player::BoxCollisionProcess(BoxBase* box)
{
	if (box != nullptr)
	{
		Transform* blockBaseTransform = box->GetTransform();
		// ���� ��ġ
		Vector2D blockBasePos = blockBaseTransform->GetPosition();
		// ���� ������
		D2D1_SIZE_F blockBaseSize = box->GetSize();
		// Player�� ������
		D2D1_SIZE_F playerSize = dynamic_cast<AABBCollision*>(this->mCollision)->GetSize();

		// -- ��� ���⿡�� �÷��̾�� �浹�ߴ°� + ��� ������ �浹���� �˻��Ѵ�.
		float vertTovertTangent = ((blockBaseSize.height / 2) + (playerSize.height * PLAYER_SCALE / 2)) / ((blockBaseSize.width / 2) + (playerSize.width * PLAYER_SCALE / 2));

		Vector2D centerTocenter = this->mTransform->GetPosition() - blockBasePos;

		float curTangent = centerTocenter.mY / centerTocenter.mX;

		Vector2D velo = this->GetVelocity();

		// �÷��̾�� ���� ���鿡 �ִ�.
		// ���⼭ �� ����ؾ��� ���� �ִٸ�, �÷��̾ ��� �߿��� ��ġ ���� X ..!
		// ����, ���� �鿡�� ���� �� .. ���߰��ϰ� ���� �� ������ �� �� �ִ�.
		if ((centerTocenter.mY < -(TILE_HEIGHT / 2) - (PLAYER_HEIGHT * PLAYER_SCALE / 2) + ADJUST_Y) && ((curTangent >= vertTovertTangent) || (curTangent <= -vertTovertTangent)))
		{
			// �߶� �� �Ǵ� �÷��� ���� �ִ�.
			if (mDirVector.mY >= 0.f)
			{
				// �÷��̾ ��� �ִ� �ظ��� �ϳ��� �ִٴ� ���̹Ƿ� ���� �����ϰ� �ٲ��ݴϴ�.
				mIsJumpable = true;

				velo.mY = 0.f;

				// �÷��̾� ĳ������ ��ġ�� ���� ������� �������ش�.
				mTransform->SetPosition(Vector2D(mTransform->GetPosition().mX,
					blockBasePos.mY + 0.005 - (blockBaseSize.height / 2) - (playerSize.height * PLAYER_SCALE / 2)));

				box->SetCollisionDir(CollisionDir::UP);
			}
		}
		// �÷��̾�� ���� �ظ鿡 �ִ�.
		else if ((centerTocenter.mY >= 0) && ((curTangent >= vertTovertTangent) || (curTangent <= -vertTovertTangent)))
		{
			// ��� �� �÷����� �ظ鿡�� �浹�Ͽ��ٸ�.
			if (mDirVector.mY < 0.f)
			{
				velo.mY = BLOCKING_POWER;

				// �÷��̾� ĳ������ ��ġ�� ���� �ϴ����� �������ش�.
				mTransform->SetPosition(Vector2D(mTransform->GetPosition().mX,
					blockBasePos.mY + (blockBaseSize.height / 2) + (playerSize.height * PLAYER_SCALE / 2)));

				box->SetCollisionDir(CollisionDir::DOWN);
			}
		}
		// �÷��̾�� ���� ������ �鿡 �ִ�.
		else if ((centerTocenter.mX >= 0) && ((curTangent < vertTovertTangent) || (curTangent > -vertTovertTangent)))
		{
			// velo.mX = 0.f;

			// �÷��̾� ĳ������ ��ġ�� ���� �������� �������ش�.
			mTransform->SetPosition(Vector2D(blockBasePos.mX + (blockBaseSize.width / 2) + (playerSize.width * PLAYER_SCALE / 2),
				mTransform->GetPosition().mY));

			box->SetCollisionDir(CollisionDir::RIGHT);
		}
		// �÷��̾�� ���� ���� �鿡 �ִ�.
		else if ((centerTocenter.mX < 0) && ((curTangent < vertTovertTangent) || (curTangent > -vertTovertTangent)))
		{
			// velo.mX = 0.f;

			// �÷��̾� ĳ������ ��ġ�� ���� �������� �������ش�.
			mTransform->SetPosition(Vector2D(blockBasePos.mX - (blockBaseSize.width / 2) - (playerSize.width * PLAYER_SCALE / 2),
				mTransform->GetPosition().mY));

			box->SetCollisionDir(CollisionDir::LEFT);
		}

		// ��ȯ�� �ӵ��� �������ش�.
		this->SetVelocity(velo);
	}
}

void Player::TransBoxCollisionProcess(TransBox* transBox)
{
	if (transBox != nullptr)
	{
		Transform* blockBaseTransform = transBox->GetTransform();
		// ���� ��ġ
		Vector2D blockBasePos = blockBaseTransform->GetPosition();
		// ���� ������
		D2D1_SIZE_F blockBaseSize = transBox->GetSize();
		// Player�� ������
		D2D1_SIZE_F playerSize = dynamic_cast<AABBCollision*>(this->mCollision)->GetSize();

		// -- ��� ���⿡�� �÷��̾�� �浹�ߴ°� + ��� ������ �浹���� �˻��Ѵ�.
		float vertTovertTangent = ((blockBaseSize.height / 2) + (playerSize.height * PLAYER_SCALE / 2)) / ((blockBaseSize.width / 2) + (playerSize.width * PLAYER_SCALE / 2));

		Vector2D centerTocenter = this->mTransform->GetPosition() - blockBasePos;

		float curTangent = centerTocenter.mY / centerTocenter.mX;

		Vector2D velo = this->GetVelocity();

		// �÷��̾�� ���� ���鿡 �ִ�.
		// ���⼭ �� ����ؾ��� ���� �ִٸ�, �÷��̾ ��� �߿��� ��ġ ���� X ..!
		if ((centerTocenter.mY < 0) && ((curTangent >= vertTovertTangent) || (curTangent <= -vertTovertTangent)))
		{
			// �ش� �������� Jump�� ����� �������̸� �Ѿ��.
			if (((mPrevState == PlayerState::Run) && (mCurrentState == PlayerState::Jump))
				|| ((mPrevState == PlayerState::Idle) && (mCurrentState == PlayerState::Jump)))
				return;

			transBox->SetCollisionDir(CollisionDir::UP);
		}
		// �÷��̾�� ���� �ظ鿡 �ִ�.
		else if ((centerTocenter.mY > 0) && ((curTangent >= vertTovertTangent) || (curTangent <= -vertTovertTangent)))
		{
			// ��� �� �÷����� �ظ鿡�� �浹�Ͽ��ٸ�.
			if (mDirVector.mY < 0.f)
			{
				transBox->TriggerOn();	// Ʈ���Ÿ� ���ش�

				velo.mY = BLOCKING_POWER;

				// �÷��̾� ĳ������ ��ġ�� ���� �ϴ����� �������ش�.
				mTransform->SetPosition(Vector2D(mTransform->GetPosition().mX,
					blockBasePos.mY + (blockBaseSize.height / 2) + (playerSize.height * PLAYER_SCALE / 2)));

				transBox->SetCollisionDir(CollisionDir::DOWN);
			}
		}
		// �÷��̾�� ���� ������ �鿡 �ִ�.
		else if ((centerTocenter.mX > 0) && ((curTangent < vertTovertTangent) || (curTangent > -vertTovertTangent)))
		{
			transBox->SetCollisionDir(CollisionDir::RIGHT);
		}
		// �÷��̾�� ���� ���� �鿡 �ִ�.
		else if ((centerTocenter.mX < 0) && ((curTangent < vertTovertTangent) || (curTangent > -vertTovertTangent)))
		{
			transBox->SetCollisionDir(CollisionDir::LEFT);
		}

		// ��ȯ�� �ӵ��� �������ش�.
		this->SetVelocity(velo);
	}
}

void Player::ItemBoxCollisionProcess(ItemBox* itemBox)
{
	if (itemBox != nullptr)
	{
		Transform* blockBaseTransform = itemBox->GetTransform();
		// ���� ��ġ
		Vector2D blockBasePos = blockBaseTransform->GetPosition();
		// ���� ������
		D2D1_SIZE_F blockBaseSize = itemBox->GetSize();
		// Player�� ������
		D2D1_SIZE_F playerSize = dynamic_cast<AABBCollision*>(this->mCollision)->GetSize();

		// -- ��� ���⿡�� �÷��̾�� �浹�ߴ°� + ��� ������ �浹���� �˻��Ѵ�.
		float vertTovertTangent = ((blockBaseSize.height / 2) + (playerSize.height * PLAYER_SCALE / 2)) / ((blockBaseSize.width / 2) + (playerSize.width * PLAYER_SCALE / 2));

		Vector2D centerTocenter = this->mTransform->GetPosition() - blockBasePos;

		float curTangent = centerTocenter.mY / centerTocenter.mX;

		Vector2D velo = this->GetVelocity();

		// �÷��̾�� ���� ���鿡 �ִ�.
		// ���⼭ �� ����ؾ��� ���� �ִٸ�, �÷��̾ ��� �߿��� ��ġ ���� X ..!
		// ����, ���� �鿡�� ���� �� .. ���߰��ϰ� ���� �� ������ �� �� �ִ�.
		if ((centerTocenter.mY < -(TILE_HEIGHT / 2) - (PLAYER_HEIGHT * PLAYER_SCALE / 2) + ADJUST_Y) && ((curTangent >= vertTovertTangent) || (curTangent <= -vertTovertTangent)))
		{
			// �߶� �� �Ǵ� �÷��� ���� �ִ�.
			if (mDirVector.mY >= 0.f)
			{
				// �÷��̾ ��� �ִ� �ظ��� �ϳ��� �ִٴ� ���̹Ƿ� ���� �����ϰ� �ٲ��ݴϴ�.
				mIsJumpable = true;

				velo.mY = 0.f;

				// �÷��̾� ĳ������ ��ġ�� ���� ������� �������ش�.
				mTransform->SetPosition(Vector2D(mTransform->GetPosition().mX,
					blockBasePos.mY + 0.005 - (blockBaseSize.height / 2) - (playerSize.height * PLAYER_SCALE / 2)));

				itemBox->SetCollisionDir(CollisionDir::UP);
			}
		}
		// �÷��̾�� ���� �ظ鿡 �ִ�.
		else if ((centerTocenter.mY >= 0) && ((curTangent >= vertTovertTangent) || (curTangent <= -vertTovertTangent)))
		{
			// ��� �� �÷����� �ظ鿡�� �浹�Ͽ��ٸ�.
			if (mDirVector.mY < 0.f)
			{
				velo.mY = BLOCKING_POWER;

				itemBox->TriggerOn();	// Ʈ���Ÿ� ���ش�

				// �÷��̾� ĳ������ ��ġ�� ���� �ϴ����� �������ش�.
				mTransform->SetPosition(Vector2D(mTransform->GetPosition().mX,
					blockBasePos.mY + (blockBaseSize.height / 2) + (playerSize.height * PLAYER_SCALE / 2)));

				itemBox->SetCollisionDir(CollisionDir::DOWN);
			}
		}
		// �÷��̾�� ���� ������ �鿡 �ִ�.
		else if ((centerTocenter.mX >= 0) && ((curTangent < vertTovertTangent) || (curTangent > -vertTovertTangent)))
		{
			// velo.mX = 0.f;

			// �÷��̾� ĳ������ ��ġ�� ���� �������� �������ش�.
			mTransform->SetPosition(Vector2D(blockBasePos.mX + (blockBaseSize.width / 2) + (playerSize.width * PLAYER_SCALE / 2),
				mTransform->GetPosition().mY));

			itemBox->SetCollisionDir(CollisionDir::RIGHT);
		}
		// �÷��̾�� ���� ���� �鿡 �ִ�.
		else if ((centerTocenter.mX < 0) && ((curTangent < vertTovertTangent) || (curTangent > -vertTovertTangent)))
		{
			// velo.mX = 0.f;

			// �÷��̾� ĳ������ ��ġ�� ���� �������� �������ش�.
			mTransform->SetPosition(Vector2D(blockBasePos.mX - (blockBaseSize.width / 2) - (playerSize.width * PLAYER_SCALE / 2),
				mTransform->GetPosition().mY));

			itemBox->SetCollisionDir(CollisionDir::LEFT);
		}

		// ��ȯ�� �ӵ��� �������ش�.
		this->SetVelocity(velo);
	}
}

void Player::EnemyCollsionProcess(EnemyBase* enemy)
{
	if (enemy != nullptr)
	{
		if (enemy->GetCurrentState() == EnemyState::Die)
			return;

		FreeEnemy* freeEnemy = dynamic_cast<FreeEnemy*>(enemy);

		if ((freeEnemy != nullptr) && ((freeEnemy->GetType() == "Down") || (freeEnemy->GetType() == "NiddleDown")))
			return;

		Transform* blockBaseTransform = enemy->GetTransform();

		// ���� ��ġ
		Vector2D blockBasePos = blockBaseTransform->GetPosition();

		// ���� ������
		D2D1_SIZE_F blockBaseSize = dynamic_cast<AABBCollision*>(enemy->GetCollision())->GetSize();
		// Player�� ������
		D2D1_SIZE_F playerSize = dynamic_cast<AABBCollision*>(this->mCollision)->GetSize();

		// -- ��� ���⿡�� �÷��̾�� �浹�ߴ°� + ��� ������ �浹���� �˻��Ѵ�.
		float vertTovertTangent = ((blockBaseSize.height / 2) + (playerSize.height * PLAYER_SCALE / 2)) / ((blockBaseSize.width / 2) + (playerSize.width * PLAYER_SCALE / 2));

		Vector2D centerTocenter = this->mTransform->GetPosition() - blockBasePos;

		float curTangent = centerTocenter.mY / centerTocenter.mX;

		Vector2D velo = this->GetVelocity();

		// �÷��̾�� �÷����� ���鿡 �ִ�.
		// ���⼭ �� ����ؾ��� ���� �ִٸ�, �÷��̾ ��� �߿��� ��ġ ���� X ..!
		// ���⼭ �� ó�� ������ �ǹ��ϴ� �ٴ� �÷����� ĳ���Ͱ� �ǹ��ִ� �Ÿ� �̻� ������ �־�߸� �÷��� ������ �ǽ��ϵ��� ������ ���Դϴ�.
		if ((centerTocenter.mY < -(blockBaseSize.height / 2) - (PLAYER_HEIGHT * PLAYER_SCALE / 2) + ADJUST_Y) && ((curTangent >= vertTovertTangent) || (curTangent <= -vertTovertTangent)))
		{
			if ((mDirVector.mY >= 0.f))
			{
				// �� ������ �������� ��¦ Up ?
				velo.mY = -1000.f;

				// �÷��̾� ĳ������ ��ġ�� �÷����� ������� �������ش�. ������ ����� �񱳰����� �� �۾����Ƿ� ������ �����˴ϴ�.
				mTransform->SetPosition(Vector2D(mTransform->GetPosition().mX,
					blockBasePos.mY + 0.005f - (blockBaseSize.height / 2) - (playerSize.height * PLAYER_SCALE / 2)));

				enemy->SetHp(0);
			}
		}
		else
			SetHp(0);

		// ��ȯ�� �ӵ��� �������ش�.
		this->SetVelocity(velo);
	}
}

// ��Ʈ�� �������� �̵��� ����ؾ���. ���� �� ���μ����� ���´�.
void Player::IntroSceneProcess()
{
	if (Game::GetInstance().GetCurrentScene()->GetSceneName() != "Intro")
		return;

	Vector2D playerPos = mTransform->GetPosition();

	D2D1_SIZE_F size = Graphics::GetInstance().GetScreenSize();

	if (playerPos.mX < 0.0f)
		mTransform->SetPosition(Vector2D(0.0f, playerPos.mY));
	else if (playerPos.mX > 1920.0f / CAMERA_ZOOM)
		mTransform->SetPosition(Vector2D(1920.0f / CAMERA_ZOOM, playerPos.mY));
}

void Player::StageProcess()
{
	// 1. �� ���������� ���� ����
	if (mTransform->GetPosition().mY >= 3000.f)
		SetHp(0);
}

void Player::TimeProcess()
{
	std::string sceneName = Game::GetInstance().GetCurrentScene()->GetSceneName();

	if ((sceneName == "Intro") || (sceneName == "StageThreeOne") || (sceneName == "StageThreeTwo") || (sceneName == "StageThreeThree") ||
		(sceneName == "StageThreeFour") || (sceneName == "StageThreeFive"))
		return;

	if (mCurrentState == PlayerState::Trans)
		return;

	float delta = Timer::GetInstance().GetDeltaTime() / 1000.f;

	mStagePlayTime += delta;

	mAcculmulTime += delta;

	if (mAcculmulTime >= 10.f)
	{
		PlusBodyWeight(GAIN_PER_SEC);

		mAcculmulTime = 0.f;
	}
}