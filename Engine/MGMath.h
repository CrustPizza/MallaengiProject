#pragma once

/// <summary>
/// �������� ������ ���� �Լ����� ��Ƴ��� Ŭ�����Դϴ�.
/// 
/// 2022. 02. 07. MG�� ��ȥ
/// </summary>
class MGMath
{
public:
	template<typename T>
	static T Clamp(T value, T min, T max);
};

template<typename T>
T MGMath::Clamp(T value, T min, T max)
{
	if (value < min)
		return min;

	if (value > max)
		return max;

	return value;
}