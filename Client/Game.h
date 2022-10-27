#pragma once
#include "Scene.h"
#include "Resource.h"

class Exit;
class Title;
class Back;
class Restart;
class UIDecoObject;
class Player;
class CollisionGrid;

/**
 * Client���� ���� ������ �Ѱ�
 *
 * 2022. 02. 05. Comet
 */
class Game
{
public:
	Game();
	~Game();

	// Singleton
	static Game& GetInstance()
	{
		static Game instance;
		return instance;
	}

	void Initialize(HWND hWnd);

	// ���� ���õ� ���� ���� Loop�� �����鼭 ������ �����մϴ�.
	bool GameLoop();

	// �� �Լ��� �̿��� ����.
	void EndGame();

private:
	// Game�� �������� �� �� ģ���� True�� ���ݴϴ�.
	bool mIsEndGame;

	// ���� �� ���� �÷��̾��� ������ ��� ��� �ְڽ��ϴ�.
	// �� �� ���� ������ ������ �ִ� �÷��̾ ȣ���ؾ��ϱ� �����Դϴ�.
	Player* mGamePlayer;

	// ���̺� ����Ʈ�� �������� �� �����մϴ�.
	float mSaveBodyWeight;

	// ���̺� ����Ʈ�� �������� �� �� ��ġ�� �����մϴ�.
	Vector2D mSavePosition;

	// �������� 3�� ���� �� �����մϴ�. �� ģ���� ���ؼ� ������ ����ϰڽ��ϴ�.
	float mLastBodyWeight;

	// �浹 ����� ���
	bool mIsDebugMode;

	// �ɼ� ��� => �������������� ų �� �ִ� ?
	bool mIsOptionOn;

public:
	Player* GetPlayer() const { return mGamePlayer; }

	// ������ ������ Save
	void SaveLastBodyWeight(float value) { mLastBodyWeight = value; }

	float GetLastBodyWeight() { return mLastBodyWeight; }

private:
	// Scene���� ��Ƶд�.
	std::map<std::string, Scene*> mScenes;

	// ���� �÷��� ���� �������� ��
	Scene* mCurrentStage;

	// ���� ���� ������ ���ư� ��
	Scene* mCurrentScene;

	// Scene Load�� ����Ǹ� ���⿡ �����صӴϴ�.
	Scene* mNextScene;

	// �� �ε尡 ����Ǿ� �ִٸ�, ���� �����ӿ� ����� ���� �ε��մϴ�.
	bool mIsReservedLoadScene;

public:
	// ���� ����մϴ�.
	void RegisterScene(Scene* scene);

	// Scene Load�� �����մϴ�.
	void ReserveLoadScene(std::string sceneName);

	// ������ �Ǿ� �ִٸ� Scene�� Load�մϴ�.
	void LoadScene();

	// ���� Loop ���� ���� ��ȯ�մϴ�. �̸� �޾�
	// �� ���� �� ������ ���� ��ü ���� ���� ������ �����Դϴ�.
	Scene* GetCurrentScene() { return mCurrentScene; }

private:
	float mStagePlayTime;

private:
	// Resource Managing
	Resource* mResource;

public:
	// �ۿ����� �� �Լ��� ���� ���ҽ��� �����մϴ�.
	Resource* GetResource() const { return mResource; }

public:
	// �������� ���� ������ �� �̰� ���ָ� �Ǵ°��ݾ� ..?
	void SetCurrentStage(Scene* scene);

	float GetSaveBodyWeight() { return mSaveBodyWeight; }
	void SetSaveBodyWeight(float value) { mSaveBodyWeight = value; }

	Vector2D GetSavePosition() { return mSavePosition; }
	void SetSavePosition(Vector2D position) { mSavePosition = position; }

	bool GetIsDebugMode() { return mIsDebugMode; }

	bool GetIsOptionOn() { return mIsOptionOn; }
	void OptionToggle();

	void SetIsOptionClicked() { mIsOptionClicked = true; }
	
	// �÷��̾ �ʱ�ȭ ���ݴϴ�.
	void InitPlayer();
	// ��� ������ �ʱ�ȭ ���ݴϴ�.
	void InitScenes();
	// �ɼ� �ʱ�ȭ
	void InitOption();

private:
	void OnOption();
	void OffOption();

	std::vector<GameObject*> mMenuList;

	bool mIsOptionClicked;
};