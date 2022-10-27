#include "PreCompiledHeader.h"
#include "ProgressBar.h"

ProgressBar::ProgressBar(Sprite* sprite, bool isFront, void (*func)(float Percent), float percent)
	: mValue(percent)
	, mMax(1.0f)
	, mFront(isFront)
	, mFunc(func)
{
	// ��������Ʈ�� ��������Ѵ�.
	GetSpriteRenderer()->SetSprite(new Sprite(*sprite));

	// ��������Ʈ�� ���� ����� ����
	mSize = GetSpriteRenderer()->GetSprite()->mSize;

	// ����ٴϱ� ���ʿ������� ����ؾ� �����ϱ� ���Ѱ� ����.
	mPivot = Vector2D::Zero;
	GetTransform()->SetPosition(GetTransform()->GetPosition() - Vector2D(mSize.width * CAMERA_ZOOM / 2, 0));
}

ProgressBar::~ProgressBar()
{
	delete GetSpriteRenderer()->GetSprite();
}

void ProgressBar::Update()
{
	ButtonBase::Update();

	// Bar�� Ŭ������ �� ���콺 ��ǥ�� ���� ���� ��ȭ�����ش�.
	if (mIsClicked == true)
	{
		Vector2D pos = GetTransform()->GetPosition();

		Vector2D mousePos = Input::GetInstance().GetCurrentMousePos();

		SetPercent((mousePos.mX - pos.mX) / mSize.width / CAMERA_ZOOM);

		mFunc(mValue / mMax);
	}

	// ���콺�� ���� false
	if (Input::GetInstance().GetKeyState(VK_LBUTTON) == KEYSTATE::UP)
		mIsClicked = false;
}

void ProgressBar::SetPercent(float percent)
{
	// �ۼ�Ʈ�� 0 ~ 1���̷� ������Ų��.
	if (percent < 0.0f)
		mValue = 0.0f;
	else if (percent > 1.0f)
		mValue = mMax;
	else
		mValue = percent * mMax;

	// ����� ���� ��������Ʈ�� ����
	GetSpriteRenderer()->GetSprite()->mSize.width = mSize.width * mValue / mMax;
}

void ProgressBar::SetIncrease(float rate)
{
	mValue += 0.01 * rate;

	if (mValue > mMax)
		mValue = mMax;

	GetSpriteRenderer()->GetSprite()->mSize.width = mSize.width * mValue / mMax;
}

void ProgressBar::SetDecrease(float rate)
{
	mValue -= 0.01 * rate;

	if (mValue < 0.0f)
		mValue = 0.0f;

	GetSpriteRenderer()->GetSprite()->mSize.width = mSize.width * mValue / mMax;
}

void ProgressBar::OnButtonClicked()
{
	if (mIsActivated != true)
		return;

	ButtonBase::OnButtonClicked();

	if (mFront == true)
	{
		mIsClicked = true;
	}
}