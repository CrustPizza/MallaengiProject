#pragma once

class FallingBlock;
/**
 * �ð����� �ΰ� ���߿��� �������� ����. ������ �״´�.
 *
 * 2022. 02. 13. Comet
 */
class TimeDiffTrap : public GameObject
{
public:
	TimeDiffTrap();
	virtual ~TimeDiffTrap();

	std::vector<FallingBlock*> mFallingBlocks;

public:
	virtual void FixedUpdate() override;
	virtual void Update() override;
};