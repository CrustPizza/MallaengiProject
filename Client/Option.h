#pragma once


/**
 * GameLoop�� �ִ� ������ �սô� ..
 */
class Option : public GameObject
{
public:
	Option();
	virtual ~Option();

public:

	virtual void Update();
};

// 1. esc ��ư�� ������ (�ƹ�ư ��ư)

// 2. ������ Game ���� �ִ� Option ��ü�� CurrentScene�� ReserveCreate() �˴ϴ�.

// 3 - 1. Option�� ���� ������ �ٸ� �༮���� Update�� ��� �����. (�ɼǸ� ������Ʈ)

// 3 - 2. ��ư 4���� ���� -> ������ ��ư�� ���� �̺�Ʈ�� �ǽ��Ѵ�.

// 1. �����ϱ� : �׳� ������ endgame == true

// 2. �׸��ϱ� : player ���� �����ϰ� Intro�� �̵�

// 3. ����ϱ� : �ɼ�â ���� �ٽ� ��� �͵��� Update�� �����Ѵ�.

// 4. �ٽ��ϱ� : UI�� ������Ʈ ���ε� .. FadeOUt�ϰ� -> ReserveLoadScene(mCurrenetScene->GetSceneName()) ..?


// 3. �׷� ���� �����ӿ��� Option�� �������� ..?