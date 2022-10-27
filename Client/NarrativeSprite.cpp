#include "PreCompiledHeader.h"
#include "NarrativeSprite.h"

#include "FadeOutObject.h"

NarrativeSprite::NarrativeSprite(Sprite* sprite, double delay, double duration, int speed)
	: mDelay(delay)
	, mDuration(duration)
	, mAlphaSpeed(speed)
	, mAlpha(0.0f)
	, mNext(false)
{
	AddSpriteRenderer()->SetSprite(sprite);
	GetSpriteRenderer()->SetOpacity(mAlpha);
}

NarrativeSprite::~NarrativeSprite()
{

}

void NarrativeSprite::Update()
{
	// ���� �ð��� �Ǳ� ������ ������� �ʴ´�.
	if (mDelay <= 0.0f)
	{
		// ���� �ð��� ������
		if (mDuration <= 0.0f)
		{
			// ���İ��� �����Ͽ� Fade Out ȿ�� ����
			if (mAlphaSpeed > 0)
			{
				mAlpha -= (Timer::GetInstance().GetDeltaTime() / 2000.0f) * mAlphaSpeed;
				GetSpriteRenderer()->SetOpacity(mAlpha);
			}

			// ���İ��� 0���ϰ� �Ǹ� ������Ʈ�� ����
			if (mAlpha <= 0.0f || mAlphaSpeed <= 0)
			{
				Game::GetInstance().GetCurrentScene()->ReserveDestroy(this);

				// ������ �ƾ��� ��� �������� ����
				if (GetSpriteRenderer()->GetSprite()->mSpriteSheetIndex == 10210)
				{
					FadeOutObject* fadeOut = new FadeOutObject("StageOneLoadingScene");

					Game::GetInstance().GetCurrentScene()->AddGameObject(fadeOut);
				}
				else if (GetSpriteRenderer()->GetSprite()->mSpriteSheetIndex == 10304)
				{
					FadeOutObject* fadeOut = new FadeOutObject("StageTwoLoadingScene");

					Game::GetInstance().GetCurrentScene()->AddGameObject(fadeOut);
				}
				else if (GetSpriteRenderer()->GetSprite()->mSpriteSheetIndex == 10415)
				{
					FadeOutObject* fadeOut = new FadeOutObject("StageThreeOneLoadingScene");

					Game::GetInstance().GetCurrentScene()->AddGameObject(fadeOut);
				}
				else if (GetSpriteRenderer()->GetSprite()->mSpriteSheetIndex == 10508
					|| GetSpriteRenderer()->GetSprite()->mSpriteSheetIndex == 10607)
				{
					FadeOutObject* fadeOut = new FadeOutObject("IntroLoadingScene");

					Game::GetInstance().GetCurrentScene()->AddGameObject(fadeOut);
				}
			}
		}
		// �������̸�
		else
		{
			// ���İ��� �����Ͽ� Fade In ȿ�� ����
			if (mAlphaSpeed > 0)
			{
				mAlpha += (Timer::GetInstance().GetDeltaTime() / 2000.0f) * mAlphaSpeed;

				// 1������
				if (mAlpha > 1.0f)
				{
					mAlpha = 1.0f;

					if (GetSpriteRenderer()->GetSprite()->mSpriteSheetIndex == 10607)
					{
						Vector2D pos = GetTransform()->GetPosition();

						if (pos.mX > Graphics::GetInstance().GetScreenSize().width / 2)
							GetTransform()->SetPosition(pos - Vector2D(100.0f, 0) * Timer::GetInstance().GetDeltaTime() / 1000);
					}
				}

				GetSpriteRenderer()->SetOpacity(mAlpha);
			}
			else
			{
				GetSpriteRenderer()->SetOpacity(1.0f);
			}

			mDuration -= Timer::GetInstance().GetDeltaTime() / 1000;
		}
	}
	else
	{
		mDelay -= Timer::GetInstance().GetDeltaTime() / 1000;
	}
}