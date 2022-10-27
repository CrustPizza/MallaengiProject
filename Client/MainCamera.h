#pragma once

class Player;

/**
 * ������������ ������ �÷����� �� ���� ī�޶�
 *
 * �÷��̾��� ��ġ�� �������� ī�޶� Control
 *
 * 2022. 02. 12. Comet
 */
class MainCamera : public GameObject
{
public:
	MainCamera(float mapWidth, float mapHeight);
	virtual ~MainCamera();

	Player* mPlayer;

	float mMapWidth;
	float mMapHeight;

public:
	virtual void LateUpdate() override;
	float GetXMovablePercent();
	float GetYMovablePercent();
};