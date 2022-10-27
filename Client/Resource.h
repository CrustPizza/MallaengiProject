#pragma once
#include <unordered_map>

class JsonManager;

/// <summary>
/// ���ҽ��� �����ϴ� Class
/// 
/// Game Class�� �ͼӵǾ� ���˴ϴ�.
///  
/// 2022. 02. 12. Comet
/// </summary>
class Resource
{
public:
	Resource();
	~Resource();

public:
	// Game Class�� Initialize�� ��, ��� ���ҽ��� �ε��ϸ鼭
	// �غ��մϴ�.
	void Initialize();

private:
	// ��� ���� ��������Ʈ ������ �����մϴ�.
	// (�ִϸ��̼��� ���� ��ü�鿡 ���Ǵ� ��������Ʈ�� �����մϴ�.)
	std::unordered_map<std::string, Sprite*> mAllSprites;

	// ��� �ִϸ��̼� ������ �����մϴ�.
	std::unordered_map<std::string, AnimationClip*> mAllAnimationClips;

public:
	Sprite* GetSprite(std::string name) const { return mAllSprites.at(name); }

	AnimationClip* GetAnimationClip(std::string name) const { return mAllAnimationClips.at(name); }
};