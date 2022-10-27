#include "PreCompiledHeader.h"
#include "PlatformBase.h"

PlatformBase::PlatformBase():
	mSize({ TILE_WIDTH + 1, TILE_HEIGHT}),
	mCollisionDir(CollisionDir::NONE)
{
	// �÷����� AABBCollision���� �۵��Ѵ�.
	mAABBCollision = AddAABBCollision();

	AddSpriteRenderer();

	// �±׸� �÷��� ���̽����� �����Ѵ�.
	SetTag("PlatformBase");
}

PlatformBase::~PlatformBase()
{

}

void PlatformBase::FixedUpdate()
{
	GameObject::FixedUpdate();
}

void PlatformBase::Update()
{
	GameObject::Update();
}