#pragma once

/// <summary>
/// 2022. 02. 08. MG
/// 
/// ��������Ʈ ������
/// </summary>
class SpriteRenderer : public Renderer
{
public:
	SpriteRenderer(GameObject* gameObject);
	virtual ~SpriteRenderer();

private:
	Sprite* mSprite;			// ����� ��������Ʈ ����

	float mOpacity;				// ����

public:
	// �׸� �׸� ��������Ʈ�� ������ �־��ִ� �Լ�
	void SetSprite(Sprite* sprite);
	Sprite* GetSprite() { return mSprite; }

	float GetOpacity() { return mOpacity; }

	void SetOpacity(float opacity) { mOpacity = opacity; }

	virtual void Render() override;
	virtual void Release() override;
};