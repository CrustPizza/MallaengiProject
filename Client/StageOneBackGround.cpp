#include "PreCompiledHeader.h"
#include "StageOneBackGround.h"

#include "Player.h"

#include "MainCamera.h"

#define SCROLL_SPEED_X 0.05f
#define SCROLL_SPEED_Y 0.01f

StageOneBackGround::StageOneBackGround()
{
	SetTag("BackGround");

	AddSpriteRenderer();

	D2D1_SIZE_F size = Graphics::GetInstance().GetScreenSize();

	GetTransform()->SetPosition(Vector2D(size.width, size.height));
}

StageOneBackGround::~StageOneBackGround()
{
	// ����� ..����
}

void StageOneBackGround::LateUpdate()
{
	GameObject::LateUpdate();

	// ī�޶� ��ǥ�� �����ͼ� �׻� ����� ��µ� �� �ְ� �Ѵ�.
	MainCamera* camera = dynamic_cast<MainCamera*>(Game::GetInstance().GetCurrentScene()->GetCamera());
	Vector2D cameraPos = camera->GetTransform()->GetPosition();

	// ��� ũ��
	D2D1_SIZE_F bgSize = GetSpriteRenderer()->GetSprite()->mSize;

	// ȭ�� ũ��
	D2D1_SIZE_F size = { Graphics::GetInstance().GetScreenSize().width / CAMERA_ZOOM, Graphics::GetInstance().GetScreenSize().height / CAMERA_ZOOM };
	
	// ȭ��� ��� ũ�� ������ ����
	FLOAT moreThanScreenX = bgSize.width - size.width;
	FLOAT moreThanScreenY = bgSize.height - size.height;

	if (moreThanScreenX < 0)
		moreThanScreenX = 0;

	if (moreThanScreenY < 0)
		moreThanScreenY = 0;

	// �ణ �̷� ������ ����ϸ� �����ϰ� ������ó�� ��ũ�ѵǴ� �������� �� �� ���� ������ ..?
	GetTransform()->SetPosition(Vector2D(
	cameraPos.mX + moreThanScreenX * (0.5f - camera->GetXMovablePercent()), 
	cameraPos.mY + moreThanScreenY * (0.5f - camera->GetYMovablePercent())));
}