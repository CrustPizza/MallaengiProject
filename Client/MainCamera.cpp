#include "PreCompiledHeader.h"
#include "MainCamera.h"

#include "Player.h"

#define CAMERA_POS_PER_PLAYER 0.4f

MainCamera::MainCamera(float mapWidth, float mapHeight)
	:mPlayer(Game::GetInstance().GetPlayer())
	, mMapWidth(mapWidth)
	, mMapHeight(mapHeight)
{
	SetTag("Camera");

	AddCamera();

	GetTransform()->SetPosition(mPlayer->GetTransform()->GetPosition());

	GetTransform()->SetScale(Vector2D(CAMERA_ZOOM, CAMERA_ZOOM));

	Game::GetInstance().GetCurrentScene()->SetCamera(this);
}

MainCamera::~MainCamera()
{
}

void MainCamera::LateUpdate()
{
	GameObject::LateUpdate();

	// �� ī�޶� ������ Player�� ���߾ӿ� �ΰ� �������ϴ� ī�޶��Դϴ�.
	// ��ȹ �ǵ��� ���� �ڵ带 �ۼ����־���մϴ�.

	Vector2D playerPos = mPlayer->GetTransform()->GetPosition();

	// �÷��̾� ��ġ�� �۾����� (�����ϸ�) ==> ī�޶� ��ġ�� �̿� ����ؼ� �ö󰣴�.
	float adjust = mMapHeight * CAMERA_ZOOM + (playerPos.mY - mMapHeight) * CAMERA_POS_PER_PLAYER;

	if (adjust < Graphics::GetInstance().GetScreenSize().height * CAMERA_ZOOM + 20)
		adjust = Graphics::GetInstance().GetScreenSize().height * CAMERA_ZOOM + 20;
	else if (adjust > mMapHeight - Graphics::GetInstance().GetScreenSize().height * CAMERA_ZOOM - 20)
		adjust = mMapHeight - Graphics::GetInstance().GetScreenSize().height * CAMERA_ZOOM - 20;

	if (playerPos.mX < Graphics::GetInstance().GetScreenSize().width * CAMERA_ZOOM)
		GetTransform()->SetPosition(Vector2D(Graphics::GetInstance().GetScreenSize().width * CAMERA_ZOOM, adjust));
	else if (playerPos.mX > mMapWidth - Graphics::GetInstance().GetScreenSize().width * CAMERA_ZOOM)
		GetTransform()->SetPosition(Vector2D(mMapWidth - Graphics::GetInstance().GetScreenSize().width * CAMERA_ZOOM, adjust));
	else
		GetTransform()->SetPosition(Vector2D(playerPos.mX, adjust));

	// ī�޶��� ù ������ 3105, 3105���ٴ� �� Ŀ�� �� ����.

	// �÷��̾��� ��ġ ���� �÷��̾� ��ġ�� �پ�� ���� ī�޶��� ��ġ�� ����Ѵ�.
}

float MainCamera::GetXMovablePercent()
{
	float progress = GetTransform()->GetPosition().mX - Graphics::GetInstance().GetScreenSize().width * CAMERA_ZOOM;
	float max = mMapWidth - Graphics::GetInstance().GetScreenSize().width * CAMERA_ZOOM * 2;

	return progress / max;
}

float MainCamera::GetYMovablePercent()
{
	float progress = GetTransform()->GetPosition().mY - Graphics::GetInstance().GetScreenSize().height * CAMERA_ZOOM + 20;
	float max = mMapHeight - Graphics::GetInstance().GetScreenSize().height * CAMERA_ZOOM * 2;

	return progress / max;
}