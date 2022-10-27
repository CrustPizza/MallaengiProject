#pragma once

#define FIXED_UPDATE_TIME 0.02

class Player;
class CollisionGrid;

// SortOrder�� ���� multi-set�� ������ �ǵ��� ������ִ� �Լ� ��ü
struct RendererCompare
{
	bool operator () (const Renderer* renderer1, const Renderer* renderer2) const
	{
		return renderer2->GetSortOrder() < renderer1->GetSortOrder();
	}
};

/*
 * Game Scene�� ������ Base Scene
 *
 * 2022. 02. 05. Comet.
 */
class Scene
{
public:
	Scene(std::string sceneName);
	virtual ~Scene();

protected:
	std::string mSceneName;

	Player* mGamePlayer;

public:
	bool mIsFirstLoad;

protected:
	std::vector<GameObject*> mGameObjects;

	std::vector<GameObject*> mGlobalGameObjects;

	std::vector<Collision*> mCollisions;

	// Collision Grids !
	std::vector<CollisionGrid*> mCollisionGrids;

	std::multiset<Renderer*, RendererCompare> mRenderers;

	// �ش� ���� ī�޶��Դϴ�. ī�޶� �ø��� ���� ���˴ϴ�.
	GameObject* mCamera;

public:
	void SetCamera(GameObject* camera) { mCamera = camera; }
	GameObject* GetCamera() { return mCamera; }

public:
	// �� �Լ��� override�Ͽ�, ���� �����մϴ�.
	// �׸��� �츮 ������ ������ Ư�� ��, �ε��� �� Scene�� �ʱ�ȭ�ϰ� �ٽ� �����Ѵ�.
	virtual void BuildScene();

	// -1. ���������� �������� �����ϱ� ���� ���� �۾��� �����մϴ�.
	void PrePhase() const;

	// 0. Input, Timer ���� �����͸� ������Ʈ�մϴ�.
	void DataPhase();

	// 1. �浹 ó���� �����մϴ�.
	void PhysicsPhase() const;

	// 1-1. ���� ó���� �����մϴ�.
	void FixedPhase() const;

	// 2. �����Ϳ� �浹�� ���� Game Logic�� ������Ʈ�մϴ�.
	void LogicPhase() const;

	// 3. ��� ó�� �Ϸ� ���� �׸��� ȭ�� �� ǥ���մϴ�.
	void RenderPhase() const;

	// while ('Data -> Physics -> Logic -> Render')
	void SceneLoop();

	// �� �μ���
	void ClearScene();

	void LoadSceneData(const string& path, int mapCount);

	void LoadSceneDataStageThree(const string& path);

public:
	// Add GameObject at collection in scene.
	void AddGameObject(GameObject* object);
	// Add Global GameObject at collection in scene.
	void AddGlobalGameObject(GameObject* object);
	// Subtract GameObject at collection in Scene
	void SubGameObject(GameObject* object);
	// Subtract GameObject at collection in Scene
	void SubGlobalGameObject(GameObject* object);

	std::string GetSceneName();

	Player* GetPlayer() { return mGamePlayer; }
	void SetPlayer(Player* player) { mGamePlayer = player; }

private:
	std::vector<std::pair<GameObject*, float>> mCreateReserveList;
	std::vector<std::pair<GameObject*, float>> mDestroyReserveList;

public:
	// ������ �����մϴ�. �ð��� ������ �� ���� �����ӿ� �����˴ϴ�.
	void ReserveCreate(GameObject* object, float time = 0.f);
	// ������ �����մϴ�. �ð��� ������ �� ���� �����ӿ� �����մϴ�.
	void ReserveDestroy(GameObject* object, float time = 0.f);
	// ��� ������ ���μ���
	void BirthAndDeath();
};