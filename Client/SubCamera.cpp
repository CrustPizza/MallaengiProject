#include "PreCompiledHeader.h"
#include "SubCamera.h"

SubCamera::SubCamera()
{
	SetTag("Camera");

	AddCamera();

	D2D1_SIZE_F size = GetCamera()->GetSize();
	  
	// SubCamera�� �����̴� ..!
	GetTransform()->SetPosition(Vector2D(size.width / 2 / CAMERA_ZOOM, size.height / 2 / CAMERA_ZOOM));
	// ī�޶� ��.
	GetTransform()->SetScale(Vector2D(CAMERA_ZOOM, CAMERA_ZOOM));

	Game::GetInstance().GetCurrentScene()->SetCamera(this);
}

SubCamera::~SubCamera()
{

}

void SubCamera::LateUpdate()
{
	GameObject::LateUpdate();

	// ���� ������Ʈ�� �ʿ䰡 �����ϴ�. ���߾ӿ� �����Ǵ� ī�޶�ϱ� ..!
}