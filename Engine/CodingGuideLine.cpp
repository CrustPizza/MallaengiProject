#include "pch.h"
#include "CodingGuideLine.h"

CodingGuideLine::CodingGuideLine()
	: mNumber(nullptr)							// ���� �ʱ�ȭ ���ֱ�
{
	// �ʱ�ȭ �ȿ����� ����
}

CodingGuideLine::~CodingGuideLine()
{
}

void CodingGuideLine::Initialize()
{
	int num = 10;
	SetNumber(num);

	// == �� �ƴ� != �� ����Ͽ� nullptr �� �ƴҶ��� ����  ->  �̰� ������ �³�?
	if (mNumber != nullptr)
	{
		
	}
}
