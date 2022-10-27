#include "PreCompiledHeader.h"
#include "TransBox.h"

TransBox::TransBox()
{
	// �±׸� TransBox ���� �����Ѵ�.
	SetTag("TransBox");
}

TransBox::~TransBox()
{
	
}

void TransBox::TriggerEvent()
{
	GetSpriteRenderer()->SetOpacity(1.f);

	SoundManager::GetInstance()->Play(500031);

	SetTag("BoxBase");
}