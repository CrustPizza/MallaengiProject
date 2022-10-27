#include "PreCompiledHeader.h"
#include "BreakableBlock.h"


BlockFragment::BlockFragment(Vector2D pos)
	: mDuration(5000.0f)
	, mGravity(30.0f)
	, mEffectSpeed(500.0f)
{
	mTransform->SetPosition(pos);
}

BlockFragment::~BlockFragment()
{
}

void BlockFragment::Update()
{
	// ��Ÿ Ÿ���� �����ͼ� ���� �ð��� ���δ�.
	mDuration -= Timer::GetInstance().GetDeltaTime();

	// ���� ��ü�� �̵������ �߷��� ������Ѽ� �̵���Ų��.
	mTransform->SetPosition(mTransform->GetPosition() + Timer::GetInstance().GetDeltaTime() / 1000 * mEffectSpeed * mDirection);
	mTransform->SetPosition(mTransform->GetPosition() + Timer::GetInstance().GetDeltaTime() / 1000 * mGravity * Vector2D::Down);
	
	// �߷°��ӵ��� �����Ѵ�.
	mGravity *= 1 + 8.8f * Timer::GetInstance().GetDeltaTime() / 1000;

	if (mDuration < 0.0f)
	{
		// ���� �ð��� ������ ����
		Game::GetInstance().GetCurrentScene()->ReserveDestroy(this);
	}
}

void BlockFragment::SetDirection(Vector2D direction)
{
	mDirection = direction;
}

BreakableBlock::BreakableBlock()
{
	mSize = { TILE_WIDTH, TILE_HEIGHT };

	// ����� AABBCollision���� �۵��Ѵ�.
	mAABBCollision = AddAABBCollision();

	AddSpriteRenderer();

	// �±׸� �극��Ŀ�� ������� �����Ѵ�.
	SetTag("BreakableBlock");
}

BreakableBlock::~BreakableBlock()
{

}

void BreakableBlock::FixedUpdate()
{
	BlockBase::FixedUpdate();
}

void BreakableBlock::Update()
{
	BlockBase::Update();
}

/// <summary>
/// ����� �ı��� �� ȣ��Ǵ� �Լ�
/// 
/// 4�������� ���� ������ ����Ʈ ������ �Ѵ�.
/// </summary>
void BreakableBlock::BreakBlock()
{
	BlockFragment* fragment[4];

	// ������ ���� ���� ��ġ�κ��� 4������ �밢�� �������� ������.
	// �׿� ���� �˸´� ��������Ʈ �̹����� ����ش�.
	for (int i = 0; i < 4; i++)
	{
		fragment[i] = new BlockFragment(this->GetTransform()->GetPosition());
		fragment[i]->AddSpriteRenderer()->SetSprite(new Sprite(*this->GetSpriteRenderer()->GetSprite()));
		fragment[i]->GetSpriteRenderer()->GetSprite()->mPosInSheet -= Vector2D(TILE_WIDTH * (2 - i % 2) + 1, 0);
		Game::GetInstance().GetCurrentScene()->AddGameObject(fragment[i]);
	}

	fragment[0]->SetDirection((Vector2D::Right + Vector2D::Up).Normalize());
	fragment[1]->SetDirection((Vector2D::Left + Vector2D::Up).Normalize());
	fragment[2]->SetDirection((Vector2D::Left + Vector2D::Down).Normalize());
	fragment[3]->SetDirection((Vector2D::Right + Vector2D::Down).Normalize());

	// ������ �����ϰ��� ���� ��ü�� �����Ѵ�.
	Game::GetInstance().GetCurrentScene()->ReserveDestroy(this);
	SoundManager::GetInstance()->Play(500030);
	
}