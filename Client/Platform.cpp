#include "PreCompiledHeader.h"
#include "Platform.h"

Platform::Platform():
	PlatformBase()
{
	// �±׸� �÷������� �����Ѵ�.
	SetTag("Platform");
}

Platform::~Platform()
{

}

void Platform::FixedUpdate()
{
	PlatformBase::FixedUpdate();
}

void Platform::Update()
{
	PlatformBase::Update();
}