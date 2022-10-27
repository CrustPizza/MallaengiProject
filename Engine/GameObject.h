#pragma once
#include <string>
#include "AABBCollision.h"
#include "CircleCollision.h"
#include "Renderer.h"
#include "Animator.h"
#include "SpriteRenderer.h"
#include "RectRenderer.h"
#include "Camera.h"
#include "Timer.h"

class Transform;

/// <summary>
/// 2022. 02. 08. MG
/// 
/// ���� ���� ��� ������Ʈ�� �����ϱ� ���� Ŭ����
/// </summary>
class GameObject
{
	// �ʿ��Ѱ�
public:
	GameObject();
	virtual ~GameObject();

	// ���� ���� ������ �ִٸ�, ���⼭ ó���ϰڽ��ϴ�.
	virtual void FixedUpdate();

	// ���⼭ ������Ʈ���� Update�� �����ϰ�
	// ��ӹ��� ��ü���� Update (Script ����)�� ���� Override �� ������ ��ȹ�Դϴ�.
	virtual void Update();

	// ���� �������� Update �Ǿ���ϴ� ģ���� (E.x. Camera)
	virtual void LateUpdate();

	virtual void Release() {}

protected:
	// �߰��� ������ �Ҹ��ڿ��� delete �߰��ϴ� �� ������ ..!
	// Getter / Adder �߰��ϴ� �͵� ..!
	Transform* mTransform;

	Collision* mCollision;

	Renderer* mRenderer;

	Animator* mAnimator;

	Camera* mCamera;

	GameObject* mReservedGameObject;							// ��������� ���� ������Ʈ�� ������ ��� ������ ����

	double mTime;

	std::string mTag;											// ������Ʈ���� �з��� ���� ����

	unsigned int mID;											// Ÿ�� ID

public:
	Transform* GetTransform() { return mTransform; }

	Collision* GetCollision() { return mCollision; }
	AABBCollision* GetAABBCollision() { return dynamic_cast<AABBCollision*>(mCollision); }
	CircleCollision* AddCircleCollision();
	AABBCollision* AddAABBCollision();

	Renderer* GetRenderer() { return mRenderer; }
	RectRenderer* AddRectRenderer();
	SpriteRenderer* AddSpriteRenderer();
	SpriteRenderer* GetSpriteRenderer() { return dynamic_cast<SpriteRenderer*>(mRenderer); }

	Animator* GetAnimator() { return mAnimator; }
	Animator* AddAnimator() { mAnimator = new Animator(this); return mAnimator; }

	Camera* GetCamera() { return mCamera; }
	Camera* AddCamera() { mCamera = new Camera(this); Graphics::GetInstance().SetCamera(mCamera); return mCamera; }

	std::string GetTag() { return mTag; }
	void SetTag(std::string tag) { mTag = tag; }
	void SetID(unsigned int ID) { mID = ID; }
	unsigned int GetID() { return mID; }
};

// ���� ������Ʈ �����ϴ� ���

// ���� ���� (���� �������� ������ �� ������ ����� ģ������ �� �����ϰ� ������ ������Ʈ�� ����.)

// ������ �����Ѵٴ� ���� �� ģ���� ���� �÷��� (mReservedDestroy ���شٰų� ��� ..)

// �׷� ���� ������Ʈ�Ҷ����� üũ�ؼ� �����ϸ� �׸��̾�. ���� ���� ?


// 1. ������ �����ϴ� ���

// 2. ������ �����ϴ� ���


// ���� �ڽ��� �浹����