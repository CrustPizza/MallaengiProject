#pragma once
#include "PlatformBase.h"

/// <summary>
/// �⺻���� �÷��� ��ü. �� ģ���� ���� ä��ô�.
/// 
/// 2022. 02. 09. Comet
/// </summary>
class Platform : public PlatformBase
{
public:
	Platform();
	virtual ~Platform();

public:
	virtual void FixedUpdate() override;

	virtual void Update() override;
};