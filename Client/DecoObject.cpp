#include "PreCompiledHeader.h"
#include "DecoObject.h"

DecoObject::DecoObject()
{
	// �̰��� ���̴�. ���ڴϱ� ��Ʈ������ �� �ڷ� ���ݽô� ..!
	AddSpriteRenderer()->SetSortOrder(7);
}

DecoObject::~DecoObject()
{

}