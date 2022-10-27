#include "PreCompiledHeader.h"
#include "IntroBackGround.h"

#include "Player.h"

#define INTRO_SCROLL_X_SPEED 0.1f
#define INTRO_SCROLL_Y_SPEED 0.01f

IntroBackGround::IntroBackGround()
{
	SetTag("BackGround");

	AddSpriteRenderer()->SetSortOrder(10);

	D2D1_SIZE_F size = Graphics::GetInstance().GetScreenSize();

	GetTransform()->SetPosition(Vector2D(size.width / 2, size.height / 2 - 300.f));
}

IntroBackGround::~IntroBackGround()
{
}

void IntroBackGround::LateUpdate()
{
	GameObject::LateUpdate();

	// Intro Scene���� ����� �̵���Ű�� �ʾƼ� ��ü �ּ�

	//// �÷��̾��� ��ġ�� ������ ���丮ó�� ����� �� �ֵ���
	//Vector2D playerPos = Game::GetInstance().GetPlayer()->GetTransform()->GetPosition();

	//D2D1_SIZE_F size = Graphics::GetInstance().GetScreenSize();

	//// �ణ �̷� ������ ����ϸ� �����ϰ� ������ó�� ��ũ�ѵǴ� �������� �� �� ���� ������ ..?
	//GetTransform()->SetPosition(Vector2D(size.width / 2 - 70.f - size.width / 2 * INTRO_SCROLL_X_SPEED, size.height / 2 - 300.f - size.height / 2 * INTRO_SCROLL_Y_SPEED));
}