#include "PreCompiledHeader.h"
#include "BlockBase.h"
#include "Player.h"

BlockBase::BlockBase() :
	mSize({ TILE_WIDTH, TILE_HEIGHT })
{
	// ����� AABBCollision���� �۵��Ѵ�.
	mAABBCollision = AddAABBCollision();

	AddSpriteRenderer();

	// �±׸� ��� ���̽��� �����Ѵ�.
	SetTag("BlockBase");
}

BlockBase::~BlockBase()
{

}

void BlockBase::FixedUpdate()
{
	
}

void BlockBase::Update()
{
	GameObject::Update();
}