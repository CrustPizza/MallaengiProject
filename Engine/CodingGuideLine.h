#pragma once

/// <summary>
/// 2022. 02. 08. MG
/// 
/// �ڵ� ���̵������ ���� Ŭ����
/// 
/// Ŭ���� ������ �ּ����� �� �����(����� �޶� OK)
/// </summary>
class CodingGuideLine
{
public:
	// ������, �Ҹ��ڴ� �⺻���� ����, ���� �����ڰ� �ִٸ� �� �����ڸ� ����
	CodingGuideLine();
	~CodingGuideLine();

	// Ŭ������ ó���� ���� ������ֱ�
private:
	int* mNumber;									// ������ ������ ��� private ���� ����


	// �Ʒ��� �Լ� ������ֱ�
public:
	// ����� �Լ� ����
	void Initialize();



	// Get, Set �� �Լ� �� ������
	int GetNumber() { return *mNumber; }				// private �����̹Ƿ� �ۿ��� �� �� �ֵ��� Get ������ֱ�
	void SetNumber(int num) { *mNumber = num; }		// ������ �� ��ȭ �ʿ��� ��� Set ������ֱ�
};

